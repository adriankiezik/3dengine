#include <vector>
#include <glm/glm.hpp>

#ifndef MESH_H
#define MESH_H

struct Vertex
{
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

struct Texture
{
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh
{
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;
  unsigned int VAO;

  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<unsigned int> &indices,
       const std::vector<Texture> &textures);
  void Draw(unsigned int shaderProgram);

private:
  unsigned int VBO, EBO;
  void setupMesh();
};

#endif
