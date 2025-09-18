#ifndef LIST_H
#define LIST_H

#include <string.h>
#include "va_expand.h"

#define DEFINE_LISTS(...) __VA_ARGS_EXPAND(DEFINE_LIST, __VA_ARGS__)


#define DEFINE_LIST(T)                                                                             \
  typedef struct T##_list {                                                                        \
    T *data;                                                                                       \
    unsigned int length;                                                                           \
    unsigned int capacity;                                                                         \
  } T##_list;

#define LIST_OF(T) T##_list

/* infer element size from list.data, avoid passing T for push/pop/free */
#define LIST_INIT(list)                                                                            \
  do {                                                                                             \
    (list).data = NULL;                                                                            \
    (list).length = 0;                                                                             \
    (list).capacity = 0;                                                                           \
  } while (0)

#define LIST_PUSH(list, v)                                                                         \
  do {                                                                                             \
    if ((list).length == (list).capacity) {                                                        \
      unsigned int newcap = (list).capacity ? (list).capacity * 2 : 1;                             \
      void *tmp = realloc((list).data, newcap * sizeof *(list).data);                              \
      if (!tmp)                                                                                    \
        break;                                                                                     \
      (list).data = tmp;                                                                           \
      (list).capacity = newcap;                                                                    \
    }                                                                                              \
    (list).data[(list).length++] = (v);                                                            \
  } while (0)

/* LIST_POP uses GNU/Clang extensions (typeof + statement expression) to synthesize a zero value */
#define LIST_POP(list)                                                                             \
  ({                                                                                               \
    typeof((list).data[0]) __ret = {0};                                                            \
    if ((list).length > 0)                                                                         \
      __ret = (list).data[--(list).length];                                                        \
    __ret;                                                                                         \
  })

#define LIST_FREE(list)                                                                            \
  do {                                                                                             \
    free((list).data);                                                                             \
    (list).data = NULL;                                                                            \
    (list).length = 0;                                                                             \
    (list).capacity = 0;                                                                           \
  } while (0)

#define LIST_CLEAR(list)                                                                           \
  do {                                                                                             \
    (list).length = 0;                                                                             \
  } while (0)


#endif