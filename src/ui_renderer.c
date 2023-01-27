#include "ui_renderer.h"

#include "../shaders/ui.h"
#include "imports.h"
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
} Vertex;

#define VERTICES_CAPACITY 300
static Vertex triangle_vertices[VERTICES_CAPACITY];
static int vertex_count = 0;

static void AddTriangle(
  Vec2 p1, Vec2 p2, Vec2 p3, Color c, Vec2 uv1, Vec2 uv2, Vec2 uv3
) {
  if (vertex_count + 3 > VERTICES_CAPACITY) UIRendererRender();
  triangle_vertices[vertex_count].pos = p1;
  triangle_vertices[vertex_count].color = c;
  triangle_vertices[vertex_count].uv = uv1;
  vertex_count++;

  triangle_vertices[vertex_count].pos = p2;
  triangle_vertices[vertex_count].color = c;

  triangle_vertices[vertex_count].uv = uv2;
  vertex_count++;

  triangle_vertices[vertex_count].pos = p3;
  triangle_vertices[vertex_count].color = c;
  triangle_vertices[vertex_count].uv = uv3;
  vertex_count++;
}

static Vec2 untextured_uv = {0.5f, 0.5f};
static void AddRect(float x, float y, float w, float h, Color c) {
  Vec2 p1 = {x, y};
  Vec2 p2 = {x + w, y};
  Vec2 p3 = {x, y + h};
  Vec2 p4 = {x + w, y + h};

  AddTriangle(p1, p2, p3, c, untextured_uv, untextured_uv, untextured_uv);
  AddTriangle(p2, p3, p4, c, untextured_uv, untextured_uv, untextured_uv);
}

static void AddRectTextured(
  float x, float y, float w, float h, Color c, float u, float v
) {
  Vec2 p1 = {x, y};
  Vec2 p2 = {x + w, y};
  Vec2 p3 = {x, y + h};
  Vec2 p4 = {x + w, y + h};

  Vec2 uv1 = {u, v};
  Vec2 uv2 = {u + 1.0f, v};
  Vec2 uv3 = {u, v + 1.0f};
  Vec2 uv4 = {u + 1.0f, v + 1.0f};

  AddTriangle(p1, p2, p3, c, uv1, uv2, uv3);
  AddTriangle(p2, p3, p4, c, uv2, uv3, uv4);
}

static void AddBorder(float x, float y, float w, float h, float s, Color c) {
  AddRect(x - s, y - s, w + s * 2.0f, s, c);
  AddRect(x - s, y + h, w + s * 2.0f, s, c);
  AddRect(x - s, y, s, h, c);
  AddRect(x + w, y, s, h, c);
}

static void AddPiece(Piece p, float x, float y) {
  const bool *shape = PieceGetShape(p);
  const float *color = PieceGetColor(p);
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
        AddRectTextured(place_x, place_y, 0.75f, 0.75f, c, 0.0f, 0.0f);
      }
    }
  }
}

void UIRendererInit(UIRenderer *ur) {
  ur->program = create_program(ui_vert, ui_frag);

  ur->vao = glCreateVertexArray();
  ur->vbo = glCreateBuffer();

  glBindVertexArray(ur->vao);
  glBindBuffer(GL_ARRAY_BUFFER, ur->vbo);
  glBufferData(
    GL_ARRAY_BUFFER, sizeof(triangle_vertices), NULL, GL_DYNAMIC_DRAW
  );

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

  ur->u_resolution = glGetUniformLocation(ur->program, "u_resolution");
}

void UIRendererUpdate(UIRenderer *ur, Game *game) {
  glUniform2f(ur->u_resolution, game->width, game->height);

  Color ui_bg = {0.12f, 0.12f, 0.12f};
  AddRect(0.5f, 0.0f, 4.0f, 3.0f, ui_bg);
  AddRect(15.5f, 0.0f, 4.0f, 20.0f, ui_bg);

  Color border = {0.34f, 0.34f, 0.34f};
  AddBorder(0.5f, 0.0f, 4.0f, 3.0f, 0.1f, border);
  AddBorder(15.5f, 0.0f, 4.0f, 20.0f, 0.1f, border);
  AddBorder(5.0f, 0.0f, 10.0f, 20.0f, 0.1f, border);

  if (game->hold_piece != PIECE_EMPTY) AddPiece(game->hold_piece, 0.5f, 0.0f);

  for (int i = 0; i < 6; ++i) {
    int idx = game->bag_index + i;
    if (idx < PIECE_COUNT - 1) {
      AddPiece(game->current_bag[idx], 15.5f, 3.4f * i);
    } else {
      AddPiece(game->next_bag[idx - (PIECE_COUNT - 1)], 15.5f, 3.4f * i);
    }
  }
}

void UIRendererUse(UIRenderer *ur) {
  glUseProgram(ur->program);
  glBindVertexArray(ur->vao);
  glBindBuffer(GL_ARRAY_BUFFER, ur->vbo);
  vertex_count = 0;
}

void UIRendererRender() {
  glBufferSubData(
    GL_ARRAY_BUFFER, 0, sizeof(Vertex) * vertex_count, triangle_vertices
  );
  glDrawArrays(GL_TRIANGLES, 0, vertex_count);
  vertex_count = 0;
}
