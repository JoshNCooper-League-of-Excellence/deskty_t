#include "os.h"
#include "bitset.h"
#include "gfx.h"
#include "util.h"
#include "window.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "stdio.h"

void init_window(process_t *, int, int, const char *);

void spawn_process(procman_t *procman, const char *path) {
  if (procman->process_count >= MAX_PROC) {
    printf("reached maximum number of windows!");
    return;
  }
  process_t proc = {0};
  proc.procman = procman;
  proc.init_window = &init_window;

  proc.dl_handle = compile_and_open_process(path);

  // process spawning failed, we just return and don't add it to the process
  // list
  if (!proc.dl_handle)
    return;

  proc.init = dlsym(proc.dl_handle, "init");
  proc.update = dlsym(proc.dl_handle, "update");
  proc.deinit = dlsym(proc.dl_handle, "deinit");

  if (!proc.init || !proc.update || !proc.deinit) {
    printf("Unable to find init, update, or deinit functions in %s\n", path);
    printf("functions: init=%p, update=%p, deinit=%p\n", proc.init, proc.update,
           proc.deinit);
    return;
  }

  proc.init(&proc);
  procman->processes[procman->process_count++] = proc;
}

void procman_init(procman_t *procman) {
  procman->proc_list = directory_read_files("./user");
  Vector2I sz = gfx_window_size();
  bitset_init(&procman->hit_mask, sz.x, sz.y);
  procman->input_state = (input_state_t){0};
}

void update_procman(procman_t *procman) {
  
  if (gfx_window_resized()) {
    Vector2I size = gfx_window_size();
    bitset_resize(&procman->hit_mask, size.x, size.y);
  }

  for (int i = 0; i < procman->process_count; ++i) {
    process_t *proc = &procman->processes[i];
    proc->update(proc);

    if ((proc->flags & PROCESS_HAS_WINDOW) != 0) {
      draw_window(proc, &proc->window);
    }
  }
}

void *compile_and_open_process(const char *path) {
  static char so_path[512];
  static char compile_command[512];
  static char name[512];
  
  filename_without_extension((char*)path, (char**)&name);

  snprintf(so_path, sizeof(so_path), "binaries/%s.so", name);

  int required_length = snprintf(nullptr, 0, COMPILE_COMMAND_FORMAT, so_path, path);

  snprintf(compile_command, required_length + 1, COMPILE_COMMAND_FORMAT,
           so_path, path);

  printf("compiling... \e[1;32m%s\e[0m, :: \e[1;34m%s\e[0m\n", so_path,
         compile_command);

  // compile this ish.
  int result = system(compile_command);

  if (result != 0) {
    printf("Failed to compile %s.. compiler returned %d\n", path, result);
    return nullptr;
  }

  void *dl_handle = dlopen(so_path, RTLD_NOW);
  if (!dl_handle) {
    printf("Unable to load shared object file! %s\n", so_path);
    return nullptr;
  }

  return dl_handle;
}
