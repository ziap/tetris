#include "renderer.h"

#include "imports.h"
#include "resources.h"
#include "shader.h"

typedef struct {
  float x;
  float y;
} Vec2;

typedef struct {
  float r;
  float g;
  float b;
} Color;

typedef struct {
  Vec2 pos;
  Color color;
  Vec2 uv;
  int type;
} Vertex;

#define VERTICES_CAPACITY 3 * 1024
static Vertex vertices[VERTICES_CAPACITY];
static int vertex_count = 0;

static void AddTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Color c, Vec2 uv1, Vec2 uv2, Vec2 uv3, int type) {
  if (vertex_count + 3 > VERTICES_CAPACITY) RendererRender();
  vertices[vertex_count].pos = p1;
  vertices[vertex_count].color = c;
  vertices[vertex_count].uv = uv1;
  vertices[vertex_count].type = type;
  vertex_count++;

  vertices[vertex_count].pos = p2;
  vertices[vertex_count].color = c;
  vertices[vertex_count].uv = uv2;
  vertices[vertex_count].type = type;
  vertex_count++;

  vertices[vertex_count].pos = p3;
  vertices[vertex_count].color = c;
  vertices[vertex_count].uv = uv3;
  vertices[vertex_count].type = type;
  vertex_count++;
}

static void AddRectTextured(float x, float y, float w, float h, Color c, float u, float v, float uv_scale, int type) {
  Vec2 p1 = {x, y};
  Vec2 p2 = {x + w, y};
  Vec2 p3 = {x, y + h};
  Vec2 p4 = {x + w, y + h};

  float uvw = w / uv_scale;
  float uvh = h / uv_scale;

  Vec2 uv1 = {u, v};
  Vec2 uv2 = {u + uvw, v};
  Vec2 uv3 = {u, v + uvh};
  Vec2 uv4 = {u + uvw, v + uvh};

  AddTriangle(p1, p2, p3, c, uv1, uv2, uv3, type);
  AddTriangle(p2, p3, p4, c, uv2, uv3, uv4, type);
}

static void AddRect(float x, float y, float w, float h, Color c) {
  AddRectTextured(x, y, w, h, c, 0, 0, 0, 0);
}

static void AddBorder(float x, float y, float w, float h, float s, Color c) {
  AddRect(x - s, y - s, w + s * 2, s, c);
  AddRect(x - s, y + h, w + s * 2, s, c);
  AddRect(x - s, y, s, h, c);
  AddRect(x + w, y, s, h, c);
}

static float hold_color[3] = {0.72f, 0.72f, 0.72f};

static void AddPiece(Piece p, float x, float y, bool held) {
  const bool *shape = PieceGetShape(p);
  const float *color = held ? hold_color : PieceGetColor(p);
  const float *offset = PieceGetOffset(p);

  x += offset[0] * 0.75f + 4 * 0.125f;
  y += offset[1] * 0.75f + 3 * 0.125f;

  Color c;
  c.r = color[0];
  c.g = color[1];
  c.b = color[2];

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 2; ++j) {
      if (shape[4 * j + i]) {
        float place_x = x + i * 0.75f;
        float place_y = y + j * 0.75f;
        AddRectTextured(place_x, place_y, 0.75f, 0.75f, c, 0, 0, 0.75f, 1);
      }
    }
  }
}

void RendererInit(Renderer *ur) {
  ur->program = create_program(shaders_tetris_vert, shaders_tetris_frag);

  ur->vao = glCreateVertexArray();
  ur->vbo = glCreateBuffer();

  glBindVertexArray(ur->vao);
  glBindBuffer(GL_ARRAY_BUFFER, ur->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), NULL, GL_DYNAMIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
    0, 2, GL_FLOAT, false, sizeof(Vertex), (const void *)offsetof(Vertex, pos)
  );

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
    1, 3, GL_FLOAT, false, sizeof(Vertex), (const void *)offsetof(Vertex, color)
  );

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(
    2, 2, GL_FLOAT, false, sizeof(Vertex), (const void *)offsetof(Vertex, uv)
  );

  glEnableVertexAttribArray(3);
  glVertexAttribPointer(
    3, 1, GL_INT, false, sizeof(Vertex), (const void *)offsetof(Vertex, type)
  );

  ur->u_resolution = glGetUniformLocation(ur->program, "u_resolution");
}

void RendererUpdate(Renderer *ur, Game *game) {
  glUniform2f(ur->u_resolution, game->width, game->height);

  Color ui_bg = {0.12f, 0.12f, 0.12f};
  AddRect(0.5f, 0, 4, 3, ui_bg);
  AddRect(15.5f, 0, 4, 20, ui_bg);

  AddRectTextured(5, 0, 10, 20, ui_bg, 0, 0, 1, 2);

  Color border = {0.34f, 0.34f, 0.34f};
  AddBorder(0.5f, 0, 4, 3, 0.1f, border);
  AddBorder(15.5f, 0, 4, 20, 0.1f, border);
  AddBorder(5, 0, 10, 20, 0.1f, border);

  if (game->hold_piece != PIECE_EMPTY) {
    AddPiece(game->hold_piece, 0.5f, 0, game->held);
  }

  for (int i = 0; i < 6; ++i) {
    int idx = game->bag_index + i;
    if (idx < PIECE_COUNT - 1) {
      AddPiece(game->current_bag[idx], 15.5f, 3.4f * i, false);
    } else {
      AddPiece(game->next_bag[idx - (PIECE_COUNT - 1)], 15.5f, 3.4f * i, false);
    }
  }

  for (int i = 0; i < 20; ++i) {
    for (int j = 0; j < 10; ++j) {
      if (game->data[10 * i + j]) {
        const float *color = PieceGetColor(game->data[10 * i + j]);
        Color c;
        c.r = color[0];
        c.g = color[1];
        c.b = color[2];
        AddRectTextured(5 + j, i, 1, 1, c, 0, 0, 1, 1);
      }
    }
  }

  const bool *rotation_table = FallingPieceGetRotation(game->falling);
  const float *color = PieceGetColor(game->falling.type);

  Color c;
  c.r = color[0];
  c.g = color[1];
  c.b = color[2];

  Color c1;
  c1.r = c.r * 0.35f;
  c1.g = c.g * 0.35f;
  c1.b = c.b * 0.35f;

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (!rotation_table[4 * j + i]) continue;

      int x = game->falling.x + i;
      int y = game->ghost_y + j;
      AddRectTextured(x + 5, y, 1, 1, c1, 0, 0, 1, 1);

      int y1 = game->falling.y + j;
      AddRectTextured(x + 5, y1, 1, 1, c, 0, 0, 1, 1);
    }
  }
}

void RendererUse(Renderer *ur) {
  glUseProgram(ur->program);
  glBindVertexArray(ur->vao);
  glBindBuffer(GL_ARRAY_BUFFER, ur->vbo);
  vertex_count = 0;
}

void RendererRender(void) {
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertex_count, vertices);
  glDrawArrays(GL_TRIANGLES, 0, vertex_count);
  vertex_count = 0;
}
