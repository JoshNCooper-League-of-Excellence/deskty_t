#include "loader.h"
#include "rl.h"
#include "winman.h"
#include <raylib.h>

int main(void) {
  InitWindow(800, 600, "Window Manager");

  winman_t winman = new_winman();

  winman.windows[winman.window_count++] = load_app(winman.app_database.paths[0]);


  SetTargetFPS(60);
  while(!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawFPS(0, 0);
    draw_winman(&winman);
    EndDrawing();
  }

  for (int i = 0; i < winman.window_count; ++i) {
    window_t window = winman.windows[i];
    window.deinit(&window);
  }

}
