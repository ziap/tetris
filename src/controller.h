#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "game.h"
#include "key.h"

typedef enum {
  LEFT,
  RIGHT,
} Direction;

typedef struct {
  bool pressing[KEY_COUNT];
  bool dispatched[KEY_COUNT];

  double as_left_delay;
  double as_right_delay;

  Direction dir;
} Controller;

void ControllerUpdateGame(Controller*, Game*, double);

#endif
