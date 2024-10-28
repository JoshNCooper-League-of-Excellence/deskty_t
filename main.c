#include "winman.h"
#include <raylib.h>
#include <stdio.h>

int main(void) {
  InitWindow(800, 600, "Window Manager");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  winman_t winman = new_winman();

  for (int i = 0; i < winman.app_database.count; ++i) {
    load_app(&winman, winman.app_database.paths[i]);
    printf("Loaded app %s\n", winman.app_database.paths[i]);
  }

  SetTargetFPS(60);
  int INDEX = 0;
  while(!WindowShouldClose()) {
    if (IsKeyDown(KEY_P)) {
      load_app(&winman, winman.app_database.paths[(INDEX += 1) % (winman.app_database.count - 1)]);
    }

    BeginDrawing();
    ClearBackground(BLACK);
    DrawFPS(0, 0);
    update_winman(&winman);
    draw_winman(&winman);
    EndDrawing();
  }

  for (int i = 0; i < winman.window_count; ++i) {
    window_t window = winman.windows[i];
    window.deinit(&window);
  }

}
