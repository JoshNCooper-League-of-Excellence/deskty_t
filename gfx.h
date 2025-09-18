#ifndef GFX_H
#define GFX_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

typedef struct {
  unsigned char r, g, b, a;
} Color;

#define CLITERAL(v) (v)

#define WHITE CLITERAL(Color){255, 255, 255, 255}
#define BLACK CLITERAL(Color){0, 0, 0, 255}
#define LIGHT_GRAY CLITERAL(Color){211, 211, 211, 255}

typedef struct {
  float x, y;
} Vector2;

typedef struct {
  int x, y;
} Vector2I;

typedef struct {
  float x, y, z;
} Vector3;

typedef struct {
  float x, y, width, height;
} Rectangle;

void glfw_error_handler(int error_code, const char *description);
void glfw_resize_handler(GLFWwindow *window, int width, int height);
void gfx_init();

void gfx_begin_frame();

GLFWwindow *gfx_get_window();

Vector2I gfx_window_size();
Vector2 gfx_window_size_f();
bool gfx_window_resized();

Vector2 gfx_get_mouse_position();
Vector2 gfx_get_mouse_delta();

bool check_point_in_rect(Vector2 point, Rectangle rect);
bool check_rectangles_overlap(Rectangle rect1, Rectangle rect2);

GLuint gfx_compile_shader(const char *vertex, const char *fragment);


#endif