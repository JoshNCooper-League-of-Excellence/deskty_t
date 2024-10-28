
#include <raylib.h>
#include "../winman.h"

void init(window_t* window) {
  window->title = "Cubes and Cubulations";
}

Rectangle cubulations[24];
Vector2 velocities[24];

void update(window_t* window) {
  for (int i = 0; i < 24; ++i) {
    Rectangle* cubulator = &cubulations[i];
    Vector2* velocity = &velocities[i];

    // Update position
    cubulator->x += velocity->x;
    cubulator->y += velocity->y;

    if (cubulator->x <= 0 || cubulator->x + cubulator->width >= window->bounds.width) {
      velocity->x *= -1;
    }
    if (cubulator->y <= 0 || cubulator->y + cubulator->height >= window->bounds.height) {
      velocity->y *= -1;
    }

    draw_rectangle(window, GREEN, (Vector2){cubulator->x, cubulator->y}, (Vector2){cubulator->width, cubulator->height});
  }
}

void deinit() {


}
