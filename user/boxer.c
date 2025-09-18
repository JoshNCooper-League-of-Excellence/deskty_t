#include "../os.h"
#include <stddef.h>
#include <stdio.h>

GLuint vbo, vao;

GLuint program;

typedef struct {
  vec3 position;
  vec3 scale;
  vec4 color;
} vertex_t;

constexpr vertex_t vertices[] = {
  // First triangle
  {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},  // Top-left
  {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},   // Top-right
  {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}, // Bottom-left

  // Second triangle
  {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},  // Top-right
  {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}}, // Bottom-right
  {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}} // Bottom-left
};

void init(process_t *proc) {

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)offsetof(vertex_t, position));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)offsetof(vertex_t, scale));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void *)offsetof(vertex_t, color));
  glEnableVertexAttribArray(2);

  string_builder_t vert = {0};
  if (!read_file_to_end("shader/vert.glsl", &vert)) {
    printf("Unable to compile vertex shader\n");
    return;
  }

  string_builder_t frag = {0};
  if (!read_file_to_end("shader/frag.glsl", &frag)) {
    printf("Unable to compile fragment shader\n");
    return;
  }

  char *vert_source = sb_get_string(&vert), *frag_source = sb_get_string(&frag);

  program = gfx_compile_shader(vert_source, frag_source);

  sb_free(&vert);
  sb_free(&frag);

  free(vert_source);
  free(frag_source);

  proc->init_window(proc, 200, 200, "boxer!");
}

void update(process_t *proc) {

  int timeLoc;
  if ((timeLoc = glGetUniformLocation(program, "time")) != -1) {
    glUniform1f(timeLoc, glfwGetTime());
  }

  glUseProgram(program);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

}

void deinit(process_t *proc) {}
