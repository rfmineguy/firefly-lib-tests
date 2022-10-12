#include <firefly/Core/Core.h>
#include <firefly/IO/IO.h>
#include <firefly/Resource/Resource.h>
#include <firefly/Rendering/Renderer.h>
#include <firefly/Core/OrthoCamera.h>
#include <firefly/Core/PerspectiveCamera.h>
#include <stdbool.h>

typedef struct {
  float x, y, w, h;
} Rect;

typedef struct {
  Rect r;
  float vel_x, vel_y;
} Ball;

void setup_keybinds() {}

void init() {
  SetLogStream(stdout);
  FF_InitKeybindHT();
  FF_InitAudioSystem();
  FF_RandomSeedTime();
  FF_SoundListener((vec3){0, 0, 0}, (vec3){0, 0, 0});

  setup_keybinds();
}

void deinit(Window* w) {
  FF_DeinitRenderer();
  FF_DeinitAudioSystem();
  FF_DeinitKeybindHT();
  FF_DestroyWindowGL(w);
}

void run_left_paddle(Rect* left_paddle, int h) {
    if (FF_IsKeyDown(KEY_S))
      left_paddle->y += 4;
    if (FF_IsKeyDown(KEY_W))
      left_paddle->y -= 4;
    if (left_paddle->y < 0)
      left_paddle->y = 0;
    if (left_paddle->y + left_paddle->h > h)
      left_paddle->y = h - left_paddle->h;
}
void run_right_paddle(Rect* right_paddle, int h) {
    if (FF_IsKeyDown(KEY_DOWN))
      right_paddle->y += 4;
    if (FF_IsKeyDown(KEY_UP))
      right_paddle->y -= 4;
    if (right_paddle->y < 0)
      right_paddle->y = 0;
    if (right_paddle->y + right_paddle->h > h)
      right_paddle->y = h - right_paddle->h;
}
void run_ball(Ball* ball, Window* w) {
  if (ball->r.x < 0 || ball->r.x + ball->r.w > FF_WindowGetWidth(w)) {
    ball->vel_x *= -1;
  }
  if (ball->r.y < 0 || ball->r.y > FF_WindowGetHeight(w)) {
    ball->vel_y *= -1;
  }
  ball->r.x += ball->vel_x * FF_WindowDeltaTime(w);
  ball->r.y += ball->vel_y * FF_WindowDeltaTime(w);
}

void reset_ball(Ball* ball, Window* w) {
  ball->vel_x = (100 + FF_RandomRangeFloat(-20, 20)) * FF_RandomFromSet(2, (double[]){-1, 1});
  ball->vel_y = (150 + FF_RandomRangeFloat(-20, 20)) * FF_RandomFromSet(2, (double[]){-1, 1});
  ball->r.x = FF_WindowGetWidth(w) / 2.f;
  ball->r.y = FF_WindowGetHeight(w) / 2.f;
}

//collision assuming x,y is the center
bool collided(Rect a, Rect b) {
  bool xOverlap = a.x < b.x + b.w && a.x + a.w > b.x;
  bool yOveralp = a.y < b.y + b.h && a.y + a.h > b.y;
  return xOverlap && yOveralp;
}

int main() {
  init();
  Window* w = FF_CreateWindowGL("Pong", 600, 600);
  FF_InitRenderer();
  
  Geometry quad = GenerateQuad();
  Camera c = FF_OrthoCamera();
  Texture t = FF_LoadTexture("res/splotch.png");
  Sound paddle_bounce = FF_LoadSound("res/sounds/paddle_bounce.wav");
  Sound death = FF_LoadSound("res/sounds/death_sound.wav");
  SoundSource source = FF_SoundSourceEx(1.0f, 1.0f, (vec3){0, 0, 0}, false);
  Ball ball = (Ball) { .r=(Rect){300, 300, 20, 20}, .vel_x=100, .vel_y=150 };
  Rect left_paddle = (Rect){40, FF_WindowGetHeight(w) / 2.f - 75, 10, 150};
  Rect right_paddle = (Rect){FF_WindowGetWidth(w) - 50, FF_WindowGetHeight(w) / 2.f - 75, 10, 150};
  int left_score = 0, right_score = 0;
  
  while (!FF_WindowShouldClose(w)) {
    FF_WindowPollEvents(w);
    FF_WindowClearBackgroundEx(w, 0.4f, 0.4f, 0.4f, 1.0f);
    
    run_left_paddle(&left_paddle, FF_WindowGetHeight(w));
    run_right_paddle(&right_paddle, FF_WindowGetHeight(w));
    run_ball(&ball, w);

    if (collided(ball.r, left_paddle) || collided(ball.r, right_paddle)) {
      ball.vel_x *= -1;
      FF_SoundSourcePlay(source, paddle_bounce);
    }
    if (collided(ball.r, (Rect){1, 0, 1, FF_WindowGetHeight(w)})) {
      LOG_INFO("RIGHT SCORED");
      FF_SoundSourcePlay(source, death);
      reset_ball(&ball, w);
    }

    if (collided(ball.r, (Rect){FF_WindowGetWidth(w) - 1, 0, 1, FF_WindowGetHeight(w)})) {
      LOG_INFO("LEFT SCORED");
      FF_SoundSourcePlay(source, death);
      reset_ball(&ball, w);
    }
    
    if (FF_IsKeyPressed(KEY_R)) {
      reset_ball(&ball, w);
    }
    if (FF_IsKeyPressed(KEY_P)) {
      LOG_INFO("%0.4f", FF_RandomFromSet(5, (double[]){2, 3, -5, 31, 4}));
    }

    FF_OrthoCameraUpdate(&c, false);
    FF_BindTexture(t);

    // draw ball
    FF_RendererDrawGeometryEx(quad, c, (vec3){ball.r.x, ball.r.y, 0}, (vec3){ball.r.w, ball.r.h, 1}, (vec3){0}, 0);
    FF_RendererDrawGeometryEx(quad, c, (vec3){left_paddle.x, left_paddle.y, 0}, (vec3){left_paddle.w, left_paddle.h, 1}, (vec3){0}, 0);
    FF_RendererDrawGeometryEx(quad, c, (vec3){right_paddle.x, right_paddle.y, 0}, (vec3){right_paddle.w, right_paddle.h, 1}, (vec3){0}, 0);
  }
  
  FF_FreeSound(paddle_bounce);
  FF_FreeSound(death);
  FreeGeometry(quad);
  FF_FreeTexture(t);
  
  deinit(w);
}