#include <firefly/Core/Core.h>
#include <firefly/Core/Window.h>
#include <firefly/IO/Log.h>
#include <firefly/Resource/Sound.h>
#include <firefly/Rendering/Renderer.h>
#include <unistd.h>
#include <math.h>

#include <firefly/Core/OrthoCamera.h>
#include <firefly/Core/Camera.h>
#include <firefly/Resource/Texture.h>
#include <firefly/Resource/ResourceManager.h>

const char* SPLOTCH = "splotch";

void setup_keybinds() {
  FF_KeyBindRegister("left", KEY_NONE, KEY_LEFT);
  FF_KeyBindRegister("right", KEY_NONE, KEY_RIGHT);
  FF_KeyBindRegister("up", KEY_NONE, KEY_UP);
  FF_KeyBindRegister("down", KEY_NONE, KEY_DOWN);
}

void load_resources() {
  // Resource manager needs some work. it doesn't work in its current state
  //FF_PutResource(FF_TEXTURE, FF_LoadTexture("res/splotch.png"), SPLOTCH);
}

void init() {
  SetLogStream(stdout);
  FF_InitKeybindHT();
  FF_InitAudioSystem();
  FF_RandomSeedTime();
  FF_SoundListener((vec3){0, 0, 0}, (vec3){0, 0, 0});

  setup_keybinds();
  load_resources();
}

void deinit(Window* w) {
  FF_DeinitRenderer();
  FF_DeinitAudioSystem();
  FF_DeinitKeybindHT();
  FF_DestroyWindowGL(w);
}

int main() {
  // Initialize the parts of FFLib
  init();

  // Create the window using the opengl api (currently the only api)
  Window* w = FF_CreateWindowGL("Window", 600, 600);
  
  // Create the renderer with the opengl api
  FF_InitRenderer();

  // Load resources
  Timer t = FF_Timer(1, SECOND);
  Sound s = FF_LoadSound("res/PinkPanther30.wav");
  Texture tex = FF_LoadTexture("res/splotch.png");

  // Game world data
  SoundSource source = FF_SoundSource();
  Geometry quad = GenerateQuad();
  Geometry cube = GenerateCube();
  Camera c = FF_OrthoCamera();
  
  // Game loop
  float r = 0, g = 0, b = 0;
  while (!FF_WindowShouldClose(w)) {
    FF_WindowPollEvents(w);
    FF_WindowClearBackgroundEx(w, 0.4f, 0.4f, 0.4f, 1.0f);

    FF_OrthoCameraUpdate(&c);
    
    if (FF_IsKeyPressed(KEY_SPACE)) {
      FF_ToggleCursorLocked(w);
    }
    
    if (FF_IsKeyPressed(KEY_L)) {
      LOG_DEBUG("Sound playing: %d", FF_SoundSourcePlaying(source));
      if (!FF_SoundSourcePlaying(source)) {
        FF_SoundSourcePlay(source, s);
      }
      else {
        FF_SoundSourcePause(source);
      }
    }
    
    FF_BindTexture(tex);
    float scale = fabs(sin(FF_GetTime()) + 1);
    float rotation = tan(FF_GetTime()) * 100;

    int x = 0, y = 0, z = -5;
    for (int i = 0; i < 5; i++) {
      for (int j = 0; j < 5; j++) {
        FF_RendererDrawGeometryEx(cube, c, (vec3){i * 3, j * 3, z},(vec3){scale, scale, scale}, (vec3){0, 0, 1}, rotation);
      }
    }
  }

  FreeGeometry(quad);
  FreeGeometry(cube);
  FF_FreeSound(s);
  FF_FreeTexture(tex);

  deinit(w);
}