#ifndef PIECE_H
#define PIECE_H

#include "imports.h"

typedef enum {
  PIECE_EMPTY,
  PIECE_I,
  PIECE_O,
  PIECE_T,
  PIECE_S,
  PIECE_Z,
  PIECE_J,
  PIECE_L,
  PIECE_COUNT,
} Piece;

typedef struct {
  int y_min;
  int y_max;
} PieceBounding;

typedef struct {
  Piece type;

  int x;
  int y;

  int rotation;
} FallingPiece;

const bool *FallingPieceGetRotation(FallingPiece);

const bool *PieceGetShape(Piece);
const float *PieceGetColor(Piece);
const float *PieceGetOffset(Piece);
PieceBounding PieceGetBounding(FallingPiece);

#endif
