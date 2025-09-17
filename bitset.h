#ifndef BITSET_H
#define BITSET_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#define BITSET_UNSET_VALUE -1

typedef struct bitset_t {
  int64_t *values;
  int width, height;
} bitset_t;

static inline void bitset_resize(bitset_t *bitset, int new_width,
                                 int new_height) {
  int n_bytes = new_width * new_height;
  int n_values = (n_bytes + 7) / 8;
  bitset->width = new_width;
  bitset->height = new_height;
  bitset->values = realloc(bitset->values, n_values * sizeof(uint64_t));
}

static inline void bitset_init(bitset_t *bitset, int width, int height) {
  int n_bytes = width * height;
  int n_values = (n_bytes + 7) / 8;
  bitset->width = width;
  bitset->height = height;
  bitset->values = calloc(n_values, sizeof(uint64_t));
}

static inline void bitset_free(bitset_t *mask) { free(mask->values); }

static inline void bitset_clear_all(bitset_t *mask) {
  int n_values = (mask->width * mask->height + 7) / 8;
  memset(mask->values, BITSET_UNSET_VALUE, n_values * sizeof(uint64_t));
}

static inline int8_t bitset_get(bitset_t *mask, int x, int y) {
  if (x < 0 || x >= mask->width || y < 0 || y >= mask->height)
    return 0;
  int index = y * mask->width + x;
  int ind_index = index / 8;
  int offset = (index % 8) * 8;
  return (mask->values[ind_index] >> offset) & 0xFF;
}

static inline void bitset_set(bitset_t *mask, int x, int y, int8_t value) {
  if (x < 0 || x >= mask->width || y < 0 || y >= mask->height)
    return;
  int index = y * mask->width + x;
  int int_idx = index / 8;
  int offset = (index % 8) * 8;
  // clear
  mask->values[int_idx] &= ~(0xFFULL << offset);
  // set
  mask->values[int_idx] |= ((uint64_t)value << offset);
}

static inline void bitset_clear(bitset_t *mask, int x, int y) {
  if (x < 0 || x >= mask->width || y < 0 || y >= mask->height)
    return;
  int index = y * mask->width + x;
  int int_idx = index / 8;
  int offset = (index % 8) * 8;
  mask->values[int_idx] &= ~(0xFFULL << offset);
}

#endif
