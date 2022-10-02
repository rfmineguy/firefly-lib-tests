#include <firefly/Core/Core.h>
#include <firefly/Core/Window.h>
#include <firefly/IO/Log.h>
#include <firefly/Resource/Sound.h>
#include <firefly/Rendering/Renderer.h>
#include <unistd.h>
#include <math.h>

#include <firefly/Core/PerspectiveCamera.h>
#include <firefly/Core/Camera.h>

void setup_keybinds() {
  FF_KeyBindRegister("left", KEY_A, KEY_LEFT);
}

int main() {
  SetLogStream(stdout);
  FF_InitKeybindHT();
  FF_InitAudioSystem();
  FF_SoundListener((vec3){0, 0, 0}, (vec3){0, 0, 0});
  
  FF_RandomSeedTime();
  setup_keybinds();

  Window* w = FF_CreateWindowGL("Window", 600, 600);
  FF_InitRenderer();

  Timer t = FF_Timer(1, SECOND);
  Sound s = FF_LoadSound("res/PinkPanther30.wav");
  SoundSource source = FF_SoundSource();
  Geometry quad = GenerateQuad();
  Camera c = FF_PerspectiveCamera();
  
  float r = 0, g = 0, b = 0;
  while (!FF_WindowShouldClose(w)) {
    FF_WindowPollEvents(w);
    FF_WindowClearBackgroundEx(w, 0.4f, 0.4f, 0.4f, 1.0f);

    FF_PerspectiveCameraUpdate(c);
    
    if (FF_TimerElapsed(&t)) {
      LOG_DEBUG("Delta time : %0.4f", FF_WindowDeltaTime(w));
      LOG_DEBUG("Random number [0, 15] -> %d", FF_RandomRangeInt(0, 100));
    }
    
    if (FF_IsKeyPressed(KEY_SPACE)) {
      FF_ToggleCursorLocked(w);
    }
    
    if (FF_IsKeyPressed(KEY_L)) {
      LOG_DEBUG("Pressed KEY_L");
      if (FF_SoundSourcePlaying(source)) {
        FF_SoundSourcePlay(source, s);
      }
      else {
        FF_SoundSourcePause(source);
      }
    }
    
    int x = 0, y = 0, z = 0;
    FF_RendererDrawGeometryEx(quad, c, (vec3){x, y, z},(vec3){1, 1, 1}, (vec3){0, 0, 1}, 0);
  }

  FreeGeometry(quad);
  FF_FreeSound(s);
  FF_DeinitRenderer();
  FF_DeinitAudioSystem();
  FF_DeinitKeybindHT();
  FF_DestroyWindowGL(w);
}