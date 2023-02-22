#include "game.h"

static int line_sz(int lines) { return sizeof(Piece) * 10 * lines; }

static void ShuffleBag(Piece *bag) {
  for (int i = 0; i < PIECE_COUNT - 1; ++i) {
    int j = randint(PIECE_COUNT - 1);

    Piece x = bag[i];
    bag[i] = bag[j];
    bag[j] = x;
  }
}

static bool Collision(
  Game *game, const bool *rotation_table, int pos_x, int pos_y
) {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (!rotation_table[4 * j + i]) continue;

      int x = pos_x + i;
      int y = pos_y + j;

      if (x >= 0 && x < 10 && y >= 0 && y < 20) {
        if (game->data[10 * y + x] != PIECE_EMPTY) return true;
      } else {
        return true;
      }
    }
  }

  return false;
}

static void SetFallingPiece(Game *game) {
  game->falling.type = game->current_bag[game->bag_index++];
  if (game->bag_index == PIECE_COUNT - 1) {
    game->bag_index = 0;

    Piece *x = game->next_bag;
    game->next_bag = game->current_bag;
    game->current_bag = x;

    ShuffleBag(game->next_bag);
  }
  game->falling.rotation = 0;
  game->falling.x = 3;
  game->falling.y = 0;
  game->gravity_delay = 1;

  if (Collision(game, PieceGetShape(game->falling.type), 3, 0)) {
    game->over = true;
  }
}

static void LevelUp(Game *game) {
  game->level += game->level_progress / 5;
  game->level_progress %= 5;

  game->speed = 1.0f;
  for (int i = 0; i < game->level; ++i) {
    game->speed *= (0.8f - game->level * 0.007f);
  }
}

static void UpdateGhost(Game *game) {
  game->ghost_y = game->falling.y;

  const bool *rotation_table = FallingPieceGetRotation(&game->falling);

  while (true) {
    if (Collision(game, rotation_table, game->falling.x, game->ghost_y + 1)) {
      break;
    }
    game->ghost_y++;
  }
}

static void LineClear(Game *game) {
  int current_row = game->ghost_y + 3;
  int line_cleared = 0;

  for (int i = current_row; i >= game->ghost_y; --i) {
    bool filled = true;

    for (int j = 0; j < 10; ++j) {
      game->data[10 * current_row + j] = game->data[10 * i + j];

      if (game->data[10 * i + j] == PIECE_EMPTY) filled = false;
    }

    if (!filled) {
      current_row--;
    } else {
      line_cleared++;
    }
  }

  if (line_cleared > 0) {
    memmove(game->data + line_cleared * 10, game->data, line_sz(game->ghost_y));
    memset(game->data, 0, line_sz(line_cleared));

    game->line_cleared += line_cleared;
    game->level_progress += line_cleared;

    LevelUp(game);

    // TODO: Implement scoring system
  }
}

static void Lock(Game *game) {
  const bool *rotation_table = FallingPieceGetRotation(&game->falling);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (!rotation_table[4 * j + i]) continue;

      int x = game->falling.x + i;
      int y = game->ghost_y + j;

      if (x >= 0 && x < 10 && y >= 0 && y < 20) {
        if (game->data[10 * y + x] == PIECE_EMPTY) {
          game->data[10 * y + x] = game->falling.type;
        }
      }
    }
  }

  game->hit_ground = false;
  game->lock_resets = 0;
  game->held = false;

  LineClear(game);
  SetFallingPiece(game);
  UpdateGhost(game);
}

static void ResetLock(Game *game) {
  if (game->falling.y >= game->ghost_y) game->hit_ground = true;
  if (game->hit_ground) {
    game->lock_resets++;
    if (game->lock_resets < 15) game->gravity_delay = 1;
  }
}

typedef struct {
  int x;
  int y;
} Offset;

static Offset KICK_NORMAL[4][4] = {
  {{-1, 0}, {-1, +1}, {0, -2}, {-1, -2}},  // 3 >> 0
  {{-1, 0}, {-1, -1}, {0, +2}, {-1, +2}},  // 0 >> 1
  {{+1, 0}, {+1, +1}, {0, -2}, {+1, -2}},  // 1 >> 2
  {{+1, 0}, {+1, -1}, {0, +2}, {+1, +2}},  // 2 >> 3
};

