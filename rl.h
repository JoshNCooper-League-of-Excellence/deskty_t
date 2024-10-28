#ifndef RL_H
#define RL_H
#include <raylib.h>
#include <raymath.h>
#include <stdint.h>

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

static inline void update_input_state(input_state_t *state) {
  state->keys[KEY_APOSTROPHE] = IsKeyDown(KEY_APOSTROPHE);
  state->keys[KEY_COMMA] = IsKeyDown(KEY_COMMA);
  state->keys[KEY_MINUS] = IsKeyDown(KEY_MINUS);
  state->keys[KEY_PERIOD] = IsKeyDown(KEY_PERIOD);
  state->keys[KEY_SLASH] = IsKeyDown(KEY_SLASH);
  state->keys[KEY_ZERO] = IsKeyDown(KEY_ZERO);
  state->keys[KEY_ONE] = IsKeyDown(KEY_ONE);
  state->keys[KEY_TWO] = IsKeyDown(KEY_TWO);
  state->keys[KEY_THREE] = IsKeyDown(KEY_THREE);
  state->keys[KEY_FOUR] = IsKeyDown(KEY_FOUR);
  state->keys[KEY_FIVE] = IsKeyDown(KEY_FIVE);
  state->keys[KEY_SIX] = IsKeyDown(KEY_SIX);
  state->keys[KEY_SEVEN] = IsKeyDown(KEY_SEVEN);
  state->keys[KEY_EIGHT] = IsKeyDown(KEY_EIGHT);
  state->keys[KEY_NINE] = IsKeyDown(KEY_NINE);
  state->keys[KEY_SEMICOLON] = IsKeyDown(KEY_SEMICOLON);
  state->keys[KEY_EQUAL] = IsKeyDown(KEY_EQUAL);
  state->keys[KEY_A] = IsKeyDown(KEY_A);
  state->keys[KEY_B] = IsKeyDown(KEY_B);
  state->keys[KEY_C] = IsKeyDown(KEY_C);
  state->keys[KEY_D] = IsKeyDown(KEY_D);
  state->keys[KEY_E] = IsKeyDown(KEY_E);
  state->keys[KEY_F] = IsKeyDown(KEY_F);
  state->keys[KEY_G] = IsKeyDown(KEY_G);
  state->keys[KEY_H] = IsKeyDown(KEY_H);
  state->keys[KEY_I] = IsKeyDown(KEY_I);
  state->keys[KEY_J] = IsKeyDown(KEY_J);
  state->keys[KEY_K] = IsKeyDown(KEY_K);
  state->keys[KEY_L] = IsKeyDown(KEY_L);
  state->keys[KEY_M] = IsKeyDown(KEY_M);
  state->keys[KEY_N] = IsKeyDown(KEY_N);
  state->keys[KEY_O] = IsKeyDown(KEY_O);
  state->keys[KEY_P] = IsKeyDown(KEY_P);
  state->keys[KEY_Q] = IsKeyDown(KEY_Q);
  state->keys[KEY_R] = IsKeyDown(KEY_R);
  state->keys[KEY_S] = IsKeyDown(KEY_S);
  state->keys[KEY_T] = IsKeyDown(KEY_T);
  state->keys[KEY_U] = IsKeyDown(KEY_U);
  state->keys[KEY_V] = IsKeyDown(KEY_V);
  state->keys[KEY_W] = IsKeyDown(KEY_W);
  state->keys[KEY_X] = IsKeyDown(KEY_X);
  state->keys[KEY_Y] = IsKeyDown(KEY_Y);
  state->keys[KEY_Z] = IsKeyDown(KEY_Z);
  state->keys[KEY_LEFT_BRACKET] = IsKeyDown(KEY_LEFT_BRACKET);
  state->keys[KEY_BACKSLASH] = IsKeyDown(KEY_BACKSLASH);
  state->keys[KEY_RIGHT_BRACKET] = IsKeyDown(KEY_RIGHT_BRACKET);
  state->keys[KEY_GRAVE] = IsKeyDown(KEY_GRAVE);
  state->keys[KEY_SPACE] = IsKeyDown(KEY_SPACE);
  state->keys[KEY_ESCAPE] = IsKeyDown(KEY_ESCAPE);
  state->keys[KEY_ENTER] = IsKeyDown(KEY_ENTER);
  state->keys[KEY_TAB] = IsKeyDown(KEY_TAB);
  state->keys[KEY_BACKSPACE] = IsKeyDown(KEY_BACKSPACE);
  state->keys[KEY_INSERT] = IsKeyDown(KEY_INSERT);
  state->keys[KEY_DELETE] = IsKeyDown(KEY_DELETE);
  state->keys[KEY_RIGHT] = IsKeyDown(KEY_RIGHT);
  state->keys[KEY_LEFT] = IsKeyDown(KEY_LEFT);
  state->keys[KEY_DOWN] = IsKeyDown(KEY_DOWN);
  state->keys[KEY_UP] = IsKeyDown(KEY_UP);
  state->keys[KEY_PAGE_UP] = IsKeyDown(KEY_PAGE_UP);
  state->keys[KEY_PAGE_DOWN] = IsKeyDown(KEY_PAGE_DOWN);
  state->keys[KEY_HOME] = IsKeyDown(KEY_HOME);
  state->keys[KEY_END] = IsKeyDown(KEY_END);
  state->keys[KEY_CAPS_LOCK] = IsKeyDown(KEY_CAPS_LOCK);
  state->keys[KEY_SCROLL_LOCK] = IsKeyDown(KEY_SCROLL_LOCK);
  state->keys[KEY_NUM_LOCK] = IsKeyDown(KEY_NUM_LOCK);
  state->keys[KEY_PRINT_SCREEN] = IsKeyDown(KEY_PRINT_SCREEN);
  state->keys[KEY_PAUSE] = IsKeyDown(KEY_PAUSE);
  state->keys[KEY_F1] = IsKeyDown(KEY_F1);
  state->keys[KEY_F2] = IsKeyDown(KEY_F2);
  state->keys[KEY_F3] = IsKeyDown(KEY_F3);
  state->keys[KEY_F4] = IsKeyDown(KEY_F4);
  state->keys[KEY_F5] = IsKeyDown(KEY_F5);
  state->keys[KEY_F6] = IsKeyDown(KEY_F6);
  state->keys[KEY_F7] = IsKeyDown(KEY_F7);
  state->keys[KEY_F8] = IsKeyDown(KEY_F8);
  state->keys[KEY_F9] = IsKeyDown(KEY_F9);
  state->keys[KEY_F10] = IsKeyDown(KEY_F10);
  state->keys[KEY_F11] = IsKeyDown(KEY_F11);
  state->keys[KEY_F12] = IsKeyDown(KEY_F12);
  state->keys[KEY_LEFT_SHIFT] = IsKeyDown(KEY_LEFT_SHIFT);
  state->keys[KEY_LEFT_CONTROL] = IsKeyDown(KEY_LEFT_CONTROL);
  state->keys[KEY_LEFT_ALT] = IsKeyDown(KEY_LEFT_ALT);
  state->keys[KEY_LEFT_SUPER] = IsKeyDown(KEY_LEFT_SUPER);
  state->keys[KEY_RIGHT_SHIFT] = IsKeyDown(KEY_RIGHT_SHIFT);
  state->keys[KEY_RIGHT_CONTROL] = IsKeyDown(KEY_RIGHT_CONTROL);
  state->keys[KEY_RIGHT_ALT] = IsKeyDown(KEY_RIGHT_ALT);
  state->keys[KEY_RIGHT_SUPER] = IsKeyDown(KEY_RIGHT_SUPER);
  state->keys[KEY_KB_MENU] = IsKeyDown(KEY_KB_MENU);
  state->keys[KEY_KP_0] = IsKeyDown(KEY_KP_0);
  state->keys[KEY_KP_1] = IsKeyDown(KEY_KP_1);
  state->keys[KEY_KP_2] = IsKeyDown(KEY_KP_2);
  state->keys[KEY_KP_3] = IsKeyDown(KEY_KP_3);
  state->keys[KEY_KP_4] = IsKeyDown(KEY_KP_4);
  state->keys[KEY_KP_5] = IsKeyDown(KEY_KP_5);
  state->keys[KEY_KP_6] = IsKeyDown(KEY_KP_6);
  state->keys[KEY_KP_7] = IsKeyDown(KEY_KP_7);
  state->keys[KEY_KP_8] = IsKeyDown(KEY_KP_8);
  state->keys[KEY_KP_9] = IsKeyDown(KEY_KP_9);
  state->keys[KEY_KP_DECIMAL] = IsKeyDown(KEY_KP_DECIMAL);
  state->keys[KEY_KP_DIVIDE] = IsKeyDown(KEY_KP_DIVIDE);
  state->keys[KEY_KP_MULTIPLY] = IsKeyDown(KEY_KP_MULTIPLY);
  state->keys[KEY_KP_SUBTRACT] = IsKeyDown(KEY_KP_SUBTRACT);
  state->keys[KEY_KP_ADD] = IsKeyDown(KEY_KP_ADD);
  state->keys[KEY_KP_ENTER] = IsKeyDown(KEY_KP_ENTER);
  state->keys[KEY_KP_EQUAL] = IsKeyDown(KEY_KP_EQUAL);
  state->keys[KEY_BACK] = IsKeyDown(KEY_BACK);
  state->keys[KEY_MENU] = IsKeyDown(KEY_MENU);
  state->keys[KEY_VOLUME_UP] = IsKeyDown(KEY_VOLUME_UP);
  state->keys[KEY_VOLUME_DOWN] = IsKeyDown(KEY_VOLUME_DOWN);

  for (int i = 0; i < 3; ++i) {
    state->mouse_buttons[i] = IsMouseButtonDown(i);
  }

  state->mouse_position = GetMousePosition();
}

