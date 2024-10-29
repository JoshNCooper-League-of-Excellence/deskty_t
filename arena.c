#include "arena.h"
#include <stdlib.h>

arena_t new_arena(uint64_t size) {
  arena_t arena = {0};
  arena.memory = malloc(size);
  arena.size = size;
  arena.ptr = 0;
  return arena;
}
void free_arena(arena_t *arena) { free(arena->memory); }
char *arena_alloc(arena_t *arena, uint64_t size) {
  if (arena->ptr + size > arena->size) {
    return NULL;
  }
  char *ptr = arena->memory + arena->ptr;
  arena->ptr += size;
  return ptr;
}
