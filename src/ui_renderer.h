#ifndef UI_RENDERER_H
#define UI_RENDERER_H

#include "game.h"

typedef struct {
  int u_resolution;
  int program;
  int vao;
  int vbo;
} UIRenderer;

extern void UIRendererInit(UIRenderer *);
extern void UIRendererUse(UIRenderer *);
extern void UIRendererUpdate(UIRenderer *, Game *);
extern void UIRendererRender();

#endif
