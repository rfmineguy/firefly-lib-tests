#include <firefly/Core/Core.h>
#include <firefly/IO/IO.h>
#include <firefly/Resource/Resource.h>
#include <firefly/Rendering/Renderer.h>
#include <firefly/Core/OrthoCamera.h>
#include <firefly/Core/PerspectiveCamera.h>
#include <firefly/Core/Interpolation.h>
#include <stdbool.h>

/*
  Learn about arena allocators and try to see if it will make a difference for a new resource manager approach
*/
typedef struct {
  float x, y, w, h;
} Rect;

typedef struct {
  Rect r;
  float vel_x, vel_y;
} Ball;

// SETUP
void setup_keybinds() {
  FF_KeyBindRegister("left_paddle_up", KEY_W, KEY_NONE);
  FF_KeyBindRegister("left_paddle_down", KEY_S, KEY_NONE);
  FF_KeyBindRegister("right_paddle_up", KEY_UP, KEY_NONE);
  FF_KeyBindRegister("right_paddle_down", KEY_DOWN, KEY_NONE);
}

void init() {
  SetLogStream(stdout);
  FF_InitKeybindHT();
  FF_RandomSeedTime();

  setup_keybinds();
}

void deinit(FF_Window* w, FF_Renderer* r, FF_AudioSystem* as) {
  FF_DeinitKeybindHT();
  FF_DestroyAudioSystem(as);
  FF_DestroyRenderer(r);
  FF_DestroyWindowGL(w);
}

void run_paddle_human(Rect* paddle, const char* up, const char* down, int h) {
  if (FF_IsKeyBindDown(up)) {
    paddle->y -= 4;
  }
  if (FF_IsKeyBindDown(down)) {
    paddle->y += 4;
  }
  if (paddle->y < 0)
    paddle->y = 0;
  if (paddle->y + paddle->h > h)
    paddle->y = h - paddle->h;
}
void run_paddle_ai(Rect* paddle, Rect* ball, double dt) {
  paddle->y = FF_Lerp(paddle->y, ball->y, dt * 5);
}
void run_ball(Ball* ball, FF_Window* w) {
  if (ball->r.x < 0 || ball->r.x + ball->r.w > FF_WindowGetWidth(w)) {
    ball->vel_x *= -1;
  }
  if (ball->r.y < 0 || ball->r.y > FF_WindowGetHeight(w)) {
    ball->vel_y *= -1;
  }
  ball->r.x += ball->vel_x * FF_WindowDeltaTime(w);
  ball->r.y += ball->vel_y * FF_WindowDeltaTime(w);
}
void reset_ball(Ball* ball, FF_Window* w) {
  ball->vel_x = (100 + FF_RandomRangeFloat(-20, 20)) * FF_RandomFromSet(2, (double[]){-1, 1});
  ball->vel_y = (150 + FF_RandomRangeFloat(-20, 20)) * FF_RandomFromSet(2, (double[]){-1, 1});
  ball->r.x = FF_WindowGetWidth(w) / 2.f;
  ball->r.y = FF_WindowGetHeight(w) / 2.f;
}
bool collided(Rect a, Rect b) {
  bool xOverlap = a.x < b.x + b.w && a.x + a.w > b.x;
  bool yOveralp = a.y < b.y + b.h && a.y + a.h > b.y;
  return xOverlap && yOveralp;
}

