#version 300 es

#ifdef GL_ES
precision highp float;
#endif

#define BORDER 0.1

in vec3 out_color;
in vec2 out_uv;

out vec4 frag_color;

void main() {
  float x = out_uv.x;
  float y = out_uv.y;

  if (x > BORDER && x < 1.0 - BORDER && y > BORDER && y < 1.0 - BORDER) {
    frag_color = vec4(out_color * 0.9, 1.0);
  } else {
    if (x > y) {
      if ((1.0 - x) > y) {
        frag_color = vec4(out_color * 1.0, 1.0);
      } else {
        frag_color = vec4(out_color * 0.7, 1.0);
      }
    } else {
      if ((1.0 - x) > y) {
        frag_color = vec4(out_color * 0.85, 1.0);
      } else {
        frag_color = vec4(out_color * 0.5, 1.0);
      }
    }
  }
}
