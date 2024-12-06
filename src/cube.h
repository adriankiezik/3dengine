#include "shader.h"
#include "camera.h"
#include <GLFW/glfw3.h>

#ifndef CUBE_H
#define CUBE_H

class Cube {
public:
  Cube(Camera &camera);
  ~Cube();

  void initialize();
  void update();
private:
  Shader m_shader;
  Camera &m_camera;
  unsigned int m_VBO, m_VAO;
};

#endif
