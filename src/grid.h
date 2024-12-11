#ifndef GRID_H
#define GRID_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "shader.h"

class Grid
{

public:
  Grid(float spacing, const char *vertexShaderPath, const char *fragmentShaderPath);

  ~Grid();

  void render(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection,
              const glm::vec3 &cameraPosition, float renderRadius, const glm::vec3 &color);

private:
  unsigned int VAO, VBO;
  std::vector<float> vertices;
  float spacing;
  Shader shader;

  void generateVertices(const glm::vec3 &cameraPosition, float renderRadius);
  void setup();
};

#endif