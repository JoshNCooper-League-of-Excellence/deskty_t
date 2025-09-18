#ifndef WINMAN_H
#define WINMAN_H

#include "gfx.h"
#include <dlfcn.h>
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

typedef struct {
  // these have a ton of empty values. use the GLFW_* macros to get the indices for each key.
  bool keys[GLFW_KEY_LAST];
  bool mouse_buttons[GLFW_MOUSE_BUTTON_LAST];
  vec2 mouse_position;
  vec2 mouse_wheel;
} input_state_t;

typedef enum WindowFlags {
  WINDOW_HIDDEN = 1 << 0,
  WINDOW_SHOWN = 1 << 1,
  WINDOW_FULLSCREEN = 1 << 2,
} WindowFlags;

typedef struct window_t {
  rect bounds; // Do not set this, this is set by the window manager

  const char *title; // feel free to modify this.

  bool should_close; // This is set by the window manager, but can be set to
                     // force close.

  color border_color; // Theming, todo: make a theming system that's extensible
  float border_thickness;

  int layer;
  bool focused;
  int flags;

  // for reading input.
  input_state_t *input_state;
} window_t;

void update_input_state(GLFWwindow *window, input_state_t *state);

struct process_t;

void init_window(struct process_t *proc, int w, int h, const char *title);

void draw_window(struct process_t *proc, window_t *window);
void free_window(window_t *window);

#define WIN_RECT                                                                                   \
  {pos.x, pos.y, screen_size.x / SCREEN_DIVISION_FACTOR, screen_size.y / SCREEN_DIVISION_FACTOR}

struct procman_t;

void update_hit_mask(struct procman_t *procman);
rect get_next_free_window_bounds(struct procman_t *);
void update_focused_window(struct procman_t *winman);

static bool is_key_down(input_state_t *state, int key) { return state->keys[key]; }
static bool is_key_pressed(input_state_t *state, int key) {
  static bool previous_keys[GLFW_KEY_LAST] = {0};
  bool pressed = state->keys[key] && !previous_keys[key];
  previous_keys[key] = state->keys[key];
  return pressed;
}

#endif
