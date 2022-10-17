#include "paddle.h"
#include <firefly/Core/Interpolation.h>
#include <firefly/Core/Input.h>

void run_paddle_ai(Paddle *paddle, Ball *ball, int h, double dt) {
  paddle->r.y = FF_Lerp(paddle->r.y, ball->r.y, dt * 10);  
  if (paddle->r.y < 0) {
    paddle->r.y = 0;
  }
  if (paddle->r.y + paddle->r.h >= h) {
    paddle->r.y = h - paddle->r.h;
  }
}

void run_paddle_human(Paddle* paddle, const char* up, const char* down, int h) {
  if (FF_IsKeyBindDown(up)) {
    paddle->r.y = FF_Lerp(paddle->r.y, paddle->r.y - 10, 0.4);
  }
  if (FF_IsKeyBindDown(down)) {
    paddle->r.y = FF_Lerp(paddle->r.y, paddle->r.y + 10, 0.4);
  }
  if (paddle->r.y < 0)
    paddle->r.y = 0;
  if (paddle->r.y + paddle->r.h > h)
    paddle->r.y = h - paddle->r.h;
}
