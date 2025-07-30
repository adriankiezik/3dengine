#include "mesh.h"
#include <glad.h>
#include <iostream>
#include "shader.h"

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures)
    : vertices(vertices), indices(indices), textures(textures)
{
  setupMesh();
}

Mesh::Mesh(std::vector<Vertex> &&vertices, std::vector<unsigned int> &&indices, std::vector<Texture> &&textures)
    : vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures))
{
  setupMesh();
}

void Mesh::Draw(Shader &shader)
{
  // Cache uniform locations if not done already
  if (uniformLocations.size() != textures.size()) {
    cacheUniformLocations(shader);
  }

  // Bind textures more efficiently
  for (unsigned int i = 0; i < textures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, textures[i].id);
    
    // Use cached uniform location instead of looking it up every frame
    if (uniformLocations[i] != -1) {
      glUniform1i(uniformLocations[i], i);
    }
  }

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // Reset to default texture unit only once at the end
  glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));

  glBindVertexArray(0);
}

void Mesh::cacheUniformLocations(Shader &shader)
{
  uniformLocations.clear();
  uniformLocations.reserve(textures.size());
  
  for (const auto &texture : textures) {
    int location = glGetUniformLocation(shader.id, texture.type.c_str());
    uniformLocations.push_back(location);
  }
}
