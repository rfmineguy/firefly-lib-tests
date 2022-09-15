#include "firefly/Core/Vec.h"
#include "firefly/Core/Window.h"
#include <firefly/Core/Core.h>
#include <firefly/Core/Camera.h>
#include <firefly/Core/Init.h>
#include <firefly/IO/IO.h>
#include "firefly/IO/Log.h"
#include <firefly/Rendering/Rendering.h>
#include "firefly/Resource/Shader.h"
#include <firefly/Resource/Resource.h>
#include <firefly/Resource/Sound.h>
#include <unistd.h>
#include <math.h>

#define CAN_DROP_WAV "can_drop_wav"
#define PINK_PANTHER_WAV "pink_panther_wav"

void setup_keybinds() {
    KeyBindRegister("toggle_panther_sound", KEY_P, KEY_NONE);
    KeyBindRegister("play_can_drop", KEY_C, KEY_B);
}

void load_resources() {
    PutSoundResource(LoadSound("res/can_drop.wav"), CAN_DROP_WAV);
    PutSoundResource(LoadSound("res/PinkPanther30.wav"), PINK_PANTHER_WAV);
    PutTextureResource(LoadTexture("res/splotch.png"), "splotch_tex");
    PutTextureResource(LoadTexture("res/weird_green_circle_tex.png"), "weird_circle");
}

int main() {
    InitializeFeatures(ALL);
    setup_keybinds();
    load_resources();
    
    SoundSource* pink_panther_source = SoundSourceCreateEx(1.1f, 1.0f, (vec3){0, 0, 1}, true);
    SoundSource* can_drop_source = SoundSourceCreateEx(0.9f, 1.0f, (vec3){0, 0, 0}, false);
    SoundSourcePlay(pink_panther_source, GetSoundResource(PINK_PANTHER_WAV));
    
    Camera c;
    Timer timer1, timer2;
    InitCamera(&c, PERSPECTIVE);
    
    float rotation = 0, scale = 1;
    
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
        
        TimerStartIntervalEx(&timer1, 1, SECOND);
        if (IsKeyBindPressed("toggle_panther_sound")) {
            LOG_DEBUG("%p", GetSoundResource(PINK_PANTHER_WAV));
            if (SoundSourcePlaying(pink_panther_source)) {
                SoundSourcePause(pink_panther_source);
            }
            else {
                SoundSourceUnpause(pink_panther_source);
            }
        }
        if (IsKeyBindPressed("play_can_drop")) {
            Sound* s = GetSoundResource(CAN_DROP_WAV);
            LOG_DEBUG("%p", s);
            SoundSourcePlay(can_drop_source, GetSoundResource(CAN_DROP_WAV));
        }
        if (IsKeyBindDown("play_can_drop")) {
            LOG_INFO("\"play_can_drop\" keybind");
        }
        
        BindTexture(GetTextureResource("weird_circle"));
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
                DrawCubeEx((vec3){i * 5 + cos(WindowGetTime()) * 2, j * 5 + sin(WindowGetTime()) * 2, 0}, (vec3){scale, scale, scale}, (vec3){1, 0, 1}, rotation, &c);
            }
        }
        
        SoundSourceSetPos(pink_panther_source, (vec3){0, 0, 2});
    }

    SoundSourceDestroy(pink_panther_source);
    SoundSourceDestroy(can_drop_source);
    
    DeInitializeFeatures();
    LOG_WARN("Program ending. Successful run");
}
