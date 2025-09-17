## process:
  - add a memory allocator to the process_t, malloc, free, realloc, calloc.

## rendering:
  - replace raylib with a hand rolled gl renderer, so that we can have more control over the rendering process. right now, we'd have to manually wrap every single
  raylib function call and that's just annoying as hell, as well as it creates a ton of temporary 'draw command' objects.

## theming:
  - add a more generalized way to handle windowing, especially theming, so someone could write a 'window manager'
  - add a way to have a 'theme' file, so the user can change the look of the system \?? maybe ??\

## configuration:
  - add a way to have some sort of config file for the user space, so developers can control which compiler is used, compilation flags, etc.
