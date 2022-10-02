typedef enum {
  FF_SUCCESS = 0, FF_GLFW, FF_WINDOW_CREATE_FAIL, FF_GLAD_FAIL,
  FF_TEXTURE_LOAD_FAIL, FF_SHADER_LOAD
} FF_Error;

void setup_ff_lib() {
  FFInit(FF_ALL);
  if (FFError() != FF_SUCCESS) {
    LOG_CRITICAL("Failed to initialize FFLib, %s", FFGetErrorStr());
    exit(1);
  }
}

FFWindow* setup_window() {
  FFWindow *w = FFCreateWindow("Game", 600, 600);
  if (FFError() != FF_SUCCESS) {
    LOG_CRITICAL("Failed to create FFWindow, %s", FFGetErrorStr());
    FFDestroy();
    exit(1);
  }
  return w;
}

void setup_keybinds() {
  FFKeyBindRegister("move_right", KEY_D, KEY_RIGHT);
  FFKeyBindRegister("move_left", KEY_A, KEY_LEFT);
  FFKeyBindRegister("move_up", KEY_W, KEY_UP);
  FFKeyBindRegister("move_down", KEY_S, KEY_DOWN);
}

FFState* INT_FFGetState();

#define TILEMAP_RESOURCE_POOL "new_resource_pool"
void setup_resources() {
  FFMakeResourcePool(TILEMAP_RESOURCE_POOL);
  FFPut(TILEMAP_RESOURCE_POOL, LoadTileMap("../some/path/"), "some name");
  Resource* r = (Resource*) FFGet(NEW_RESOURCE_POOL, "some name");

  FFPutTexture(FFLoadTexture("../res/textures/player.png"), "player");
  FFPutTexture(FFLoadTexture("../res/textures/ball.png"), "ball");
  FFPutShader(FFLoadShader("../res/shaders/camera/", "camera"), "camera");
  FFPutSound(FFLoadSound("../res/sounds/steps.wav"), "steps");
  
  FFState* state = INT_FFGetState();    //gives the user the global state of the library for technical users
  state->windows[0].width;
  state->windows[0].height;
  state->windows[0].keys[0];
  state->windows[0].buttons[0];
  state->windows[0].api;
  state->loaded_shaders;
  state->loaded_textures;
  state->loaded_sounds;
  state->loaded_geometry;
  state->loaded_keybinds;
  state->error;
}

typedef struct {
  float x, y;
  FFTexture* texture;
  FFSound* walk;
} player_t;

typedef struct {
  float x, y;
} enemy_t;

int main() {
  // SETUP
  setup_ff_lib();
  FFWindow *w = setup_window();
  setup_keybinds();
  setup_resources();
  
  player_t player = (player_t) {.x=0, .y=0, .texture=FFGetTexture("player"), .walk=FFGetSound("steps")};

  // GAME LOOP
  while (!FFWindowShouldClose(w)) {
    Sound* s = FFGetSound("steps");
    if (FFIsKeyPressed(KEY_A, w)) {
      if (!FFIsSoundPlaying(ss_1))
        FFPlaySound(s, ss_1);
      else
        FFStopSound(s, ss_1);
    }
    if (FFIsKeyBindDown("player_walk_left")) {
      player.x --;
    }
    if (FFIsKeyBindDown("player_walk_right")) {
      player.x ++;
    }
    if (FFIsKeyBindDown("player_walk_up")) {
      player.y --;
    }
    if (FFIsKeyBindDown("player_walk_down")) {
      player.y ++;
    }

    FFBegin(TWO_DIM);  //for now dont worry about batching
    FFDrawTexturedQuad(player.x, player.y, player.texture);
    FFEnd();
    
    FFUIBegin();    //for now dont worry about batching
    FFDrawText("Hello World");
    FFDrawButton("Button", 10, 10, 100, 50);
    FFEnd();
  }
  
  // TEARDOWN
  FFDestroy();
}