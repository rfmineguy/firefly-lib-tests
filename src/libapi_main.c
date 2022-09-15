void setup_keybinds();
void setup_window_settings();

int main() {
  /*
    If the user decides to try to use a function from one of these
      features without initializing it, then ignore the request and supply
      a message to notify them of the problem
  */
  InitializeFeatures(AUDIO | WINDOW | INPUT | RENDERING); //ALL

  setup_keybinds();
  setup_window_settings();
   
  Sound *pink_panther_sound = LoadSound("res/PinkPanther30.wav");

  while (!WindowShouldClose()) {
    if (IsKeyBindPressed("move_forward")) {
      LOG_INFO("Move forward pressed");
    }
    if (IsKeyBindPressed("toggle_sound")) {
      if (!PlayingSound(pink_panther_sound)) {
        PlaySound(pink_panther_sound, (vec3){0, 0, 0}, 0.8f, 1.0f);
      }
    }
    
    //RenderSceneBegin(THREE_DIMENSION);
    RenderSceneBegin(TWO_DIMENSION);  //should i infer orthographic projection here
    DrawCubeEx((vec3){3, 3, 0}, (vec3){1, 1, 1}, (vec3){1, 0, 0}, 60);
    RenderSceneEnd();
    
    RenderUIBegin();
    DrawText("Hello World", (vec2){0, 0}, (vec2){100, 10});
    if (DrawButton("Click me")) {
      LOG_INFO("Pressed button");
    }
    RenderUIEnd();
  }
  
  DeInitializeFeatures();
}

void setup_keybinds() {
  KeyBindRegister("move_forward", KEY_LEFT_CONTROL | KEY_W, KEY_UP);    //this may not work the way i want it to
  KeyBindRegister("move_backward", KEY_S, KEY_DOWN);
  KeyBindRegister("move_right", KEY_D, KEY_RIGHT);
  KeyBindRegister("move_forward", KEY_A, KEY_LEFT);
  KeyBindRegister("toggle_sound", KEY_P, KEY_NONE);
}

void setup_window_settings() {
  SetLogStream(stdout);
  WindowSetClearColor(0.5f, 0.68f, 1.0f);
  WindowInitCamera(PERSPECTIVE);
  WindowSetCloseKey(KEY_ESCAPE);
}