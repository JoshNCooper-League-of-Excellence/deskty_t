#ifndef OS_H
#define OS_H

#include "bitset.h"
#include "util.h"
#include "window.h"

#define MAX_PROC 127

typedef enum ProcFlags {
  PROCESS_HAS_WINDOW = 1 << 0,
} ProcFlags;

struct process_t;
typedef void (*proc_callback)(struct process_t *);

typedef struct process_t {
  int pid;
  window_t window;
  int flags;
  // Dynamic library handle
  void *dl_handle;
  // Callbacks for the DL
  proc_callback init, update, deinit;
  void (*init_window)(struct process_t *, int, int, const char *);
  struct procman_t *procman; // pointer to parent, used only for spawning windows
} process_t;

typedef struct procman_t {
  process_t processes[MAX_PROC];
  int process_count;

  // for cleaning up timestamped compilation output
  // todo: make this a list. for now,
  // just use a fixed size array, it's unlikely you'd use this for a long time.
  char temp_binary_names[1024];
  int temp_binary_count;

  string_list_t proc_list;    // paths of .c files in the user dir that we can
                             // compile into apps.

  input_state_t input_state; // input state (for all windows)
  bitset_t hit_mask;         // hit mask for all windows

} procman_t;

void procman_init(procman_t *procman);

#define COMPILE_COMMAND_FORMAT \
  "clang -std=c23 -g -shared -fPIC -o %s %s -Lbin -ldesky_t -lGL -lGLEW -lglfw"

void *compile_and_open_process(const char *path);
void spawn_process(procman_t *procman, const char *path);
void update_procman(procman_t *procman);

#endif
