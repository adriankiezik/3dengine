#include "shader.h"
#include "camera.h"
#include <GLFW/glfw3.h>

#ifndef CUBE_H
#define CUBE_H

class Cube
{
public:
  Cube(Camera &camera);
  ~Cube();

  void init();
  void update();

private:
  Shader shader;
  Camera &camera;
  unsigned int VBO, VAO;
};

#endif
