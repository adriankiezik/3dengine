#include <iostream>

#include "window.h"

#ifdef ENGINE_MODE
#include "editor.h"
#endif

int main() {
  Window window("3dengine", 800, 600);

  if (!window.initialize()) {
    return -1;
  }

#ifdef ENGINE_MODE
  Editor editor(window);

  if (!editor.initialize()) {
    return -1;
  }
#endif

  while (!window.shouldClose()) {
    glClear(GL_COLOR_BUFFER_BIT);
#ifdef ENGINE_MODE
    editor.update();
#endif
    window.update();
  }

  return 0;
}
