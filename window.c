
#include "window.h"
#include "bitset.h"
#include "os.h"
#include <dlfcn.h>
#include <raylib.h>

#define SCREEN_DIVISION_FACTOR 6
#define FOCUSED_LAYER 100


// Draw all the commands for a window's draw command stack.
// Scissor cull the commands to the window's bounds.
// Adjust each shape to the window's own pixel space.
void _internal_draw_all_commands_window(window_t *window) {
  Camera2D camera = {0};
  camera.target = (Vector2){0, 0};
  camera.offset = (Vector2){window->bounds.x, window->bounds.y};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  BeginScissorMode(window->bounds.x, window->bounds.y, window->bounds.width, window->bounds.height);
  BeginMode2D(camera);


  for (int i = 0; i < window->stack_length; ++i) {
    rl_draw_command_t command = window->draw_command_stack[i];
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
  window->stack_length = 0;

  EndMode2D(); // End 2D mode with the camera
  EndScissorMode();
}

void update_input_state(input_state_t *state) {
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

void update_hit_mask(procman_t *procman) {
  bitset_clear_all(&procman->hit_mask);
  for (int i = 0; i < procman->process_count; ++i) {
    process_t *process = &procman->processes[i];
    if ((process->flags & PROCESS_HAS_WINDOW) == 0) continue;

    window_t *window = &process->window;
    if (window->flags & WINDOW_SHOWN) {
      auto range_x = window->bounds.x + window->bounds.width;
      auto range_y = window->bounds.y + window->bounds.height;
      for (int x = window->bounds.x; x < range_x; ++x)
        for (int y = window->bounds.y; y < range_y; ++y) {
          int8_t current_value = bitset_get(&procman->hit_mask, x, y);
          bool should_set_mask_bit = current_value == BITSET_UNSET_VALUE;

          // if there's another window there,
          // check if it's above the current one or it's focused
          if (!should_set_mask_bit) {
            process_t *other_proc = &procman->processes[current_value];
            if ((other_proc->flags & PROCESS_HAS_WINDOW) == 0) continue;
            window_t *current_window = &other_proc->window;
            should_set_mask_bit = current_window->layer > window->layer || window->focused;
          }

          // set the mask bit if it's the topmost window
          if (should_set_mask_bit) bitset_set(&procman->hit_mask, x, y, i);
        }
    }
  }
}

void update_focused_window(procman_t *procman) {
  Vector2 mouse_pos = GetMousePosition();
  // check if the mouse is over any window
  int8_t proc_idx = bitset_get(&procman->hit_mask, mouse_pos.x, mouse_pos.y);


  if (proc_idx >= 0 && proc_idx <= 127) {
    process_t *process = &procman->processes[proc_idx];
    if ((process->flags & PROCESS_HAS_WINDOW) == 0) {
      return;
    }
    window_t *window = &process->window;
    Vector2 delta = GetMouseDelta();
    if (CheckCollisionPointRec(GetMousePosition(), window->bounds)) {
      bool lmb = IsMouseButtonDown(MOUSE_LEFT_BUTTON),
           rmb = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
      bool alt = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
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

Rectangle get_next_free_window_bounds(procman_t *procman) {
  bool found_position = false;
  Vector2 pos = {10, 10};
  Vector2 screen_size = {GetScreenWidth(), GetScreenHeight()};
  Rectangle rect = {pos.x, pos.y, screen_size.x / SCREEN_DIVISION_FACTOR,
                    screen_size.y / SCREEN_DIVISION_FACTOR};

  while (!found_position) {
    found_position = true;
    for (int i = 0; i < procman->process_count; ++i) {
      process_t *process = &procman->processes[i];
      if ((process->flags & PROCESS_HAS_WINDOW) == 0) {
        continue;
      }
      window_t *o = &process->window;
      if (CheckCollisionRecs(rect, o->bounds)) {
        pos.x = o->bounds.x + o->bounds.width + o->border_thickness;
        if (pos.x + screen_size.x / SCREEN_DIVISION_FACTOR > screen_size.x) {
          pos.x = 10;
          pos.y += screen_size.y / SCREEN_DIVISION_FACTOR + o->border_thickness;
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
  auto text_w = MeasureText(window->title, FONT_SIZE) / 2.0;
  Rectangle title_rect = {x, y, window->bounds.width, Y_PADDING_TITLE};
  DrawRectangleRounded(title_rect, TITLE_BAR_ROUNDNESS, SUBDIVISIONS,
                       window->border_color);
  DrawText(window->title, window->bounds.x + half_width - text_w, y, FONT_SIZE,
           BLACK);

  // If the user decided to kill the application, this is how we do it.
  if (window->should_close) {
    window->flags &= ~WINDOW_SHOWN;
    return;
  }
  // Close button // TODO: FIX ME.. Shouldn't be here, and it looks
  // TERRIBLE.
  Rectangle closeButton = {window->bounds.x + window->bounds.width - 12,
                           window->bounds.y - Y_PADDING_TITLE - 1, 9, 9};

  if (CheckCollisionPointRec(GetMousePosition(), closeButton)) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      window->flags &= ~WINDOW_SHOWN;
      return;
    }

    DrawRectangleRounded(closeButton, 0.5, SUBDIVISIONS, WHITE);
    DrawText("X", closeButton.x + 2, closeButton.y - 1, FONT_SIZE, BLACK);
  } else {
    DrawRectangleRounded(closeButton, 0.5, SUBDIVISIONS, RED);
    DrawText("X", closeButton.x + 2, closeButton.y - 1, FONT_SIZE, WHITE);
  }

  DrawRectangleRoundedLinesEx(window->bounds, WINDOW_BORDER_ROUNDNESS,
                              SUBDIVISIONS, window->border_thickness,
                              window->border_color);

  _internal_draw_all_commands_window(window);
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
  window->border_color = LIGHTGRAY;
  window->layer = 0;
  window->focused = false;
  window->should_close = false;
}
