#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "game.h"
#include "key.h"

typedef struct {
  bool pressing[KEY_COUNT];
  bool dispatched[KEY_COUNT];

  float as_left_delay;
  float as_right_delay;
} Controller;

void ControllerUpdateGame(Controller*, Game*, float);

#endif
