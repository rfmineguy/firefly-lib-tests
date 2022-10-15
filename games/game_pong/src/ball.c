#include "ball.h"
#include <firefly/Core/Random.h>

void run_ball(Ball *ball, FF_Window *w) {
  if (ball->r.x < 0 || ball->r.x + ball->r.w > FF_WindowGetWidth(w)) {
    ball->vel_x *= -1;
  }
  if (ball->r.y < 0 || ball->r.y > FF_WindowGetHeight(w)) {
    ball->vel_y *= -1;
  }
  ball->r.x += ball->vel_x * FF_WindowDeltaTime(w);
  ball->r.y += ball->vel_y * FF_WindowDeltaTime(w);
}

void reset_ball(Ball* ball, FF_Window* w) {
  ball->vel_x = (100 + FF_RandomRangeFloat(-20, 20)) * FF_RandomFromSet(2, (double[]){-1, 1});
  ball->vel_y = (150 + FF_RandomRangeFloat(-20, 20)) * FF_RandomFromSet(2, (double[]){-1, 1});
  ball->r.x = FF_WindowGetWidth(w) / 2.f;
  ball->r.y = FF_WindowGetHeight(w) / 2.f;
}
