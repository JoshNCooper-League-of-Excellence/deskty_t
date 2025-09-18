#ifndef VA_EXPAND_H
#define VA_EXPAND_H

#define __VA_ARGS_EXPAND(macro, ...)                                                               \
  __VA_ARGS_EXPAND_(__VA_ARGS__, __VA_ARGS_EXPAND_SEQ_N())(macro, __VA_ARGS__)

#define __VA_ARGS_EXPAND_(...) __VA_ARGS_EXPAND_N(__VA_ARGS__)
#define __VA_ARGS_EXPAND_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) __VA_ARGS_EXPAND_##N

#define __VA_ARGS_EXPAND_SEQ_N() 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define __VA_ARGS_EXPAND_1(macro, a1) macro(a1)
#define __VA_ARGS_EXPAND_2(macro, a1, a2) macro(a1) macro(a2)
#define __VA_ARGS_EXPAND_3(macro, a1, a2, a3) macro(a1) macro(a2) macro(a3)
#define __VA_ARGS_EXPAND_4(macro, a1, a2, a3, a4) macro(a1) macro(a2) macro(a3) macro(a4)
#define __VA_ARGS_EXPAND_5(macro, a1, a2, a3, a4, a5)                                              \
  macro(a1) macro(a2) macro(a3) macro(a4) macro(a5)
#define __VA_ARGS_EXPAND_6(macro, a1, a2, a3, a4, a5, a6)                                          \
  macro(a1) macro(a2) macro(a3) macro(a4) macro(a5) macro(a6)
#define __VA_ARGS_EXPAND_7(macro, a1, a2, a3, a4, a5, a6, a7)                                      \
  macro(a1) macro(a2) macro(a3) macro(a4) macro(a5) macro(a6) macro(a7)
#define __VA_ARGS_EXPAND_8(macro, a1, a2, a3, a4, a5, a6, a7, a8)                                  \
  macro(a1) macro(a2) macro(a3) macro(a4) macro(a5) macro(a6) macro(a7) macro(a8)
#define __VA_ARGS_EXPAND_9(macro, a1, a2, a3, a4, a5, a6, a7, a8, a9)                              \
  macro(a1) macro(a2) macro(a3) macro(a4) macro(a5) macro(a6) macro(a7) macro(a8) macro(a9)
#define __VA_ARGS_EXPAND_10(macro, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10)                        \
  macro(a1) macro(a2) macro(a3) macro(a4) macro(a5) macro(a6) macro(a7) macro(a8) macro(a9)        \
      macro(a10)

#endif