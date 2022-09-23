void setup_keybinds(FFLibContext ctx) {
  FFKeyBindRegister("move_right", KEY_D, KEY_RIGHT, ctx);
  FFKeyBindRegister("move_left", KEY_A, KEY_LEFT, ctx);
}

void setup_resources(FFLibContex ctx) {
  FFPutTextureResource(FFLoadTexture("res/tex.png"), "tex", ctx);
}

int main() {
  FFLibContext ctx;
  FFInitContext(ctx);
  FFWindow *w = FFCreateWindow(ctx);
  setup_keybinds(ctx);
  setup_resources(ctx);
  
  while (!FFWindowShouldClose(ctx)) {
    if (FFIsKeyPressed(KEY_A)) {
      FFPlaySound(FFGetTextureResource("tex", ctx));
    }
  }
  
  FFDeinitContext(&ctx);
}