#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glad.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "mesh.h"
#include "shader.h"

class Model
{
public:
  Model(const std::string &path);
  void Draw(Shader &shader);

private:
  std::vector<Mesh> meshes;
  std::unordered_map<std::string, Texture> loadedTextures;

  void loadModel(const std::string &path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);
  std::vector<Texture> loadAllTextures(std::vector<std::pair<std::string, std::string>> texturePaths);
  unsigned int loadTextureFromFile(const char *path);
  glm::mat4 aiMatrixToGlm(const aiMatrix4x4 &matrix);
};
