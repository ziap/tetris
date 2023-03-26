#version 300 es

#ifdef GL_ES
precision highp float;
#endif

#define MARGIN 88.0 * 2.0

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec3 a_color;
layout (location = 2) in vec2 a_uv;
layout (location = 3) in float a_type;

out vec3 out_color;
out vec2 out_uv;
out float out_type;

uniform vec2 u_resolution;

void main() {
  int id = gl_VertexID;

  float m = min(u_resolution.x, u_resolution.y);
  vec2 scale = (m - MARGIN) / u_resolution;

  gl_Position = vec4((vec2(a_pos.x, 20.0 - a_pos.y) / 20.0 * 2.0 - 1.0) * scale, 0.0, 1.0);
  
  out_color = a_color;
  out_uv = a_uv;
  out_type = a_type;
}
