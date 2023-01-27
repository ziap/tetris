#include "exports.h"

#include "controller.h"
#include "imports.h"
#include "tile_renderer.h"
#include "ui_renderer.h"

static Game game;
static TileRenderer tile_renderer;
static UIRenderer ui_renderer;
static Controller controller = {0};

void press(Key key) { controller.pressing[key] = true; }
void release(Key key) { controller.pressing[key] = false; }

void resize(int new_w, int new_h) {
  game.width = new_w;
  game.height = new_h;

  glViewport(0, 0, new_w, new_h);
}

void app_init() {
  glClearColor(0.18f, 0.18f, 0.18f, 1.0f);

  GameInit(&game);

  UIRendererInit(&ui_renderer);
  TileRendererInit(&tile_renderer);
}

void app_update(float dt) {
  (void)dt;
  glClear(GL_COLOR_BUFFER_BIT);

  ControllerUpdateGame(&controller, &game, dt);
  GameTick(&game, dt);

  UIRendererUse(&ui_renderer);
  UIRendererUpdate(&ui_renderer, &game);
  UIRendererRender();

  TileRendererUse(&tile_renderer);
  TileRendererUpdate(&tile_renderer, &game);
  TileRendererRender(&tile_renderer);
}
