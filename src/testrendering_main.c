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
    InitKeybindHT();

    Texture* t = LoadTexture("res/splotch.png");
    Sound* pink_panther_sound = LoadSound("res/PinkPanther30.wav");
    Sound* can_drop_sound = LoadSound("res/can_drop.wav");
    SoundSource* pink_panther_source = SoundSourceCreate();
    SoundSource* can_drop_source = SoundSourceCreate();
    SoundSourceFull(pink_panther_source, 1.1f, 1.0f, (vec3){0, 0, 1}, true);
    SoundSourceFull(can_drop_source, 0.9f, 1.0f, (vec3){0, 0, 0}, false);
    
    SoundSourcePlay(pink_panther_source, pink_panther_sound);
    
    KeyBindRegister("toggle_panther_sound", KEY_P, KEY_NONE);
    KeyBindRegister("play_can_drop", KEY_C, KEY_B);
    
    Camera c;
    Timer timer1, timer2;
    InitCamera(&c, PERSPECTIVE);
    float rotation = 0, scale = 1;
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

        if (IsKeyBindPressed("toggle_panther_sound")) {
            if (SoundSourcePlaying(pink_panther_source)) {
                SoundSourcePause(pink_panther_source);
            }
            else {
                SoundSourceUnpause(pink_panther_source);
            }
        }
        if (IsKeyBindPressed("play_can_drop")) {
            SoundSourcePlay(can_drop_source, can_drop_sound);
        }
        
        BindTexture(t);
        if (IsKeyDown(KEY_LEFT))
            rotation += WindowDeltaTime() * 1000;
        if (IsKeyDown(KEY_RIGHT))
            rotation -= WindowDeltaTime() * 1000;
        if (IsKeyDown(KEY_UP))
            scale += WindowDeltaTime() * 1;
        if (IsKeyDown(KEY_DOWN))
            scale -= WindowDeltaTime() * 1;

        for (int i = -3; i < 3; i++) {
            for (int j = -3; j < 3; j++) {
                DrawCubeEx((vec3){i * 5, j * 5, 0}, (vec3){scale, scale, scale}, (vec3){1, 0, 1}, rotation, &c);
            }
        }
        
        x = cos(WindowGetTime()) * 10;
        y = sin(WindowGetTime()) * 10;
        SoundSourceSetPos(pink_panther_source, (vec3){x, y, 2});
    }

    SoundSourceDestroy(pink_panther_source);
    SoundSourceDestroy(can_drop_source);
    FreeSound(pink_panther_sound);
    FreeSound(can_drop_sound);
    FreeTexture(t);
    
    DeinitKeybindHT();
    FreeRendering();
    DeinitSoundMaster();
    DestroyWindowGL();
    LOG_WARN("Program ending. Successful run");
}
