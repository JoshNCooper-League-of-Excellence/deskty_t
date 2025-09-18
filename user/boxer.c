#include "../os.h"
#include <stddef.h>

GLuint vbo, vao, program;

constexpr vertex vertices[] = {
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

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, position));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, scale));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, color));
  glEnableVertexAttribArray(2);


  if (!try_gfx_compile_shader_from_files("shader/vert.glsl", "shader/rainbow-frag.glsl", &program)) {
    return;
  }

  proc->init_window(proc, 200, 200, "boxer!");
}

void update(process_t *proc) {

  // int timeLoc;
  // if ((timeLoc = glGetUniformLocation(program, "time")) != -1) {
  //   glUniform1f(timeLoc, glfwGetTime());
  // }

  // glUseProgram(program);
  // glBindVertexArray(vao);
  // glDrawArrays(GL_TRIANGLES, 0, 6);
  // glBindVertexArray(0);

}

void deinit(process_t *proc) {}
