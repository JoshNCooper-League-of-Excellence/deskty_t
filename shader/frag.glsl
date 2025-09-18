#version 450 core

layout(location = 0) out vec4 FragColor;
in vec4 color;
uniform float time;

void main() {
  float hue = fract(time * 0.2);
  vec3 rainbow = 0.5 + 0.5 * cos(6.2831853 * (hue + vec3(0.0, 0.3333, 0.6667)));
  FragColor = vec4(rainbow * color.rgb, color.a);
}