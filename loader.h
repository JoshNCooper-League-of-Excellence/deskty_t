#ifndef LOADER_H
#define LOADER_H

#include "rl.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>

static window_t load_app(const char * path) {
  window_t window = new_window();
  window.title = path;

  char so_path[256];
  char compile_command[512];


  // Get the output path, and format a system call to compile the .so
  {
    const char* name = GetFileNameWithoutExt(path);
    snprintf(so_path, sizeof(so_path), "binaries/%s.so", name);

    #define COMPILE_COMMAND_FORMAT "clang -std=c23 -shared -fPIC -o %s %s"
    int required_length = snprintf(nullptr, 0, COMPILE_COMMAND_FORMAT, so_path, path);
    snprintf(compile_command, required_length + 1, COMPILE_COMMAND_FORMAT, so_path, path);

    #if DEBUG
      printf("app path :: %s\n", path);
      printf("app name :: %s\n", name);
      printf("app .so  :: %s\n", so_path);
    #endif
  }


  printf("Compiling... %s :: %s\n", so_path, compile_command);
  int result = system(compile_command);

  if (result != 0) {
    printf("Failed to compile %s.. compiler returned %d\n", path, result);
    return window;
  }

  window.dl_handle = dlopen(so_path, RTLD_NOW);

  if (!window.dl_handle) {
    printf("Unable to load shared object file! %s\n", so_path);
    return window;
  }

  window.init = (window_callback)dlsym(window.dl_handle, "init");
  window.update = (window_callback)dlsym(window.dl_handle, "update");
  window.deinit = (window_callback)dlsym(window.dl_handle, "deinit");

  if (!window.init || !window.update || !window.deinit) {
    printf("Unable to find init, update, or deinit functions in %s\n", so_path);
    printf("functions: init=%p, update=%p, deinit=%p\n", window.init, window.update, window.deinit);
    return window;
  }

  // compile the binary to an so, put it in binaries/name.so ✅
  // load the so. ✅
  // get the init, update, and deinit function pointers. ✅

  window.init(&window);

  return window;
};


#endif
