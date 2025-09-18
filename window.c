
#include "window.h"
#include "bitset.h"
#include "gfx.h"
#include "os.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <dlfcn.h>

#define SCREEN_DIVISION_FACTOR 6
#define FOCUSED_LAYER 100


void update_input_state(GLFWwindow *window, input_state_t *state) {

  int a, b;
  for (int key = 32; key < GLFW_KEY_LAST; ++key) {
    state->keys[key] = glfwGetKey(window, key);
  }
  for (int mb = 0; mb < GLFW_MOUSE_BUTTON_LAST; ++mb) {
    state->mouse_buttons[mb] = glfwGetMouseButton(window, mb);
  }
  double mx, my;
  glfwGetCursorPos(window, &mx, &my);
  state->mouse_position = (vec2){mx, my};
}

void update_hit_mask(procman_t *procman) {
  bitset_clear_all(&procman->hit_mask);
  for (int i = 0; i < procman->process_count; ++i) {
    process_t *process = &procman->processes[i];
    if ((process->flags & PROCESS_HAS_WINDOW) == 0)
      continue;

    window_t *window = &process->window;
    if (window->flags & WINDOW_SHOWN) {
      auto range_x = window->bounds.x + window->bounds.width;
      auto range_y = window->bounds.y + window->bounds.height;
      for (int x = window->bounds.x; x < range_x; ++x)
        for (int y = window->bounds.y; y < range_y; ++y) {
          int8_t current_value = bitset_get(&procman->hit_mask, x, y);
          bool should_set_mask_bit = current_value == BITSET_UNSET_VALUE;

          if (!should_set_mask_bit) {
            process_t *other_proc = &procman->processes[current_value];
            if ((other_proc->flags & PROCESS_HAS_WINDOW) == 0)
              continue;
            window_t *current_window = &other_proc->window;
            should_set_mask_bit = current_window->layer > window->layer || window->focused;
          }

          if (should_set_mask_bit)
            bitset_set(&procman->hit_mask, x, y, i);
        }
    }
  }
}

void update_focused_window(procman_t *procman) {
  vec2 mouse_pos = gfx_get_mouse_position();

  int8_t proc_idx = bitset_get(&procman->hit_mask, mouse_pos.x, mouse_pos.y);

  if (proc_idx >= 0 && proc_idx <= 127) {
    process_t *process = &procman->processes[proc_idx];
    if ((process->flags & PROCESS_HAS_WINDOW) == 0) {
      return;
    }
    window_t *window = &process->window;
    vec2 delta = gfx_get_mouse_delta();

    if (check_point_in_rect(mouse_pos, window->bounds)) {

      bool lmb = is_key_down(window->input_state, GLFW_MOUSE_BUTTON_LEFT), 
           rmb = is_key_down(window->input_state, GLFW_MOUSE_BUTTON_RIGHT);

      bool alt = is_key_down(window->input_state, GLFW_MOD_ALT);

      if (lmb || rmb) {
        // unfocus all windows.
        for (int i = 0; i < procman->process_count; ++i) {
          process_t *other_proc = &procman->processes[i];
          if ((other_proc->flags & PROCESS_HAS_WINDOW) == 0) {
            continue;
          }
          other_proc->window.focused = false;
        }
        window->focused = true;
      }
      if (alt && lmb) {
        window->focused = true;
        window->bounds.x += delta.x;
        window->bounds.y += delta.y;
      } else if (alt && rmb) {
        window->bounds.width += delta.x;
        window->bounds.height += delta.y;
      }
    }
  }
}

rect get_next_free_window_bounds(procman_t *procman) {
  bool found_position = false;
  vec2 pos = {10, 10};
  
  vec2 win_size = gfx_window_size_f();

  rect rect = {pos.x, pos.y, win_size.x / SCREEN_DIVISION_FACTOR,
                    win_size.y / SCREEN_DIVISION_FACTOR};

  while (!found_position) {
    found_position = true;
    for (int i = 0; i < procman->process_count; ++i) {
      process_t *process = &procman->processes[i];
      if ((process->flags & PROCESS_HAS_WINDOW) == 0) {
        continue;
      }
      window_t *o = &process->window;
      if (check_rectangles_overlap(rect, o->bounds)) {
        pos.x = o->bounds.x + o->bounds.width + o->border_thickness;
        if (pos.x + win_size.x / SCREEN_DIVISION_FACTOR > win_size.x) {
          pos.x = 10;
          pos.y += win_size.y / SCREEN_DIVISION_FACTOR + o->border_thickness;
        }
        rect.x = pos.x;
        rect.y = pos.y;
        found_position = false;
        break;
      }
    }
  }
  return rect;
}

void draw_window(struct process_t *proc, window_t *window) {
  if ((window->flags & WINDOW_SHOWN) == 0) {
    return;
  }

  auto x = window->bounds.x;
  auto y = window->bounds.y - Y_PADDING;
  auto half_width = window->bounds.width / 2;

  // auto text_w = MeasureText(window->title, FONT_SIZE) / 2.0;
  // Rectangle title_rect = {x, y, window->bounds.width, Y_PADDING_TITLE};
  // DrawRectangleRounded(title_rect, TITLE_BAR_ROUNDNESS, SUBDIVISIONS, window->border_color);
  // DrawText(window->title, window->bounds.x + half_width - text_w, y, FONT_SIZE, BLACK);

  // If the user decided to kill the application, this is how we do it.
  if (window->should_close) {
    window->flags &= ~WINDOW_SHOWN;
    return;
  }

  // Close button // TODO: FIX ME.. Shouldn't be here, and it looks
  // TERRIBLE.
  rect closeButton = {window->bounds.x + window->bounds.width - 12,
                           window->bounds.y - Y_PADDING_TITLE - 1, 9, 9};

  if (check_point_in_rect(gfx_get_mouse_position(), closeButton)) {

    if (is_key_pressed(window->input_state, GLFW_MOUSE_BUTTON_LEFT)) {
      window->flags &= ~WINDOW_SHOWN;
      return;
    }

    // DrawRectangleRounded(closeButton, 0.5, SUBDIVISIONS, WHITE);
    // DrawText("X", closeButton.x + 2, closeButton.y - 1, FONT_SIZE, BLACK);
  } else {
    // DrawRectangleRounded(closeButton, 0.5, SUBDIVISIONS, RED);
    // DrawText("X", closeButton.x + 2, closeButton.y - 1, FONT_SIZE, WHITE);
  }

  // DrawRectangleRoundedLinesEx(window->bounds, WINDOW_BORDER_ROUNDNESS, SUBDIVISIONS,
                              // window->border_thickness, window->border_color);
}

void init_window(process_t *proc, int w, int h, const char *title) {
  window_t *window = &proc->window;
  *window = (window_t){0};
  window->input_state = &proc->procman->input_state;
  window->bounds = get_next_free_window_bounds(proc->procman);
  window->title = title;
  proc->flags |= PROCESS_HAS_WINDOW;
  window->flags |= WINDOW_SHOWN;
  window->border_thickness = 3;
  window->border_color = LIGHT_GRAY;
  window->layer = 0;
  window->focused = false;
  window->should_close = false;
}
