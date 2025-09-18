#include "gfx.h"
#include "stdio.h"
#include <GLFW/glfw3.h>

struct {
  bool window_resized;
  Vector2I window_size;
  Vector2 mouse_position;
  Vector2 mouse_delta;
  GLFWwindow *window;
} gContext;

void gfx_begin_frame() {
  double mx, my;
  glfwGetCursorPos(gContext.window, &mx, &my);
  gContext.mouse_delta = (Vector2){gContext.mouse_position.x - mx, gContext.mouse_position.y - my};
  gContext.mouse_position = (Vector2){mx, my};
}

void glfw_resize_handler(GLFWwindow *window, int width, int height) {
  gContext.window_resized = true;
  gContext.window_size.x = width;
  gContext.window_size.y = height;
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

  gContext.window = glfwCreateWindow(800, 600, "desky_t", NULL, NULL);

  if (!gContext.window) {
    fprintf(stderr, "glfwCreateWindow() returned NULL\n");
    return;
  }

  glfwMakeContextCurrent(gContext.window);
  glfwSwapInterval(1);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "glewInit() failed: %s\n", glewGetErrorString(err));
    return;
  }

  glfwSetErrorCallback(glfw_error_handler);
  glfwSetFramebufferSizeCallback(gContext.window, glfw_resize_handler);
  glClearColor(0, 0, 0, 1);
}

Vector2I gfx_window_size() { return gContext.window_size; }
Vector2 gfx_window_size_f() { return (Vector2){gContext.window_size.x, gContext.window_size.y}; }

bool gfx_window_resized() { return gContext.window_resized; }
Vector2 gfx_get_mouse_position() { return gContext.mouse_position; }
Vector2 gfx_get_mouse_delta() { return gContext.mouse_delta; }

GLFWwindow *gfx_get_window() { return gContext.window; }

bool check_point_in_rect(Vector2 point, Rectangle rect) {
  return point.x >= rect.x && point.x <= rect.x + rect.width && point.y >= rect.y &&
         point.y <= rect.y + rect.height;
}

bool check_rectangles_overlap(Rectangle rect1, Rectangle rect2) {
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