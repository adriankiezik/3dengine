#include "grid.h"

Grid::Grid(float spacing, const char *vertexShaderPath, const char *fragmentShaderPath)
    : shader(vertexShaderPath, fragmentShaderPath), spacing(spacing)
{
  setup();
}

Grid::~Grid()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

void Grid::render(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const glm::vec3 &cameraPosition, float renderRadius, const glm::vec3 &color)
{
  shader.use();
  shader.setMat4("model", model);
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);
  shader.setUniform3f("color", color.x, color.y, color.z);

  generateVertices(cameraPosition, renderRadius);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

  glBindVertexArray(VAO);
  glDrawArrays(GL_LINES, 0, vertices.size() / 3);
  glBindVertexArray(0);
}

void Grid::generateVertices(const glm::vec3 &cameraPosition, float renderRadius)
{
  vertices.clear();

  int halfLines = static_cast<int>(renderRadius / spacing);
  float startX = std::floor(cameraPosition.x / spacing) * spacing;
  float startZ = std::floor(cameraPosition.z / spacing) * spacing;

  for (int i = -halfLines; i <= halfLines; ++i)
  {
    float offsetX = startX + i * spacing;
    float offsetZ = startZ + i * spacing;

    vertices.push_back(offsetX);
    vertices.push_back(0.0f);
    vertices.push_back(startZ - halfLines * spacing);
    vertices.push_back(offsetX);
    vertices.push_back(0.0f);
    vertices.push_back(startZ + halfLines * spacing);

    vertices.push_back(startX - halfLines * spacing);
    vertices.push_back(0.0f);
    vertices.push_back(offsetZ);
    vertices.push_back(startX + halfLines * spacing);
    vertices.push_back(0.0f);
    vertices.push_back(offsetZ);
  }
}

void Grid::setup()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
