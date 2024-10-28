#ifndef WINMAN_H
#define WINMAN_H

#include <raylib.h>

#include <dlfcn.h>
#include <raylib.h>
#include <raymath.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static FilePathList load_app_database() { return LoadDirectoryFiles("./user"); }

static inline int64_t clampi(int64_t v, int64_t min, int64_t max) {
  if (v < min)
    return min;
  if (v > max)
    return max;
  return v;
}

static inline double clampf(double v, double min, double max) {
  if (v < min)
    return min;
  if (v > max)
    return max;
  return v;
}

static inline double minf(double a, double b) { return a < b ? a : b; }

typedef enum Shape {
  DRAW_RECTANGLE,
  DRAW_CIRCLE,
  DRAW_TRIANGLE,
} Shape;

typedef struct rl_draw_command {
  Shape shape;
  Color color;
  Vector2 position;
  union {
    struct {
      Vector2 size;
    }; // triangle, rectangle
    struct {
      float radius;
    }; // circles.
  };
} rl_draw_command_t;

typedef struct {
  bool keys[349]; // NOT ALL VALUES IN HERE POINT TO A KEY. USE RAYLIB MACROS
  bool mouse_buttons[3];
  Vector2 mouse_position;
  Vector2 mouse_wheel;

} input_state_t;

static void update_input_state(input_state_t *state);

struct window_t;
typedef void (*window_callback)(struct window_t *);

typedef enum WindowFlags {
  WINDOW_HIDDEN = 1 << 0,
  WINDOW_SHOWN = 1 << 1,
  WINDOW_FULLSCREEN = 1 << 2,
} WindowFlags;

typedef struct window_t {
  Rectangle bounds;  // Do not set this, this is set by the window manager
  const char *title; // feel free to modify this.
  bool should_close; // This is set by the window manager, but can be set to
                     // force close.
  // bool visible;

  // Theming
  Color border_color;
  float border_thickness;

  rl_draw_command_t draw_command_stack[2048];
  int stack_length;
  int layer;
  bool focused;

  // .so stuff
  void *dl_handle;
  window_callback init, update, deinit;
  input_state_t *input_state;
  int flags;
} window_t;

static void draw_rectangle(window_t *window, Color color, Vector2 position,
                           Vector2 size) {
  if (window->stack_length < 2048) {
    rl_draw_command_t cmd = {
        .shape = DRAW_RECTANGLE,
        .color = color,
        .position = position,
        .size = size,
    };
    window->draw_command_stack[window->stack_length++] = cmd;
  }
}
static void draw_circle(window_t *window, Color color, Vector2 position,
                        float radius) {
  if (window->stack_length < 2048) {
    rl_draw_command_t cmd = {
        .shape = DRAW_CIRCLE,
        .color = color,
        .position = position,
        .radius = radius,
    };
    window->draw_command_stack[window->stack_length++] = cmd;
  }
}
static void draw_triangle(window_t *window, Color color, Vector2 position,
                          Vector2 size) {
  if (window->stack_length < 2048) {
    rl_draw_command_t cmd = {
        .shape = DRAW_TRIANGLE,
        .color = color,
        .position = position,
        .size = size,
    };
    window->draw_command_stack[window->stack_length++] = cmd;
  }
}

#define BITSET_UNSET_VALUE -1

typedef struct bitset_t {
  uint64_t *values;
  int width, height;
} bitset_t;

static inline bitset_t new_bitset(int width, int height) {
  int n_bytes = width * height;
  int n_values = (n_bytes + 7) / 8;
  bitset_t mask = {
    .width = width,
    .height = height,
    .values = calloc(n_values, sizeof(uint64_t)),
  };
  return mask;
}

static inline void bitset_clear_all(bitset_t *mask) {
  int n_values = (mask->width * mask->height + 7) / 8;
  memset(mask->values, BITSET_UNSET_VALUE, n_values * sizeof(uint64_t));
}

static inline void free_bitset(bitset_t *mask) { free(mask->values); }

static inline int8_t bitset_get(bitset_t *mask, int x, int y) {
  if (x < 0 || x >= mask->width || y < 0 || y >= mask->height)  return 0;
  int index = y * mask->width + x;
  int ind_index = index / 8;
  int offset = (index % 8) * 8;
  return (mask->values[ind_index] >> offset) & 0xFF;
}

static inline void bitset_set(bitset_t *mask, int x, int y, int8_t value) {
  if (x < 0 || x >= mask->width || y < 0 || y >= mask->height) return;
  int index = y * mask->width + x;
  int int_idx = index / 8;
  int offset = (index % 8) * 8;
  // clear
  mask->values[int_idx] &= ~(0xFFULL << offset);
  // set
  mask->values[int_idx] |= ((uint64_t)value << offset);
}

static inline void bitset_clear(bitset_t *mask, int x, int y) {
  if (x < 0 || x >= mask->width || y < 0 || y >= mask->height) return;
  int index = y * mask->width + x;
  int int_idx = index / 8;
  int offset = (index % 8) * 8;
  mask->values[int_idx] &= ~(0xFFULL << offset); // Clear the value
}

typedef struct winman_t {
  window_t windows[48];
  int window_count;
  FilePathList app_database;
  input_state_t input_state;
  bitset_t hit_mask;
} winman_t;

window_t new_window(struct winman_t *winman);
void _internal_draw_all_commands_window(window_t *window);
winman_t new_winman();
void load_app(winman_t *winman, const char *path);

void update_winman(winman_t *winman);
void draw_winman(winman_t *winman);

static bool is_key_down(input_state_t *state, int key) {
  return state->keys[key];
}

static bool is_key_pressed(input_state_t *state, int key) {
  static bool previous_keys[349] = {0};
  bool pressed = state->keys[key] && !previous_keys[key];
  previous_keys[key] = state->keys[key];
  return pressed;
}


#endif
