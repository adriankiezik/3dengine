#include <iostream>
#include "window.h"
#include "editor.h"
#include "cube.h"

int main() {
  Window window("3dengine", 800, 600);

  if (!window.initialize()) {
    return -1;
  }

  Editor editor(window);

  if (!editor.initialize()) {
    return -1;
  }

  Cube cube;
  cube.initialize();

  while (!window.shouldClose()) {
    window.update();
    cube.update();
    editor.update();
    window.postUpdate();
  }

  return 0;
}
