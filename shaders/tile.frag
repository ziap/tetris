#version 300 es

#ifdef GL_ES
precision highp float;
#endif

#define BORDER 0.1
#define DOT_RADIUS 0.08

in vec3 out_color;
in vec2 tile_uv;
in vec2 tile_coord;

out vec4 frag_color;

void main() {
  float x = tile_uv.x;
  float y = tile_uv.y;

  if (length(out_color) > 0.0) {

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

  } else {
    float dx1 = abs(x - floor(x));
    float dx2 = abs(x - ceil(x));
    float dy1 = abs(y - floor(y));
    float dy2 = abs(y - ceil(y));

    float dx = min(dx1, dx2);
    float dy = min(dy1, dy2);

    if (dx * dx + dy * dy < DOT_RADIUS * DOT_RADIUS) {
      frag_color = vec4(0.44, 0.44, 0.44, 1.0);
    } else {
      frag_color = vec4(0.12, 0.12, 0.12, 1.0);
    }
  }
}
