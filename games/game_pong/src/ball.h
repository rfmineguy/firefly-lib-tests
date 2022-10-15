#ifndef BALL_H
#define BALL_H
#include <firefly/Rendering/Rect.h>
#include <firefly/Core/Window.h>

typedef struct _Ball {
  FF_Rect r;
  int vel_x, vel_y;
} Ball;

void run_ball(Ball*, FF_Window*);
void reset_ball(Ball*, FF_Window*);

#endif