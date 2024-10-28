
#include "../winman.h"
#include <raylib.h>

void init(window_t* window) {
  window->title = "Cubular Mode.";
}

void update(window_t* window) {
  static Vector2 pos = {0, 0};
  static float speed = 1.0;
  draw_rectangle(window, WHITE, pos, (Vector2){24, 24});

  if (is_key_pressed(window->input_state, KEY_UP)) {
    speed += 1.0;
  } else if (is_key_pressed(window->input_state, KEY_DOWN)) {
    speed -= 1.0;
  }

  if (window->input_state->keys[KEY_W] == true) {
    pos.y += speed;
  }
  if (window->input_state->keys[KEY_S] == true) {
    pos.y -= speed;
  }
  if (window->input_state->keys[KEY_A] == true) {
    pos.x -= speed;
  }
  if (window->input_state->keys[KEY_D] == true) {
    pos.x += speed;
  }

}

void deinit() {


}
