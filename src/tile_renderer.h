#ifndef TILE_RENDERER_H
#define TILE_RENDERER_H

#include "game.h"

typedef struct {
  int u_resolution;
  int program;
  int vao;
  int vbo;
} TileRenderer;

extern void TileRendererInit(TileRenderer *);
extern void TileRendererUse(TileRenderer *);
extern void TileRendererUpdate(TileRenderer *, Game *);
extern void TileRendererRender(TileRenderer *);

#endif
