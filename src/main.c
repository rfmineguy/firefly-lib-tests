#include "firefly/Core/Input.h"
#include "firefly/Core/Window.h"
#include "firefly/IO/Log.h"
#include <firefly/Core/Core.h>
#include <firefly/IO/IO.h>
#include <firefly/Resource/Sound.h>
#include <firefly/Resource/Texture.h>
#include <stdbool.h>
#include <stdlib.h>

#define KEY_NONE -1
#define KEY_ESC 100
#define KEY_M 101
#define KEY_W 102
#define KEY_S 103

void register_keybinds();

int main() {
    FILE* f = SetReadStream(FileOpen("../res/text.txt", READ));
    char* contents = Read(NULL);
    FileClose(f);
    SetLogStream(stderr);
    LOG_INFO("Hello World %p", f);

    InitWindowAPI(OPENGL);
    WindowSetVSync();
    

    while (!WindowShouldClose()) {
        WindowPollEvents();
        if (IsKeyDown(KEY_ESC)) {
            SetWindowShouldClose(true);
        }
        if (IsKeyDown(KEY_M)) {
            if (IsShiftDown()) {
                WindowMinimize();
            }
            else {
                WindowToggleFullscreen();
            }
        }
    }

    DestroyWindowAPI(OPENGL);

    SetLogStream(stdout);
    free(contents);
    LOG_WARN("Program exiting");
}

void register_keybinds() {
    KeyBindRegister("forward", KEY_W, KEY_NONE);
}
