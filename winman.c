
#include "winman.h"
#include <stdio.h>
#include <stdlib.h>

window_t new_window(winman_t *winman) {
  window_t window = {0};
  window.boundary = (Rectangle){24, 24, 800.0 / 6, 600.0 / 6};
  window.border_thickness = 2.0;
  window.border_color = RAYWHITE;
  return window;
}

void load_app(winman_t *winman, const char *path) {
  window_t window = new_window(winman);
  window.title = path;

  char so_path[256];
  char compile_command[512];

  // Get the output path, and format a system call to compile the .so
  {
    const char *name = GetFileNameWithoutExt(path);
    snprintf(so_path, sizeof(so_path), "binaries/%s.so", name);

#define COMPILE_COMMAND_FORMAT "clang -std=c23 -shared -fPIC -o %s %s"
    int required_length =
        snprintf(nullptr, 0, COMPILE_COMMAND_FORMAT, so_path, path);
    snprintf(compile_command, required_length + 1, COMPILE_COMMAND_FORMAT,
             so_path, path);

#if DEBUG
    printf("app path :: %s\n", path);
    printf("app name :: %s\n", name);
    printf("app .so  :: %s\n", so_path);
#endif
  }

  printf("Compiling... %s :: %s\n", so_path, compile_command);
  int result = system(compile_command);

  if (result != 0) {
    printf("Failed to compile %s.. compiler returned %d\n", path, result);
    return;
  }

  window.dl_handle = dlopen(so_path, RTLD_NOW);

  if (!window.dl_handle) {
    printf("Unable to load shared object file! %s\n", so_path);
    return;
  }

  window.init = (window_callback)dlsym(window.dl_handle, "init");
  window.update = (window_callback)dlsym(window.dl_handle, "update");
  window.deinit = (window_callback)dlsym(window.dl_handle, "deinit");

  if (!window.init || !window.update || !window.deinit) {
    printf("Unable to find init, update, or deinit functions in %s\n", so_path);
    printf("functions: init=%p, update=%p, deinit=%p\n", window.init,
           window.update, window.deinit);
    return;
  }

  // compile the binary to an so, put it in binaries/name.so ✅
  // load the so. ✅
  // get the init, update, and deinit function pointers. ✅

  window.init(&window);
  winman->windows[winman->window_count++] = window;
};
void draw_winman(winman_t *winman) {
  update_input_state(&winman->input_state);
  int layer = 0;
  int drawn_count = 0;
  while (drawn_count < winman->window_count) {
    for (int i = 0; i < winman->window_count; ++i) {
      window_t *window = &winman->windows[i];
      if (window->layer == layer) {

        // draw title bar.
        {
          auto x = window->boundary.x;
          auto y = window->boundary.y - 10;
          DrawRectangleRounded((Rectangle){x, y, window->boundary.width, 8},
                               0.1, 6, window->border_color);
          DrawText(window->title,
                   window->boundary.x + (window->boundary.width / 2) -
                       (MeasureText(window->title, 5) / 2.0),
                   y, 5, BLACK);
        }

        DrawRectangleRoundedLinesEx(window->boundary, 0.05, 6,
                                    window->border_thickness,
                                    window->border_color);
        window->input_state = &winman->input_state;

        {
          // Close button
          Rectangle closeButton = {window->boundary.x + window->boundary.width -
                                       15,
                                   window->boundary.y - 15, 10, 10};
          DrawRectangleRec(closeButton, RED);
          if (CheckCollisionPointRec(GetMousePosition(), closeButton) &&
              IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            printf("Start closing window\n");
            for (int j = i; j < winman->window_count - 1; ++j) {
              winman->windows[j] = winman->windows[j + 1];
            }
            printf("Closed window\n");
            winman->window_count--;
            continue;
          }

          // Dragging
          if (CheckCollisionPointRec(GetMousePosition(),
                                     (Rectangle){window->boundary.x,
                                                 window->boundary.y - 10,
                                                 window->boundary.width, 10})) {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
              window->boundary.x += GetMouseDelta().x;
              window->boundary.y += GetMouseDelta().y;
            }

            // Resizing with Alt key
            if (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)) {
              if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                window->boundary.width += GetMouseDelta().x;
                window->boundary.height += GetMouseDelta().y;
              }
            }
          }
        }

        window->update(window);
        window_internal_draw_all_commands(window);
        drawn_count++;
      }
    }
    layer++;
  }
}
winman_t new_winman() {
  winman_t winman = {0};
  winman.app_database = load_app_database();
  return winman;
}
void window_internal_draw_all_commands(window_t *window) {
  BeginScissorMode(window->boundary.x, window->boundary.y,
                   window->boundary.width, window->boundary.height);
  while (window->stack_length > 0) {
    auto command = window->draw_command_stack[--window->stack_length];

    // put into window space.
    command.position = Vector2Add(
        command.position, (Vector2){window->boundary.x, window->boundary.y});

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
static void update_input_state(input_state_t *state) {
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
