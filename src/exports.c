#include "exports.h"

#include "controller.h"
#include "imports.h"
#include "renderer.h"

static Game game;
static Renderer ui_renderer;
static Controller controller = {0};

void press(Key key) { controller.pressing[key] = true; }
void release(Key key) { controller.pressing[key] = false; }

void resize(int new_w, int new_h) {
  game.width = new_w;
  game.height = new_h;

  glViewport(0, 0, new_w, new_h);
}

void app_init(void) {
  glClearColor(0.18f, 0.18f, 0.18f, 1.0f);

  GameInit(&game);

  RendererInit(&ui_renderer);
}

void app_update(double dt) {
  glClear(GL_COLOR_BUFFER_BIT);

  ControllerUpdateGame(&controller, &game, dt);
  GameTick(&game, dt);

  RendererUse(&ui_renderer);
  RendererUpdate(&ui_renderer, &game);
  RendererRender();
}
