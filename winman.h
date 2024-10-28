#ifndef WINMAN_H
#define WINMAN_H

#include "rl.h"
#include <raylib.h>

static FilePathList load_app_database() { return LoadDirectoryFiles("./user"); }

typedef struct {
  window_t windows[24];
  int window_count;
  FilePathList app_database;
  input_state_t input_state;
} winman_t;

static inline winman_t new_winman() {
  winman_t winman = {0};
  winman.app_database = load_app_database();
  return winman;
}

static inline void draw_winman(winman_t *winman) {
  update_input_state(&winman->input_state);
  int layer = 0;
  int drawn_count = 0;
  while (drawn_count < winman->window_count) {
    for (int i = 0; i < winman->window_count; ++i) {
      window_t window = winman->windows[i];
      if (window.layer == layer) {
        window.boundary.x = window.position.x;
        window.boundary.y = window.position.y;
        DrawRectangleRoundedLinesEx(window.boundary, 0.05, 6,
                                    window.border_thickness,
                                    window.border_color);
        window.input_state = &winman->input_state;
        window.update(&window);
        window_internal_draw_all_commands(&window);
        drawn_count++;
      }
    }
    layer++;
  }
}

#endif
