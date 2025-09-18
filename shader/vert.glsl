#version 450 core

layout(location = 0) in vec3 aPos;

void main() {
  // Set the vertex position
  gl_Position = vec4(aPos, 1.0);
}