#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
typedef struct {
  unsigned int capacity;
  unsigned int count;
  char **values;
} string_list_t;

void string_list_push(string_list_t *list, char *v);

string_list_t directory_read_files(const char *path);

void filename_without_extension(const char *filename, char *buf, size_t buf_len);

enum {
  STRING_BUILDER_CHUNK_LEN = 512,
};

typedef struct string_builder_t {
  unsigned int length;
  unsigned int capacity;
  char *value;
} string_builder_t;

void sb_append(string_builder_t *sb, const char *str);

void sb_appendf(string_builder_t *sb, const char *format, ...);

char *sb_get_string(const string_builder_t *sb);

void sb_appendch(string_builder_t *sb, char c);

void sb_free(string_builder_t *sb);

bool read_file_to_end(const char *path, string_builder_t *sb);

#endif