static Offset KICK_I[4][4] = {
  {{+1, 0}, {-2, 0}, {+1, +2}, {-2, -1}},  // 3 >> 0
  {{-2, 0}, {+1, 0}, {-2, +1}, {+1, -2}},  // 0 >> 1
  {{-1, 0}, {+2, 0}, {-1, -2}, {+2, +1}},  // 1 >> 2
  {{+2, 0}, {-1, 0}, {+2, -1}, {-1, +2}},  // 2 >> 3
};

static bool CheckWallKick(Game *game, FallingPiece *p, bool cw) {
  const bool *rotation_table = FallingPieceGetRotation(p);
  if (!Collision(game, rotation_table, p->x, p->y)) return true;

  // Index of n - 1 >> n: n
  // Index of n + 1 >> n: (n + 1) % 4
  int idx = (p->rotation + !cw) % 4;
  Offset *kick_table = p->type == PIECE_I ? KICK_I[idx] : KICK_NORMAL[idx];

  for (Offset *offset = kick_table; offset != kick_table + 4; ++offset) {
    int x = p->x + (cw ? offset->x : -offset->x);
    int y = p->y + (cw ? offset->y : -offset->y);

    if (Collision(game, rotation_table, x, y)) continue;

    p->x = x;
    p->y = y;
    return true;
  }

  return false;
}

void GameInit(Game *game) {
  game->level = 0;
  game->speed = 1;

  game->score = 0;
  game->line_cleared = 0;
  game->level_progress = 0;

  game->soft_dropping = false;
  game->soft_dropped = false;

  game->gravity_delay = 1;
  game->hit_ground = false;
  game->lock_resets = 0;

  game->current_bag = game->bag1;
  game->next_bag = game->bag2;
  game->bag_index = 0;

  game->held = false;
  game->hold_piece = PIECE_EMPTY;

  game->over = false;

  for (int i = 1; i < PIECE_COUNT; ++i) {
    game->bag1[i - 1] = i;
    game->bag2[i - 1] = i;
  }

  ShuffleBag(game->bag1);
  ShuffleBag(game->bag2);

  memset(game->data, 0, line_sz(20));

  SetFallingPiece(game);
  UpdateGhost(game);
}

void GameTick(Game *game, float dt) {
  if (game->over) return;

  if (game->falling.y < game->ghost_y) {
    float dg = dt / game->speed;
    if (game->soft_dropping) {
      dg *= 10;
      game->soft_dropped = true;
    }
    game->gravity_delay -= dg;

    if (game->gravity_delay <= 0) {
      while (game->gravity_delay <= 0) {
        game->falling.y++;
        game->gravity_delay += 1;

        if (game->soft_dropped) game->score++;
        if (game->falling.y >= game->ghost_y) {
          game->hit_ground = true;
          break;
        }
      }
      game->soft_dropped = false;
      game->gravity_delay = 1;
    }
  } else {
    game->gravity_delay -= dt / 0.5f;
    if (game->gravity_delay <= 0) Lock(game);
  }
}

void GameSlide(Game *game, int dx) {
  if (game->over) return;

  const bool *rotation_table = FallingPieceGetRotation(&game->falling);

  if (!Collision(game, rotation_table, game->falling.x + dx, game->falling.y)) {
    game->falling.x += dx;
    UpdateGhost(game);
    ResetLock(game);
  }
}

void GameDropHard(Game *game) {
  if (game->over) return;

  game->score += 2 * (game->ghost_y - game->falling.y);
  Lock(game);
  game->gravity_delay = 1;
}

void GameRotate(Game *game, bool cw) {
  if (game->over) return;

  FallingPiece p = game->falling;
  if (p.type == PIECE_O) return;

  int d = cw * 2 - 1;
  p.rotation = (p.rotation + 4 + d) % 4;

  if (CheckWallKick(game, &p, cw)) {
    game->falling = p;
    UpdateGhost(game);
    ResetLock(game);
  }
}

void GameHold(Game *game) {
  if (game->over) return;

  if (game->held) return;
  if (game->hold_piece == PIECE_EMPTY) {
    game->hold_piece = game->falling.type;
    SetFallingPiece(game);
  } else {
    Piece x = game->falling.type;
    game->falling.type = game->hold_piece;
    game->hold_piece = x;

    game->falling.rotation = 0;
    game->falling.x = 3;
    game->falling.y = 0;
  }
  game->held = true;
  game->gravity_delay = 1;
  UpdateGhost(game);
}
