#ifndef GAME_H
#define GAME_H

#include "piece.h"

typedef struct {
  int width;
  int height;

  int level;
  float speed;

  int score;
  int line_cleared;
  int level_progress;

  bool soft_dropping;
  bool soft_dropped;

  float gravity_delay;
  FallingPiece falling;

  int ghost_y;
  bool hit_ground;
  int lock_resets;

  Piece bag1[PIECE_COUNT - 1];
  Piece bag2[PIECE_COUNT - 1];

  Piece* current_bag;
  Piece* next_bag;

  int bag_index;

  bool held;
  Piece hold_piece;

  bool over;

  Piece data[20 * 10];
} Game;

void GameInit(Game*);
void GameTick(Game*, float);

void GameSlide(Game*, int);
void GameRotate(Game*, bool);

void GameDropSoft(Game*);
void GameDropHard(Game*);

void GameHold(Game*);

#endif
