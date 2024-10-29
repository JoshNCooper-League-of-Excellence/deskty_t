#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>

typedef struct {
  char *memory;
  uint64_t size;
  uint64_t ptr;
} arena_t;

arena_t new_arena(uint64_t size);
void free_arena(arena_t *arena);
char *arena_alloc(arena_t *arena, uint64_t size);

#endif
