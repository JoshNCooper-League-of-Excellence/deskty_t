#ifndef GFX_H
#define GFX_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "list.h"

typedef struct {
  float x, y;
} vec2;

typedef struct {
  int x, y;
} vec2I;

typedef struct {
  float x, y, z;
} vec3;

typedef struct {
  float x, y, z, w;
} vec4;

typedef struct {
  float x, y, width, height;
} rect;

typedef vec4 color;

constexpr color WHITE = {1, 1, 1, 1};
constexpr color LIGHT_GRAY = {0.827f, 0.827f, 0.827f, 1.0f};
constexpr color BLACK = {0, 0, 0, 1};
constexpr color RED = {1.0f, 0.0f, 0.0f, 1.0f};
constexpr color GREEN = {0.0f, 1.0f, 0.0f, 1.0f};
constexpr color BLUE = {0.0f, 0.0f, 1.0f, 1.0f};
constexpr color YELLOW = {1.0f, 1.0f, 0.0f, 1.0f};
constexpr color ORANGE = {1.0f, 0.5f, 0.0f, 1.0f};
constexpr color PURPLE = {0.5f, 0.0f, 0.5f, 1.0f};
constexpr color CYAN = {0.0f, 1.0f, 1.0f, 1.0f};
constexpr color MAGENTA = {1.0f, 0.0f, 1.0f, 1.0f};
constexpr color BROWN = {0.6f, 0.3f, 0.0f, 1.0f};
constexpr color PINK = {1.0f, 0.75f, 0.8f, 1.0f};
constexpr color GRAY = {0.5f, 0.5f, 0.5f, 1.0f};
constexpr color DARK_GRAY = {0.25f, 0.25f, 0.25f, 1.0f};
constexpr color LIGHT_BLUE = {0.68f, 0.85f, 0.9f, 1.0f};
constexpr color LIGHT_GREEN = {0.56f, 0.93f, 0.56f, 1.0f};
constexpr color LIGHT_YELLOW = {1.0f, 1.0f, 0.88f, 1.0f};
constexpr color LIGHT_PINK = {1.0f, 0.71f, 0.76f, 1.0f};
constexpr color DARK_RED = {0.5f, 0.0f, 0.0f, 1.0f};
constexpr color DARK_GREEN = {0.0f, 0.5f, 0.0f, 1.0f};
constexpr color DARK_BLUE = {0.0f, 0.0f, 0.5f, 1.0f};

typedef struct {
  vec3 position;
  vec3 scale;
  vec4 color;
} vertex;

void glfw_error_handler(int error_code, const char *description);
void glfw_resize_handler(GLFWwindow *window, int width, int height);
void gfx_init();

void gfx_begin_frame();
void gfx_end_frame();

GLFWwindow *gfx_get_window();

vec2I gfx_window_size();
vec2 gfx_window_size_f();
bool gfx_window_resized();

vec2 gfx_get_mouse_position();
vec2 gfx_get_mouse_delta();

bool check_point_in_rect(vec2 point, rect rect);
bool check_rectangles_overlap(rect rect1, rect rect2);

bool try_gfx_compile_shader_from_files(const char *vertex_path, const char *fragment_path, GLuint *program);

GLuint gfx_compile_shader(const char *vertex, const char *fragment);

DEFINE_LISTS(vertex, vec2, vec3, vec2I, vec4, color);

static inline vertex vertex_one() {
  return (vertex){{0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}};
}

static inline vec2 vec2_one() { return (vec2){1.0f, 1.0f}; }

static inline vec3 vec3_one() { return (vec3){1.0f, 1.0f, 1.0f}; }

static inline vec2I vec2I_one() { return (vec2I){1, 1}; }

static inline vec4 vec4_one() { return (vec4){1.0f, 1.0f, 1.0f, 1.0f}; }

static inline color color_one() { return (color){255, 255, 255, 255}; }

/*  Draw commands  */
void draw_rectangle(rect rect, color color);

#endif