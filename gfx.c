#include "gfx.h"
#include "list.h"
#include "stdio.h"
#include "util.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>

typedef struct {
  LIST_OF(vertex) vertices;
} draw_rect_cmd_t;

DEFINE_LIST(draw_rect_cmd_t)

struct {
  GLuint vbo, vao, program;
  bool window_resized;
  vec2I window_size;
  vec2 mouse_position;
  vec2 mouse_delta;
  GLFWwindow *window;
  LIST_OF(draw_rect_cmd_t) cmds;
} gCtx = {0};

void draw_rectangle(rect rect, color color) {
  draw_rect_cmd_t cmd = {0};

  vertex a, b, c, d;

  a.position = (vec3){rect.x, 0, rect.y};
  b.position = (vec3){rect.x + rect.width, 0, rect.y};
  c.position = (vec3){rect.x + rect.width, 0, rect.y + rect.height};
  d.position = (vec3){rect.x, 0, rect.y + rect.height};

  a.scale = b.scale = c.scale = d.scale = vec3_one();
  a.color = b.color = c.color = d.color = color;

  LIST_PUSH(cmd.vertices, a);
  LIST_PUSH(cmd.vertices, b);
  LIST_PUSH(cmd.vertices, c);
  LIST_PUSH(cmd.vertices, d);

  LIST_PUSH(gCtx.cmds, cmd);
}

void gfx_begin_frame() {
  double mx, my;
  glfwGetCursorPos(gCtx.window, &mx, &my);
  gCtx.mouse_delta = (vec2){gCtx.mouse_position.x - mx, gCtx.mouse_position.y - my};
  gCtx.mouse_position = (vec2){mx, my};
}

void gfx_end_frame() {}

void glfw_resize_handler(GLFWwindow *window, int width, int height) {
  gCtx.window_resized = true;
  gCtx.window_size.x = width;
  gCtx.window_size.y = height;
}

void glfw_error_handler(int error_code, const char *description) {
  fprintf(stderr, "%d: %s\n", error_code, description);
}

void gfx_init() {
  if (!glfwInit()) {
    fprintf(stderr, "glfwInit() failed\n");
    return;
  }

  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

  gCtx.window = glfwCreateWindow(800, 600, "desky_t", NULL, NULL);

  if (!gCtx.window) {
    fprintf(stderr, "glfwCreateWindow() returned NULL\n");
    return;
  }

  glfwMakeContextCurrent(gCtx.window);
  glfwSwapInterval(1);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "glewInit() failed: %s\n", glewGetErrorString(err));
    return;
  }

  glfwSetErrorCallback(glfw_error_handler);
  glfwSetFramebufferSizeCallback(gCtx.window, glfw_resize_handler);
  glClearColor(0, 0, 0, 1);

  glGenVertexArrays(1, &gCtx.vao);
  glBindVertexArray(gCtx.vao);

  glGenBuffers(1, &gCtx.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, gCtx.vbo);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex),
                        (void *)offsetof(vertex, position));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, scale));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *)offsetof(vertex, color));
  glEnableVertexAttribArray(2);

  if (!try_gfx_compile_shader_from_files("shader/vert.glsl", "shader/frag.glsl", &gCtx.program)) {
    fprintf(stderr, "Failed to compile shaders\n");
    exit(1);
  }
}

vec2I gfx_window_size() { return gCtx.window_size; }
vec2 gfx_window_size_f() { return (vec2){gCtx.window_size.x, gCtx.window_size.y}; }

bool gfx_window_resized() { return gCtx.window_resized; }
vec2 gfx_get_mouse_position() { return gCtx.mouse_position; }
vec2 gfx_get_mouse_delta() { return gCtx.mouse_delta; }

GLFWwindow *gfx_get_window() { return gCtx.window; }

bool check_point_in_rect(vec2 point, rect rect) {
  return point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y &&
         point.y <= rect.y + rect.height;
}

bool check_rectangles_overlap(rect rect1, rect rect2) {
  return rect1.x < rect2.x + rect2.width && rect1.x + rect1.width > rect2.x &&
         rect1.y < rect2.y + rect2.height && rect1.y + rect1.height > rect2.y;
}

void check_shader_compile(GLuint sh, const char *name) {
  GLint ok = 0;
  glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
  if (!ok) {
    char buf[1024];
    glGetShaderInfoLog(sh, sizeof(buf), NULL, buf);
    fprintf(stderr, "shader %s compile error: %s\n", name, buf);
  }
}

void check_program_link(GLuint prog) {
  GLint ok = 0;
  glGetProgramiv(prog, GL_LINK_STATUS, &ok);
  if (!ok) {
    char buf[1024];
    glGetProgramInfoLog(prog, sizeof(buf), NULL, buf);
    fprintf(stderr, "program link error: %s\n", buf);
  }
}

GLuint gfx_compile_shader(const char *vertex, const char *fragment) {
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex, NULL);
  glCompileShader(vs);
  check_shader_compile(vs, "vertex");

  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment, NULL);
  glCompileShader(fs);
  check_shader_compile(fs, "fragment");

  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  check_program_link(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

bool try_gfx_compile_shader_from_files(const char *vertex_path, const char *fragment_path,
                                       GLuint *program) {
  string_builder_t v = {}, f = {};

  if (!read_file_to_end(vertex_path, &v)) {
    return false;
  }

  if (!read_file_to_end(fragment_path, &f)) {
    return false;
  }

  char *vertex = v.value, *frag = f.value;

  if (!vertex || !frag) {
    return false;
  }

  *program = gfx_compile_shader(vertex, frag);

  sb_free(&v);
  sb_free(&f);

  return true;
}