
#include "../winman.h"
#include <raylib.h>

void init(window_t* window) {
  window->title = "Cubular Mode.";
}

void update(window_t* window) {
  static Vector2 pos = {0, 0};
  draw_rectangle(window, WHITE, pos, (Vector2){24, 24});

  if (window->input_state->keys[KEY_W] == true) {
    pos.y += 1;
  }
  if (window->input_state->keys[KEY_S] == true) {
    pos.y -= 1;
  }
  if (window->input_state->keys[KEY_A] == true) {
    pos.x -= 1;
  }
  if (window->input_state->keys[KEY_D] == true) {
    pos.x += 1;
  }

}

void deinit() {


}