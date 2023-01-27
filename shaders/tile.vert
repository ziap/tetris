#version 300 es

#ifdef GL_ES
precision highp float;
#endif

uniform vec2 u_resolution;

layout (location = 0) in vec3 a_color;

out vec3 out_color;
out vec2 tile_uv;
out vec2 tile_coord;

#define MARGIN 24.0 * 2.0

void main() {
  vec2 map_size = vec2(10.0, 20.0);

  vec2 pos;
  int tile_id = gl_VertexID / 6;
  int vert_id = gl_VertexID % 6;

  float x = float(tile_id % 10);
  float y = float(tile_id / 10);

  tile_uv.x = float(vert_id % 2 != 0);
  tile_uv.y = float((vert_id + 4) % 6 < 3);

  pos.x = (tile_uv.x + x) / map_size.x;
  pos.y = 1.0 - (tile_uv.y + y) / map_size.y;

  float m = min(u_resolution.x, u_resolution.y);
  vec2 scale = (m - MARGIN) / u_resolution;

  gl_Position = vec4((pos * 2.0 - 1.0) * scale * vec2(0.5, 1.0), 0.0, 1.0);
  tile_coord = vec2(x, y);
  out_color = a_color;
}