struct window_t;
typedef void (*window_callback)(struct window_t *);

typedef struct window_t {
  Rectangle boundary; // Do not set this, this is set by the window manager
  Vector2 position;   // Do not set this, this is set by the window manager
  const char *title;  // feel free to modify this.
  bool should_close;  // This is set by the window manager, but can be set to
                      // force close.
  // bool visible;

  // Theming
  Color border_color;
  float border_thickness;

  rl_draw_command_t draw_command_stack[2048];
  int stack_length;
  int layer;

  // .so stuff
  void *dl_handle;
  window_callback init, update, deinit;

  input_state_t *input_state;
} window_t;

static window_t new_window() {
  window_t window = {0};
  window.boundary = (Rectangle){0, 0, 800.0 / 6, 600.0 / 6};
  window.position = (Vector2){24, 24};
  window.border_thickness = 2.0;
  window.border_color = RAYWHITE;
  return window;
}
static void window_internal_draw_all_commands(window_t *window) {
  BeginScissorMode(window->boundary.x, window->boundary.y, window->boundary.width, window->boundary.height);
  while (window->stack_length > 0) {
    auto command = window->draw_command_stack[--window->stack_length];

    // put into window space.
    command.position = Vector2Add(command.position, (Vector2){window->boundary.x, window->boundary.y});

    switch (command.shape) {
    case DRAW_RECTANGLE:
      DrawRectangleV(command.position, command.size, command.color);
      break;
    case DRAW_CIRCLE:
      DrawCircleV(command.position, command.radius, command.color);
      break;
    case DRAW_TRIANGLE:
      Vector2 v1 = {command.position.x, command.position.y};
      Vector2 v2 = {command.position.x + command.size.x, command.position.y};
      Vector2 v3 = {command.position.x, command.position.y + command.size.y};
      DrawTriangle(v1, v2, v3, command.color);
      break;
    }
  }
  EndScissorMode();
}

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

#endif
