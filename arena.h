#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>

typedef struct {
  char *memory;
  uint64_t size;
  uint64_t ptr;
} arena_t;

void arena_free(arena_t *arena);
void arena_init(arena_t *arena, uint64_t size);

char *arena_alloc(arena_t *arena, uint64_t size);

#endif
