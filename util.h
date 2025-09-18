#ifndef UTIL_H
#define UTIL_H

typedef struct {
  unsigned int capacity;
  unsigned int count;
  char **values;
} string_list_t;

void string_list_push(string_list_t *list, char *v);

string_list_t directory_read_files(const char *path);

void filename_without_extension(char *filename, char **buf);

#endif