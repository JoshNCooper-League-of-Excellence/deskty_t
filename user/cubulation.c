
#include <raylib.h>

void init(window_t* window) {
  window->title = "This is my title.";
}

Rectangle cubulations[24];

void update(window_t* window) {
  static Vector2 pos = {0, 0};

  for (int i = 0; i < 24; ++i) {
    Rectangle cubulator = cubulations[i];
    draw_rectangle(window, WHITE, (Vector2){cubulator.x, cubulator.y}, (Vector2){cubulator.width, cubulator.height});
  }
}

void deinit() {


}
