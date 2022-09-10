#include "firefly/Core/Window.h"
#include "firefly/IO/Log.h"
#include "firefly/Resource/Shader.h"
#include <firefly/Resource/ResourceManager.h>
#include <firefly/Resource/Resource.h>
#include <firefly/Core/Core.h>
#include <firefly/Rendering/Rendering.h>
#include <firefly/IO/IO.h>

void load_resource_files();

int main() {
    InitWindowGL();
    SetLogStream(stdout);
    InitResourcePool();
    load_resource_files();

    Shader* s = GetShaderResource("test_shader");

    LOG_DEBUG("Shader: %p", s);
    while (!WindowShouldClose()) {
        WindowPollEvents();
        WindowClearBackground();
    }

    DestroyResourcePool();
    DestroyWindowGL();
}

void load_resource_files() {
    PutShaderResource(LoadShader("res/shader/", "test"), "test_shader");
}
