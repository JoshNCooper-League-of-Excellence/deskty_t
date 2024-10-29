#ifndef WINMAN_H
#define WINMAN_H

#include <raylib.h>
#include <dlfcn.h>
#include <raylib.h>
#include <raymath.h>
#include <stdint.h>


// These are constants used for drawing the window, They are temporary, and
// should later be replaced with an extensible styling system.
#define FONT_SIZE 5
#define Y_PADDING 10
#define TITLE_BAR_ROUNDNESS 0.3
#define WINDOW_BORDER_ROUNDNESS 0.05
#define SUBDIVISIONS 6
#define Y_PADDING_TITLE 8


#define clamp(v, min, max) ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))
#define min(a, b) ((a) < (b) ? (a) : (b))

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
  // NOT ALL VALUES IN HERE POINT TO A KEY. USE RAYLIB MACROS TO GET KEYS
  bool keys[349];
  bool mouse_buttons[3];
  Vector2 mouse_position;
  Vector2 mouse_wheel;
} input_state_t;



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

  // Theming, todo: make a theming system that's extensible
  Color border_color;
  float border_thickness;

  rl_draw_command_t draw_command_stack[2048]; // TODO: we shouldn't treat this like a stack, otherwise
  // things drawn last by the app will appear on bottom.
  int stack_length;

  int layer;
  bool focused;
  int flags;

  // for reading input.
  input_state_t *input_state;
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

void update_input_state(input_state_t *state);

struct process_t;

window_t *init_window(struct process_t *proc, int w, int h, const char *title);

void draw_window(struct process_t *proc, window_t *window);
void free_window(window_t *window);

#define WIN_RECT                                                               \
  {pos.x, pos.y, screen_size.x / SCREEN_DIVISION_FACTOR,                       \
   screen_size.y / SCREEN_DIVISION_FACTOR}

struct procman_t;

void update_hit_mask(struct procman_t *procman);
Rectangle get_next_free_window_bounds(struct procman_t *);
void update_focused_window(struct procman_t *winman);
void _internal_draw_all_commands_window(window_t *window);
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
