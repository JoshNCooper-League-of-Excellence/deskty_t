#include "../os.h"

GLuint vbo, vao;

GLuint program;

void init(process_t *proc) {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  float vertices[] = {
      // First triangle
      -0.5f, 0.5f, 0.0f,  // Top-left
      0.5f, 0.5f, 0.0f,   // Top-right
      -0.5f, -0.5f, 0.0f, // Bottom-left

      // Second triangle
      0.5f, 0.5f, 0.0f,  // Top-right
      0.5f, -0.5f, 0.0f, // Bottom-right
      -0.5f, -0.5f, 0.0f // Bottom-left
  };

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  string_builder_t vert = {0};
  if (!read_file_to_end("shader/vert.glsl", &vert)) {
    return;
  }

  string_builder_t frag = {0};
  if (!read_file_to_end("shader/frag.glsl", &frag)) {
    return;
  }

  char *vert_source = sb_get_string(&vert), *frag_source = sb_get_string(&frag);

  gfx_compile_shader(vert_source, frag_source);

  sb_free(&vert);
  sb_free(&frag);

  free(vert_source);
  free(frag_source);

  // proc->init_window(proc, 200, 200, "boxer!");
}

void update(process_t *proc) {
  glUseProgram(program);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void deinit(process_t *proc) {}
