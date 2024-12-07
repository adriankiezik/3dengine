#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glad.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "mesh.h"

class Model
{
public:
  Model(const std::string &path, const std::string &textureDirectory);
  void Draw(unsigned int shaderProgram);

private:
  std::string directory;
  std::string textureDirectory;
  std::vector<Mesh> meshes;
  std::unordered_map<std::string, Texture> loadedTextures;

  void loadModel(const std::string &path, const std::string &textureDirectory);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);
  unsigned int loadTextureFromFile(const char *path, const std::string &directory);
  glm::mat4 aiMatrixToGlm(const aiMatrix4x4 &matrix);
};
