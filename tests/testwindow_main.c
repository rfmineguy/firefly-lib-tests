#include "firefly/Core/Vec.h"
#include "firefly/IO/Log.h"
#include "firefly/Resource/Shader.h"
#include <firefly/Core/Core.h>
#include <firefly/IO/IO.h>
#include <firefly/Rendering/Rendering.h>
#include <unistd.h>
#include <stdarg.h>

int main() {
    InitWindowGL();
    
    SetLogStream(stdout);
    
    Shader* s = LoadShader("res/shader/", "test");
    while (!WindowShouldClose()) {
        WindowPollEvents();
        WindowClearBackground();
        DrawQuad((Vec2f){0, 0}, (Vec2f){50, 50});
        if (IsControlDown()) {
            LOG_DEBUG("Control down");
        }
        if (IsKeyDown(KEY_A)) {
            LOG_DEBUG("Pressed 'A'");
        }
        if (IsMouseButtonPressed(0)) {
            LOG_DEBUG("Press Left Mouse Button");
        }
        usleep(60/1000000);
    }
    FreeShader(s);
    DestroyWindowGL();
    LOG_WARN("Program ending. Successful run");
}
