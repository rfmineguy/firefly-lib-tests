#include <firefly/Core/Core.h>
#include <firefly/IO/IO.h>
#include <firefly/Resource/Resource.h>
#include <firefly/Rendering/Renderer.h>
#include <firefly/Rendering/Rect.h>
#include <firefly/Core/OrthoCamera.h>
#include <firefly/Core/PerspectiveCamera.h>
#include <firefly/Core/Interpolation.h>
#include <stdbool.h>
#include "ball.h"
#include "paddle.h"

/*
  Learn about arena allocators and try to see if it will make a difference for a new resource manager approach
*/

// SETUP
void setup_keybinds() {
  FF_KeyBindRegister("left_paddle_up", KEY_W, KEY_NONE);
  FF_KeyBindRegister("left_paddle_down", KEY_S, KEY_NONE);
  FF_KeyBindRegister("right_paddle_up", KEY_UP, KEY_NONE);
  FF_KeyBindRegister("right_paddle_down", KEY_DOWN, KEY_NONE);
}

void init() {
  SetLogStream(stdout);
  FF_InitKeybindHT();      //figure out how to unglobalize this maybe? this is required due to a hashmap being used
  FF_RandomSeedTime();

  setup_keybinds();
}

void deinit(FF_Window* w, FF_Renderer* r, FF_AudioSystem* as, FF_FontLoader* fl) {
  FF_DeinitKeybindHT();
  FF_DestroyAudioSystem(as);
  FF_DestroyFontLoader(fl);
  FF_DestroyRenderer(r);
  FF_DestroyWindowGL(w);
}

bool collided(FF_Rect a, FF_Rect b) {
  bool xOverlap = a.x < b.x + b.w && a.x + a.w > b.x;
  bool yOveralp = a.y < b.y + b.h && a.y + a.h > b.y;
  return xOverlap && yOveralp;
}

int main() {
  /*
   * FIREFLY LIB SETUP CODE
   */
  init();
  FF_Window* w = FF_CreateWindowGL("Pong", 600, 600, false);
  FF_Renderer* r = FF_CreateRenderer(w);
  FF_AudioSystem* audioSystem = FF_CreateAudioSystem();
  FF_FontLoader* fontLoader = FF_CreateFontLoader();
  FF_SetAudioListener(audioSystem, (vec3){0, 0, 0}, (vec3){0, 0, 0});
  
  /*
   * LIBRARY RESOURCE LOADING/CREATION
   */
  Geometry quad = GenerateQuad();
  Camera c = FF_OrthoCamera();
  Texture t = FF_LoadTexture("../res/splotch.png");
  FF_Sound paddle_bounce = FF_LoadSound("../res/paddle_bounce.wav");
  FF_Sound death = FF_LoadSound("../res/death_sound.wav");
  FF_SoundSource source = FF_CreateSoundSourceEx(audioSystem, 1.0f, 1.0f, (vec3){0, 0, 0}, false);
  FF_Font f = FF_LoadFont(fontLoader, "/System/Library/Fonts/Supplemental/Times New Roman.ttf");
  
  /*
   * GAME SPECIFIC DATA (PONG)
   */
  Ball ball = (Ball) { .r=(FF_Rect){300, 300, 20, 20}, .vel_x=100, .vel_y=150 };
  Paddle left_paddle = (Paddle){.r = (FF_Rect){40, FF_WindowGetHeight(w) / 2.f - 75, 10, 150}};
  Paddle right_paddle = (Paddle){.r =(FF_Rect){FF_WindowGetWidth(w) - 50, FF_WindowGetHeight(w) / 2.f - 75, 10, 150}};

  FF_Rect test_lerp = (FF_Rect){40, 80, 30, 30};
  int left_score = 0, right_score = 0;
  
  while (!FF_WindowShouldClose(w)) {
    FF_WindowPollEvents(w);
    FF_WindowClearBackgroundEx(w, 0.4f, 0.4f, 0.4f, 1.0f);
    
    run_paddle_human(&right_paddle, "right_paddle_up", "right_paddle_down", FF_WindowGetHeight(w));
    run_paddle_ai(&left_paddle, &ball, FF_WindowGetHeight(w), FF_WindowDeltaTime(w));
    run_ball(&ball, w);

    /*
     * DETECT WHEN BALL HITS A PADDLE
     */
    if (collided(ball.r, left_paddle.r) || collided(ball.r, right_paddle.r)) {
      ball.vel_x *= -1;
      FF_SoundSourcePlay(source, paddle_bounce);
    }

    /*
     * DETECT WHEN BALL HITS LEFT WALL
     */
    if (collided(ball.r, (FF_Rect){1, 0, 1, FF_WindowGetHeight(w)})) {
      FF_SoundSourcePlay(source, death);
      reset_ball(&ball, w);
      right_score++;
      LOG_INFO("Score: %d - %d", left_score, right_score);
    }
    
    /*
     * DETECT WHEN BALL HITS RIGHT WALL
     */
    if (collided(ball.r, (FF_Rect){FF_WindowGetWidth(w) - 1, 0, 1, FF_WindowGetHeight(w)})) {
      FF_SoundSourcePlay(source, death);
      reset_ball(&ball, w);
      left_score++;
      LOG_INFO("Score: %d - %d", left_score, right_score);
    }
    
    /*
     * MANUAL GAME RESET
     */
    if (FF_IsKeyPressed(KEY_R)) {
      reset_ball(&ball, w);
    }
    
    /*
     * SOME INTERPOLATION LOGIC (not all interpolation functions work/are implemented)
     */
    test_lerp.x = FF_LerpFunc(FF_LerpFuncLinear, test_lerp.x, FF_GetMousePositionX() - test_lerp.w/2, 0.1);
    test_lerp.y = FF_LerpFunc(FF_LerpFuncLinear, test_lerp.y, FF_GetMousePositionY() - test_lerp.h/2, 0.1);

    /*
     * GET READY FOR RENDERING
     */
    FF_OrthoCameraUpdate(&c, false);
    FF_BindTexture(t);

    /*
     * START RENDERING
     */
    FF_RendererDrawGeometry(r, quad, c, (vec3){ball.r.x, ball.r.y, 0},             (vec3){ball.r.w, ball.r.h, 1},             (vec3){0}, 0);
    FF_RendererDrawGeometry(r, quad, c, (vec3){left_paddle.r.x, left_paddle.r.y, 0},   (vec3){left_paddle.r.w, left_paddle.r.h, 1},   (vec3){0}, 0);
    FF_RendererDrawGeometry(r, quad, c, (vec3){right_paddle.r.x, right_paddle.r.y, 0}, (vec3){right_paddle.r.w, right_paddle.r.h, 1}, (vec3){0}, 0);
    FF_RendererDrawGeometry(r, quad, c, (vec3){test_lerp.x, test_lerp.y, 0},       (vec3){test_lerp.w, test_lerp.h, 1},       (vec3){0}, 0);
    
    FF_RendererDrawText(r, f, c, (vec3){0, 0, 0}, (vec3){1, 1, 1}, 1, "Hello World");
  }
  
  /*
   * FREE ALLOCATED RESOURCES
   */
  FF_FreeSound(paddle_bounce);
  FF_FreeSound(death);
  FreeGeometry(quad);
  FF_FreeTexture(t);
  
  deinit(w, r, audioSystem, fontLoader);
}