int main() {
  init();
  FF_Window* w = FF_CreateWindowGL("Pong", 600, 600, false);
  FF_Renderer* r = FF_CreateRenderer(w);
  FF_AudioSystem* audioSystem = FF_CreateAudioSystem();
  FF_SetAudioListener(audioSystem, (vec3){0, 0, 0}, (vec3){0, 0, 0});
  
  Geometry quad = GenerateQuad();
  Camera c = FF_OrthoCamera();
  Texture t = FF_LoadTexture("../res/splotch.png");
  
  FF_Sound paddle_bounce = FF_LoadSound("../res/paddle_bounce.wav");
  FF_Sound death = FF_LoadSound("../res/death_sound.wav");
  FF_SoundSource source = FF_CreateSoundSourceEx(audioSystem, 1.0f, 1.0f, (vec3){0, 0, 0}, false);

  Ball ball = (Ball) { .r=(Rect){300, 300, 20, 20}, .vel_x=100, .vel_y=150 };
  Rect left_paddle = (Rect){40, FF_WindowGetHeight(w) / 2.f - 75, 10, 150};
  Rect right_paddle = (Rect){FF_WindowGetWidth(w) - 50, FF_WindowGetHeight(w) / 2.f - 75, 10, 150};
  Rect test_lerp = (Rect){40, 80, 30, 30};
  int left_score = 0, right_score = 0;
  
  while (!FF_WindowShouldClose(w)) {
    FF_WindowPollEvents(w);
    FF_WindowClearBackgroundEx(w, 0.4f, 0.4f, 0.4f, 1.0f);
    
    //run_paddle_human(&left_paddle, "left_paddle_up", "left_paddle_down", FF_WindowGetHeight(w));
    run_paddle_human(&right_paddle, "right_paddle_up", "right_paddle_down", FF_WindowGetHeight(w));
    run_paddle_ai(&left_paddle, &ball.r, FF_WindowDeltaTime(w));
    run_ball(&ball, w);

    if (collided(ball.r, left_paddle) || collided(ball.r, right_paddle)) {
      ball.vel_x *= -1;
      FF_SoundSourcePlay(source, paddle_bounce);
    }
    if (collided(ball.r, (Rect){1, 0, 1, FF_WindowGetHeight(w)})) {
      LOG_INFO("RIGHT SCORED");
      FF_SoundSourcePlay(source, death);
      reset_ball(&ball, w);
      right_score++;
    }
    if (collided(ball.r, (Rect){FF_WindowGetWidth(w) - 1, 0, 1, FF_WindowGetHeight(w)})) {
      LOG_INFO("LEFT SCORED");
      FF_SoundSourcePlay(source, death);
      reset_ball(&ball, w);
      left_score++;
    }
    if (FF_IsKeyPressed(KEY_R)) {
      reset_ball(&ball, w);
    }
    
    //NOTE: This isn't framerate independent, see SimonDev video on Lerp
    test_lerp.x = FF_LerpFunc(FF_LINEAR, test_lerp.x, FF_GetMousePositionX() - test_lerp.w/2, 0.1);
    test_lerp.y = FF_LerpFunc(FF_LINEAR, test_lerp.y, FF_GetMousePositionY() - test_lerp.h/2, 0.1);

    // get ready for rendering
    FF_OrthoCameraUpdate(&c, false);
    FF_BindTexture(t);

    // draw ball (figure out how to batch or instance render some of this maybe)
    FF_RendererDrawGeometry(r, quad, c, (vec3){ball.r.x, ball.r.y, 0},             (vec3){ball.r.w, ball.r.h, 1},             (vec3){0}, 0);
    FF_RendererDrawGeometry(r, quad, c, (vec3){left_paddle.x, left_paddle.y, 0},   (vec3){left_paddle.w, left_paddle.h, 1},   (vec3){0}, 0);
    FF_RendererDrawGeometry(r, quad, c, (vec3){right_paddle.x, right_paddle.y, 0}, (vec3){right_paddle.w, right_paddle.h, 1}, (vec3){0}, 0);
    FF_RendererDrawGeometry(r, quad, c, (vec3){test_lerp.x, test_lerp.y, 0},       (vec3){test_lerp.w, test_lerp.h, 1},       (vec3){0}, 0);
    //FF_RendererShow(r);
  }
  
  FF_FreeSound(paddle_bounce);
  FF_FreeSound(death);
  FreeGeometry(quad);
  FF_FreeTexture(t);
  
  deinit(w, r, audioSystem);
}
