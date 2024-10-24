#include "controller.h"

#define DAS 0.17
#define ARR 0.03

void ControllerUpdateGame(Controller* c, Game* game, double dt) {
  (void)game;

  bool dispatching[KEY_COUNT] = {0};

  for (Key i = 0; i < KEY_COUNT; ++i) {
    if (c->pressing[i]) {
      if (!c->dispatched[i]) {
        dispatching[i] = true;
        c->dispatched[i] = true;
      }
    } else {
      c->dispatched[i] = false;
    }
  }

  if (dispatching[KEY_RESTART]) {
    GameInit(game);
    return;
  }

  if (dispatching[KEY_DROP_HARD]) GameDropHard(game);
  if (dispatching[KEY_ROTATE_CW]) GameRotate(game, true);
  if (dispatching[KEY_ROTATE_CCW]) GameRotate(game, false);
  if (dispatching[KEY_HOLD]) GameHold(game);

  if (dispatching[KEY_SLIDE_LEFT]) {
    c->as_left_delay = DAS;
    c->dir = LEFT;
    GameSlide(game, -1);
  } else if (c->pressing[KEY_SLIDE_LEFT]) {
    if (!c->pressing[KEY_SLIDE_RIGHT]) c->dir = LEFT;
    if (c->dir == LEFT) {
      c->as_left_delay -= dt;
      if (c->as_left_delay <= 0) {
        c->as_left_delay = ARR;
        GameSlide(game, -1);
      }
    }
  }
  if (dispatching[KEY_SLIDE_RIGHT]) {
    c->as_right_delay = DAS;
    c->dir = RIGHT;
    GameSlide(game, 1);
  } else if (c->pressing[KEY_SLIDE_RIGHT]) {
    if (!c->pressing[KEY_SLIDE_LEFT]) c->dir = RIGHT;
    if (c->dir == RIGHT) {
      c->as_right_delay -= dt;
      if (c->as_right_delay <= 0) {
        c->as_right_delay = ARR;
        GameSlide(game, 1);
      }
    }
  }

  game->soft_dropping = c->pressing[KEY_DROP_SOFT];
}
