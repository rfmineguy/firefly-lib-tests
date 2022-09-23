void setup_keybinds();
void setup_window_settings();

F_Contex ctx;
int main() {
  if (F_Init(&ctx) != F_Success) {
    F_Shutdown();
  }
  
  F_Window w = F_CreateWindow("Window", 600, 600);
  while (!F_WindowShouldClose(w)) {
    if (F_KeyPressed(KEY_A)) {
      LOG_INFO("A");
    }
    if (F_KeyPressed(KEY_ESCAPE)) {
      F_SetWindowShouldClose(true);
    }
  }
  
  F_Shutdown(&ctx);
}

void setup_keybinds() {
  FF_KeyBindRegister("move_forward", KEY_LEFT_CONTROL, KEY_UP);    //this may not work the way i want it to
  FF_KeyBindRegister("move_backward", KEY_S, KEY_DOWN);
  FF_KeyBindRegister("move_right", KEY_D, KEY_RIGHT);
  FF_KeyBindRegister("move_forward", KEY_A, KEY_LEFT);
  FF_KeyBindRegister("toggle_sound", KEY_P, KEY_NONE);
}

void setup_window_settings() {
  SetLogStream(stdout);
  WindowSetClearColor(0.5f, 0.68f, 1.0f);
  WindowInitCamera(PERSPECTIVE);
  WindowSetCloseKey(KEY_ESCAPE);
}