#include "../os.h"

void init(process_t *proc) { proc->init_window(proc, 200, 200, "boxer!"); }

void update(process_t *proc) {
  window_t *window = &proc->window;
  Vector2 screen_size = {window->bounds.width, window->bounds.height};
  draw_rectangle(window, BLACK, (Vector2){0, 0}, screen_size);
  draw_rectangle(window, WHITE, (Vector2){90, 90}, (Vector2){20, 20});
}

void deinit(process_t *proc) {}
