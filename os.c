#include "os.h"
#include "window.h"
#include <stdio.h>


window_t *init_window(process_t*, int, int, const char *);

void spawn_process(procman_t *procman, const char *path) {
  static char so_path[512];
  static char compile_command[512];

  if (procman->process_count >= 48) {
    printf("reached maximum number of windows!");
    return;
  }
  process_t proc = {0};
  proc.procman = procman;
  proc.init_window = &init_window;

  const char *name = GetFileNameWithoutExt(path);
  snprintf(so_path, sizeof(so_path), "binaries/%s.so", name);

  int required_length =
      snprintf(nullptr, 0, COMPILE_COMMAND_FORMAT, so_path, path);
  snprintf(compile_command, required_length + 1, COMPILE_COMMAND_FORMAT,
           so_path, path);

  printf("compiling... \e[1;32m%s\e[0m, :: \e[1;34m%s\e[0m\n", so_path,
         compile_command);
  int result = system(compile_command);

  if (result != 0) {
    printf("Failed to compile %s.. compiler returned %d\n", path, result);
    return;
  }

  proc.dl_handle = dlopen(so_path, RTLD_NOW);

  if (!proc.dl_handle) {
    printf("Unable to load shared object file! %s\n", so_path);
    return;
  }

  proc.init = dlsym(proc.dl_handle, "init");
  proc.update = dlsym(proc.dl_handle, "update");
  proc.deinit = dlsym(proc.dl_handle, "deinit");

  if (!proc.init || !proc.update || !proc.deinit) {
    printf("Unable to find init, update, or deinit functions in %s\n", so_path);
    printf("functions: init=%p, update=%p, deinit=%p\n", proc.init, proc.update,
           proc.deinit);
    return;
  }

  proc.init(&proc);
  procman->processes[procman->process_count++] = proc;
}

procman_t new_procman() {
  procman_t procman = {0};
  procman.proc_list = LoadDirectoryFiles("./user");
  procman.hit_mask = new_bitset(GetScreenWidth(), GetScreenHeight());
  return procman;
}
void update_procman(procman_t *procman) {
  for (int i = 0; i < procman->process_count; ++i) {
    process_t *proc = &procman->processes[i];
    if (proc->update) {
      proc->update(proc);
    }

    if ((proc->flags & PROCESS_HAS_WINDOW) != 0) {
      draw_window(proc, &proc->window);
    }
  }
}
