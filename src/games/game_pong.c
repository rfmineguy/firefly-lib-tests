#include <firefly/Core/Core.h>
#include <firefly/IO/IO.h>
#include <firefly/Resource/Resource.h>
#include <firefly/Rendering/Renderer.h>

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
  
  deinit(w);
}