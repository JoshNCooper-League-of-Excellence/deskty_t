#include "gfx.h"
#include "os.h"
#include "window.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// fulfill externs
procman_t procman;

void init_os() {
  procman_init(&procman);
}

int main(void) {
  // init_os();
  // spawn_process(&procman, procman.proc_list.values[0]);
  // spawn_process(&procman, procman.proc_list.values[1]);
  
  gfx_init();
  GLFWwindow *window = gfx_get_window();

  while (!glfwWindowShouldClose(window)) {
    gfx_begin_frame();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // update_input_state(window, &procman.input_state);
    // update_hit_mask(&procman);
    // update_focused_window(&procman);
    // update_procman(&procman);
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}
