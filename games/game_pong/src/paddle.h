#ifndef PADDLE_H
#define PADDLE_H
#include <firefly/Rendering/Rect.h>
#include "ball.h"

typedef struct {
  FF_Rect r;  
} Paddle;

void run_paddle_ai(Paddle*, Ball*, int, double dt);
void run_paddle_human(Paddle*, const char*, const char*, int h);

#endif