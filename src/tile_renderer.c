#include "tile_renderer.h"

#include "../shaders/tile.h"
#include "imports.h"
#include "shader.h"

typedef struct {
  float color[3];
} TileVertex;

#define VERTEX_COUNT (6 * 20 * 10)
static TileVertex vertices[VERTEX_COUNT];

void TileRendererInit(TileRenderer *tr) {
  tr->program = create_program(tile_vert, tile_frag);

  tr->vao = glCreateVertexArray();
  tr->vbo = glCreateBuffer();

  glBindVertexArray(tr->vao);
  glBindBuffer(GL_ARRAY_BUFFER, tr->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0, 3, GL_FLOAT, false, sizeof(TileVertex),
    (const void *)offsetof(TileVertex, color)
  );

  tr->u_resolution = glGetUniformLocation(tr->program, "u_resolution");
}

void TileRendererUpdate(TileRenderer *tr, Game *game) {
  glUniform2f(tr->u_resolution, game->width, game->height);

  for (int i = 0; i < 20 * 10; ++i) {
    const float *color = PieceGetColor(game->data[i]);

    for (int j = 0; j < 6; j++) {
      vertices[6 * i + j].color[0] = color[0];
      vertices[6 * i + j].color[1] = color[1];
      vertices[6 * i + j].color[2] = color[2];
    }
  }

  const bool *rotation_table = FallingPieceGetRotation(&game->falling);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (!rotation_table[4 * j + i]) continue;
      int x = game->falling.x + i;
      int y = game->ghost_y + j;
      if (x >= 0 && x < 10 && y >= 0 && y < 20) {
        const float *color = PieceGetColor(game->falling.type);

        int idx = 10 * y + x;
        for (int k = 0; k < 6; ++k) {
          vertices[6 * idx + k].color[0] = color[0] * 0.35f;
          vertices[6 * idx + k].color[1] = color[1] * 0.35f;
          vertices[6 * idx + k].color[2] = color[2] * 0.35f;
        }
      }

      y = game->falling.y + j;
      if (x >= 0 && x < 10 && y >= 0 && y < 20) {
        const float *color = PieceGetColor(game->falling.type);

        int idx = 10 * y + x;
        for (int k = 0; k < 6; ++k) {
          vertices[6 * idx + k].color[0] = color[0];
          vertices[6 * idx + k].color[1] = color[1];
          vertices[6 * idx + k].color[2] = color[2];
        }
      }
    }
  }
}

void TileRendererUse(TileRenderer *tr) {
  glUseProgram(tr->program);
  glBindBuffer(GL_ARRAY_BUFFER, tr->vbo);
  glBindVertexArray(tr->vao);
}

void TileRendererRender(TileRenderer *tr) {
  (void)tr;
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
  glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);
}
