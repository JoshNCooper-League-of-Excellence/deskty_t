#include "os.h"
#include "window.h"
#include <raylib.h>
#include <stdio.h>

// fulfill extern
procman_t procman;

void init_os() {
  procman = new_procman();
}

int main(void) {
  init_os();

  InitWindow(800, 600, "Window Manager");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);

  const char * path = procman.proc_list.paths[0];
  spawn_process(&procman, path);
  spawn_process(&procman, path);

  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawFPS(0, 0);
    update_input_state(&procman.input_state);
    update_hit_mask(&procman);
    update_focused_window(&procman);
    update_procman(&procman);
    EndDrawing();
  }
}
