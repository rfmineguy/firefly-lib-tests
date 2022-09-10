#include "firefly/Core/Vec.h"
#include "firefly/Core/Window.h"
#include "firefly/IO/Log.h"
#include "firefly/Resource/Shader.h"
#include <firefly/Core/Core.h>
#include <firefly/IO/IO.h>
#include <firefly/Rendering/Rendering.h>
#include <firefly/Core/Camera.h>
#include <firefly/Resource/Resource.h>
#include <firefly/Resource/Sound.h>
#include <unistd.h>
#include <math.h>

int main() {
    InitWindowGL();
    SetLogStream(stdout);
    InitRendering();
    InitSoundMaster();
    Texture* t = LoadTexture("res/splotch.png");
    Sound* s = LoadSound("res/PinkPanther30.wav");
    SoundSource* source = SoundSourceCreate();
    SoundSource* source2 = SoundSourceCreate();
    SoundSourceFull(source, 1.1f, 1.0f, (vec3){0, 0, 1}, true);
    SoundSourceFull(source2, 1.0f, 2.f, (vec3){0, 1, 0}, false);
    SoundSourcePlay(source, s);
    SoundSourcePlay(source2, s);
    
    Camera c;
    Timer timer1, timer2;
    InitCameraIso(&c);
    float rotation = 0;
    int x = 0, y = 0;
    
    while (!WindowShouldClose()) {
        WindowPollEvents();
        WindowClearBackground();
        if (WasWindowResized()) {
            UpdateProjectionCamera(&c, WindowGetWidth(), WindowGetHeight());
        }
        if (IsKeyPressed(KEY_SPACE)) {
            ToggleCursorLocked();
            LOG_INFO("Cursor Locked: %d", IsCursorLocked());
        }
        
        UpdateCamera(&c);
        SoundMasterSetListener(c.camPos, (vec3){0, 0, 0});
        
        TimerStartIntervalEx(&timer1, 5, SECOND);
        if (TimerElapsed(&timer1)) {
            LOG_INFO("Timer elapsed, %0.4f", WindowGetTime());
            ResetTimer(&timer1);
        }

        if (IsKeyPressed(KEY_P)) {
            if (SoundSourcePlaying(source)) {
                SoundSourcePause(source);
            }
            else {
                SoundSourceUnpause(source);
            }
        }
        
        BindTexture(t);
        if (IsKeyDown(KEY_LEFT))
            rotation += WindowDeltaTime() * 1000;
        if (IsKeyDown(KEY_RIGHT))
            rotation -= WindowDeltaTime() * 1000;

        for (int i = -3; i < 3; i++) {
            for (int j = -3; j < 3; j++) {
                DrawCubeEx((vec3){i * 5, j * 5, 0}, (vec3){2, 2, 2}, (vec3){1, 0, 1}, rotation, &c);
            }
        }
        
        x = cos(WindowGetTime()) * 10;
        y = sin(WindowGetTime()) * 10;
        SoundSourceSetPos(source, (vec3){x, y, 2});
    }

    SoundSourceDestroy(source);
    SoundSourceDestroy(source2);    
    FreeSound(s);
    FreeTexture(t);

    FreeRendering();
    DeinitSoundMaster();
    DestroyWindowGL();
    LOG_WARN("Program ending. Successful run");
}
