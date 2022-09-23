typedef struct {
  FFTexture* texture;
  FFSound* player_jump;
  float x, y;
} player;

typedef struct {
  player player;
  FFTimer timer;
} GameState;

void game_logic(GameState*);
void draw_scene(GameState*);
void draw_ui(GameState*);

FF_Context lib_ctx;
int main() {
  lib_ctx = FF_CreateContext();
  FF_Window ff_window = FF_CreateWindow("OpenGL", 60, 600, &lib_ctx);
  GameState state;
  FFTexture *t = LoadResource("res/player.png");

  state.player = (player){.texture=FF_LoadTexture("res/player.png"), .player_jump=FF_LoadSound("res/player_jump.wav"), .x=0, .y=0};
  state.timer = FF_Timer(SECONDS, 1);
    
  while (!FF_WindowShouldClose(ff_window)) {
    game_logic(&state);
    draw_scene(&state);
    draw_ui(&state);
  }
  
  FF_FreeTexture(state.player.texture);
  FF_FreeSound(state.player.player_jump);
  FF_FreeContext(lib_ctx);
}

void game_logic(GameState* state) {
  if (FF_KeyDown(KEY_A)) {
    state->player.x--;
  }
  if (FF_KeyDown(KEY_D)) {
    state->player.x++;
  }
  if (FF_KeyDown(KEY_W)) {
    state->player.y--;
  }
  if (FF_KeyDown(KEY_S)) {
    state->player.y++;
  }
}

void draw_scene(GameState* state) {
  FF_BeginScene(ORTHOGRAPHIC);
  FF_DrawTexture(state->player.texture, x, y);
  FF_EndScene();
}

void draw_ui(GameState* state) {
  FF_BeginUI();
  FF_DrawText("Hello World", 0, 0);
  FF_EndUI();
}