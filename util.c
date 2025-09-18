#include "util.h"

#include "dirent.h"
#include <stdlib.h>
#include <string.h>

void string_list_push(string_list_t *list, char *v) {
  if (list->capacity == 0)
    list->capacity = 12;

  if (list->count >= list->capacity || list->values == nullptr)
    list->values = realloc(list->values, sizeof(char *) * list->capacity);

  list->values[list->count++] = v;
}

char *string_list_pop(string_list_t *list) {
  if (list->count == 0)
    return nullptr;

  return list->values[list->count--];
}

string_list_t directory_read_files(const char *path) {
  string_list_t files = {0};
  DIR *dir = opendir(path);

  if (dir == nullptr)
    return files;

  struct dirent *entry;

  while ((entry = readdir(dir)))
    string_list_push(&files, entry->d_name);

  return files;
}

void filename_without_extension(char *filename, char **buf) {
  const char *dot = strrchr(filename, '.');

  if (!dot || dot == filename) {
    *buf = filename;
    return;
  }

  size_t len = dot - filename;
  strncpy(*buf, filename, len);
  (*buf)[len] = '\0';
}
