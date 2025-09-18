#include "../os.h"
#include <inttypes.h>

/*
  A small example of opening and drawing to a window.
*/
void init(process_t *process) { process->init_window(process, 20, 20, "My Window!"); }

struct Player {
  Vector2 position;
  Vector2 velocity;
  Color color;
} player = {(Vector2){0, 0}, (Vector2){0, 0}, BLACK};

static void draw_player(window_t *window, Vector2 screen_size) {
  player.velocity.y += 0.5;

  screen_size.y -= 50;

  if (is_key_pressed(window->input_state, GLFW_KEY_SPACE) && player.position.y >= screen_size.y) {
    player.velocity.y = -10;
  }

  if (is_key_down(window->input_state, GLFW_KEY_A)) {
    player.velocity.x = -5;
  } else if (is_key_down(window->input_state, GLFW_KEY_D)) {
    player.velocity.x = 5;
  }

  player.position.x += player.velocity.x;
  player.position.y += player.velocity.y;

  player.velocity.x *= 0.98;
  player.velocity.y *= 0.98;

  if (player.position.x < 0) {
    player.position.x = 0;
    player.velocity.x = 0;
  } else if (player.position.x > screen_size.x - 50) {
    player.position.x = screen_size.x - 50;
    player.velocity.x = 0;
  }

  if (player.position.y > screen_size.y) {
    player.position.y = screen_size.y;
    player.velocity.y = 0;
  }
}

void update(process_t *process) {
  window_t *window = &process->window;
  Vector2 screen_size = {window->bounds.width, window->bounds.height};
  draw_player(window, screen_size);
}

void deinit(process_t *process) {}
