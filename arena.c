#include "arena.h"
#include <stdlib.h>

void arena_init(arena_t *arena, uint64_t size) {
  arena->memory = malloc(size);
  arena->size = size;
  arena->ptr = 0;
}

void arena_free(arena_t *arena) { free(arena->memory); }

char *arena_alloc(arena_t *arena, uint64_t size) {
  if (arena->ptr + size > arena->size) {
    return NULL;
  }
  char *ptr = arena->memory + arena->ptr;
  arena->ptr += size;
  return ptr;
}
