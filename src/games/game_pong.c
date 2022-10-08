#include <firefly/Core/Core.h>
#include <firefly/IO/IO.h>
#include <firefly/Resource/Resource.h>
#include <firefly/Rendering/Renderer.h>
#include <firefly/Core/OrthoCamera.h>

void setup_keybinds() {
  FF_KeyBindRegister("left", KEY_NONE, KEY_LEFT);
  FF_KeyBindRegister("right", KEY_NONE, KEY_RIGHT);
  FF_KeyBindRegister("up", KEY_NONE, KEY_UP);
  FF_KeyBindRegister("down", KEY_NONE, KEY_DOWN);
}

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

int main() {
  init();
  Window* w = FF_CreateWindowGL("Pong", 600, 600);
  FF_InitRenderer();
  
  Geometry quad = GenerateQuad();
  Camera c = FF_OrthoCamera();
  Texture t = FF_LoadTexture("res/splotch.png");
  
  int left_y = 0;
  int right_y = 0;
  
  while (!FF_WindowShouldClose(w)) {
    FF_WindowPollEvents(w);
    FF_WindowClearBackgroundEx(w, 0.4f, 0.4f, 0.4f, 1.0f);
    
    FF_OrthoCameraUpdate(&c, false);
    
    if (FF_IsKeyDown(KEY_S))
      left_y--;
    if (FF_IsKeyDown(KEY_W))
      left_y++;
    if (FF_IsKeyDown(KEY_DOWN))
      right_y--;
    if (FF_IsKeyDown(KEY_UP))
      right_y++;
    
    // draw left paddle
    FF_BindTexture(t);
    FF_RendererDrawGeometryEx(quad, c, (vec3){-7, left_y, 0}, (vec3){1, 8, 1}, (vec3){0, 0, 0}, 0);
    FF_RendererDrawGeometryEx(quad, c, (vec3){7, right_y, 0}, (vec3){1, 8, 1}, (vec3){0, 0, 0}, 0);
    // draw right paddle
  }
  
  FreeGeometry(quad);
  FF_FreeTexture(t);
  
  deinit(w);
}