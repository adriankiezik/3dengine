#include "shader.h"
#include <GLFW/glfw3.h>

#ifndef CUBE_H
#define CUBE_H

class Cube {
public:
  Cube();
  ~Cube();

  void initialize();
  void update();
private:
  Shader m_shader;
  unsigned int m_VBO, m_VAO;
};

#endif
