#include "util.h"

#include "dirent.h"
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

void string_list_push(string_list_t *list, char *v) {
  if (!list)
    return;

  if (list->capacity == 0) {
    list->capacity = 12;
    list->values = malloc(sizeof(char *) * list->capacity);
  }

  if (list->count >= list->capacity) {
    list->capacity *= 2;
    list->values = realloc(list->values, sizeof(char *) * list->capacity);
  }

  list->values[list->count++] = v;
}

char *string_list_pop(string_list_t *list) {
  if (!list || list->count == 0)
    return NULL;

  return list->values[--list->count];
}

string_list_t directory_read_files(const char *path) {
  string_list_t files = {0};
  DIR *dir = opendir(path);

  if (dir == NULL)
    return files;

  struct dirent *entry;
  while ((entry = readdir(dir))) {
    const char *name = entry->d_name;

    /* skip . and .. */
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
      continue;

    /* optional: only include .c files */
    size_t n = strlen(name);
    if (n < 3) /* too short to be *.c */
      continue;
    if (strcmp(name + n - 2, ".c") != 0)
      continue;

    char *dup = strdup(name);
    if (!dup)
      continue;

    string_list_push(&files, dup);
  }

  closedir(dir);
  return files;
}

void filename_without_extension(const char *filename, char *buf, size_t bufsize) {
  if (!filename || !buf || bufsize == 0)
    return;

  const char *dot = strrchr(filename, '.');
  size_t len = (dot && dot != filename) ? (size_t)(dot - filename) : strlen(filename);

  if (len >= bufsize)
    len = bufsize - 1;

  memcpy(buf, filename, len);
  buf[len] = '\0';
}

void sb_append(string_builder_t *sb, const char *str) {
  size_t str_len = strlen(str);
  while (sb->length + str_len >= sb->capacity) {
    sb->capacity = sb->capacity * 2 + str_len;
    sb->value = (char *)realloc(sb->value, sb->capacity);
  }
  memcpy(sb->value + sb->length, str, str_len);
  sb->length += str_len;
  sb->value[sb->length] = '\0';
}

void sb_appendf(string_builder_t *sb, const char *format, ...) {
  char buffer[1024];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  sb_append(sb, buffer);
}

char *sb_get_string(const string_builder_t *sb) {
  char *result = (char *)malloc(sb->length + 1);
  memcpy(result, sb->value, sb->length);
  result[sb->length] = '\0';
  return result;
}

void sb_appendch(string_builder_t *sb, char c) {
  if (sb->length + 1 >= sb->capacity) {
    sb->capacity = sb->capacity * 2 + 1;
    sb->value = (char *)realloc(sb->value, sb->capacity);
  }
  sb->value[sb->length++] = c;
  sb->value[sb->length] = '\0';
}

bool read_file_to_end(const char *path, string_builder_t *sb) {
  FILE *fd = fopen(path, "r");

  if (!fd) {
    fprintf(stderr, "unable to read file %s", path);
    return false;
  }

  char c;
  while ((c = fgetc(fd)) != EOF) {
    sb_appendch(sb, c);
  }

  return true;
}

void sb_free(string_builder_t *sb) {
  free(sb->value);
  sb->capacity = 0;
  sb->length = 0;
}
