#include <firefly/Core/Core.h>
#include <firefly/Core/Window.h>
#include <firefly/IO/Log.h>
#include <firefly/Resource/Sound.h>
#include <unistd.h>
#include <math.h>

void setup_keybinds() {
  FF_KeyBindRegister("left", KEY_A, KEY_LEFT);
}

int main() {
  SetLogStream(stdout);
  FF_InitKeybindHT();
  FF_InitAudioSystem();      //SoundMaster* master = FF_int_GetSoundMaster();
  FF_SoundListener((vec3){0, 0, 0}, (vec3){0, 0, 0});
  
  FF_RandomSeedTime();
  setup_keybinds();
  Window* w = FF_CreateWindowGL("Window", 600, 600);
  Timer t = FF_Timer(1, SECOND);
  Sound s = FF_LoadSound("res/PinkPanther30.wav");
  SoundSource source = FF_SoundSource();
  
  float r = 0, g = 0, b = 0;
  while (!FF_WindowShouldClose(w)) {
    r = sin(FF_GetTime());
    g = cos(FF_GetTime());
    b = sin(FF_GetTime()) * cos(FF_GetTime());
    
    FF_WindowPollEvents(w);
    FF_WindowClearBackgroundEx(w, r, g, b, 1.0f);
    
    if (FF_TimerElapsed(&t)) {
      LOG_DEBUG("Delta time : %0.4f", FF_WindowDeltaTime(w));
      LOG_DEBUG("Random number [0, 15] -> %d", FF_RandomRangeInt(0, 100));
    }
    
    if (FF_IsKeyPressed(KEY_L)) {
      LOG_DEBUG("Pressed KEY_L");
      FF_SoundSourcePlay(source, s);
      if (FF_SoundSourcePlaying(source)) {
        FF_SoundSourcePlay(source, s);
      }
      else {
        FF_SoundSourcePause(source);
      }
    }
    
    if (FF_IsKeyBindPressed("left")) {
      LOG_DEBUG("Pressed KEY_B");
    }
    /*    FUTURE (Not needed now)
     *   FF_Batch b;
     *   FF_BeginBatch(b);
     *   
     *   FF_DrawCube();
     *   
     *   FF_EndBatch(b);
     *   FF_RenderBatch(b);
     */
    
    //FF_StartRenderer();
    //FF_DrawQuad();
    //FF_EndRenderer();
    
    //FF_BeginScene(cam);      //batch subsequent draw commands
    
    //FF_DrawQuad(x, 0, 0, 1, 1, 1, FF_Rad(45.0f));
    //FF_DrawCube((vec3){5, 5, 4}, (vec3){3, 3, 3}, (vec3){0, 1, 0}, PI/4);
    //FF_DrawTexturedQuad(tex, (vec3){5, 3, 3}, (vec3){1, 1, 1}, (vec3)){1, 0, 1}, 0);
    
    //FF_EndScene();           //render batched data
    
    //FF_BeginUI(cam);         //batch subsequent draw commands
    
    //FF_EndUI(cam);           //render batched data
  }

  FF_FreeSound(s);
  FF_DeinitAudioSystem();
  FF_DeinitKeybindHT();
  FF_DestroyWindowGL(w);
}