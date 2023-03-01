#ifndef RENDERER_H
#define RENDERER_H

#include "game.h"

typedef struct {
  int u_resolution;
  int program;
  int vao;
  int vbo;
} Renderer;

extern void RendererInit(Renderer *);
extern void RendererUse(Renderer *);
extern void RendererUpdate(Renderer *, Game *);
extern void RendererRender(void);

#endif
