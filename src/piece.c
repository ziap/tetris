#include "piece.h"

typedef struct {
  bool rotation[4][16];
  PieceBounding boundings[4];
  float color[3];

  float offset[2];

  // TODO: Switch to binary format => more efficient iteration (probably)
} PieceProperty;

static const PieceProperty PROPERTIES[PIECE_COUNT] = {
  [PIECE_I] = {
    .boundings = {
      { .y_min = 1, .y_max = 2, },
      { .y_min = 0, .y_max = 4, },
      { .y_min = 2, .y_max = 3, },
      { .y_min = 0, .y_max = 4, },
    },
    .rotation = {
      {
        0, 0, 0, 0,  //
        1, 1, 1, 1,  //
        0, 0, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 0, 1, 0,  //
        0, 0, 1, 0,  //
        0, 0, 1, 0,  //
        0, 0, 1, 0,  //
      },
      {
        0, 0, 0, 0,  //
        0, 0, 0, 0,  //
        1, 1, 1, 1,  //
        0, 0, 0, 0,  //
      },
      {
        0, 1, 0, 0,  //
        0, 1, 0, 0,  //
        0, 1, 0, 0,  //
        0, 1, 0, 0,  //
      },
    },
    .color = {0.0f, 0.79f, 0.945f},
    .offset = {0.0f, 0.0f},
  },
  [PIECE_O] = {
    .boundings = {
      { .y_min = 0, .y_max = 2, },
      { .y_min = 0, .y_max = 2, },
      { .y_min = 0, .y_max = 2, },
      { .y_min = 0, .y_max = 2, },
    },
    .rotation = {
      {
        0, 1, 1, 0,  //
        0, 1, 1, 0,  //
        0, 0, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 1, 1, 0,  //
        0, 1, 1, 0,  //
        0, 0, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 1, 1, 0,  //
        0, 1, 1, 0,  //
        0, 0, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 1, 1, 0,  //
        0, 1, 1, 0,  //
        0, 0, 0, 0,  //
        0, 0, 0, 0,  //
      },
    },
    .color = {0.98f, 0.83f, 0.0f},
    .offset = {0.0f, 0.5f},
  },
  [PIECE_T] = {
    .boundings = {
      { .y_min = 0, .y_max = 2, },
      { .y_min = 0, .y_max = 3, },
      { .y_min = 1, .y_max = 3, },
      { .y_min = 0, .y_max = 3, },
    },
    .rotation = {
      {
        0, 1, 0, 0,  //
        1, 1, 1, 0,  //
        0, 0, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 1, 0, 0,  //
        0, 1, 1, 0,  //
        0, 1, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 0, 0, 0,  //
        1, 1, 1, 0,  //
        0, 1, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 1, 0, 0,  //
        1, 1, 0, 0,  //
        0, 1, 0, 0,  //
        0, 0, 0, 0,  //
      },
    },
    .color = {0.72f, 0.26f, 0.62f},
    .offset = {0.5f, 0.5f},
  },
  [PIECE_S] = {
    .boundings = {
      { .y_min = 0, .y_max = 2, },
      { .y_min = 0, .y_max = 3, },
      { .y_min = 1, .y_max = 3, },
      { .y_min = 0, .y_max = 3, },
    },
    .rotation = {
      {
        0, 1, 1, 0,  //
        1, 1, 0, 0,  //
        0, 0, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 1, 0, 0,  //
        0, 1, 1, 0,  //
        0, 0, 1, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 0, 0, 0,  //
        0, 1, 1, 0,  //
        1, 1, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        1, 0, 0, 0,  //
        1, 1, 0, 0,  //
        0, 1, 0, 0,  //
        0, 0, 0, 0,  //
      },
    },
    .color = {0.0f, 0.73f, 0.23f},
    .offset = {0.5f, 0.5f},
  },
  [PIECE_Z] = {
    .boundings = {
      { .y_min = 0, .y_max = 2, },
      { .y_min = 0, .y_max = 3, },
      { .y_min = 1, .y_max = 3, },
      { .y_min = 0, .y_max = 3, },
    },
    .rotation = {
      {
        1, 1, 0, 0,  //
        0, 1, 1, 0,  //
        0, 0, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 0, 1, 0,  //
        0, 1, 1, 0,  //
        0, 1, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 0, 0, 0,  //
        1, 1, 0, 0,  //
        0, 1, 1, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 1, 0, 0,  //
        1, 1, 0, 0,  //
        1, 0, 0, 0,  //
        0, 0, 0, 0,  //
      },
    },
    .color = {1.0f, 0.0f, 0.13f},
    .offset = {0.5f, 0.5f},
  },
  [PIECE_J] = {
    .boundings = {
      { .y_min = 0, .y_max = 2, },
      { .y_min = 0, .y_max = 3, },
      { .y_min = 1, .y_max = 3, },
      { .y_min = 0, .y_max = 3, },
    },
    .rotation = {
      {
        1, 0, 0, 0,  //
        1, 1, 1, 0,  //
        0, 0, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 1, 1, 0,  //
        0, 1, 0, 0,  //
        0, 1, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 0, 0, 0,  //
        1, 1, 1, 0,  //
        0, 0, 1, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 1, 0, 0,  //
        0, 1, 0, 0,  //
        1, 1, 0, 0,  //
        0, 0, 0, 0,  //
      },
    },
    .color = {0.36f, 0.38f, 0.68f},
    .offset = {0.5f, 0.5f},
  },
  [PIECE_L] = {
    .boundings = {
      { .y_min = 0, .y_max = 2, },
      { .y_min = 0, .y_max = 3, },
      { .y_min = 1, .y_max = 3, },
      { .y_min = 0, .y_max = 3, },
    },
    .rotation = {
      {
        0, 0, 1, 0,  //
        1, 1, 1, 0,  //
        0, 0, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 1, 0, 0,  //
        0, 1, 0, 0,  //
        0, 1, 1, 0,  //
        0, 0, 0, 0,  //
      },
      {
        0, 0, 0, 0,  //
        1, 1, 1, 0,  //
        1, 0, 0, 0,  //
        0, 0, 0, 0,  //
      },
      {
        1, 1, 0, 0,  //
        0, 1, 0, 0,  //
        0, 1, 0, 0,  //
        0, 0, 0, 0,  //
      },
    },
    .color = {0.98f, 0.45f, 0.03f},
    .offset = {0.5f, 0.5f},
  },
};

const bool *FallingPieceGetRotation(FallingPiece p) {
  return PROPERTIES[p.type].rotation[p.rotation];
}

const bool *PieceGetShape(Piece type) {
  return *PROPERTIES[type].rotation;
}

const float *PieceGetColor(Piece type) {
  return PROPERTIES[type].color;
}

const float *PieceGetOffset(Piece type) {
  return PROPERTIES[type].offset;
}

PieceBounding PieceGetBounding(FallingPiece p) {
  return PROPERTIES[p.type].boundings[p.rotation];
}
