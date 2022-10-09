#include <firefly/Core/Core.h>
#include <firefly/IO/IO.h>
#include <firefly/Resource/Resource.h>
#include <firefly/Rendering/Renderer.h>
#include <firefly/Core/OrthoCamera.h>

typedef struct {
  float x, y, w, h;
} Rect;

typedef struct {
  vec3 pos;
  vec3 scale;
  vec3 rot_axis;
  float rotation;
} Transform;

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
  //Material m = FF_Material();
  //m.albedo = (Color){255, 100, 150};
  
  int ball_x = 300, ball_y = 300;
  
  while (!FF_WindowShouldClose(w)) {
    FF_WindowPollEvents(w);
    FF_WindowClearBackgroundEx(w, 0.4f, 0.4f, 0.4f, 1.0f);
    if (FF_WasWindowResized(w)) {
      FF_OrthoCameraUpdateProj(&c, FF_WindowGetWidth(w), FF_WindowGetHeight(w));
    }
    
    FF_OrthoCameraUpdate(&c, true);
    
    FF_BindTexture(t);
    
    // draw ball
    FF_RendererDrawGeometryEx(quad, c, (vec3){5, 7, 0}, (vec3){2, 1, 1}, (vec3){0, 0, 0}, 0);
  }
  
  FreeGeometry(quad);
  FF_FreeTexture(t);
  
  deinit(w);
}