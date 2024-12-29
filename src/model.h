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
  Model(const std::string &modelPath,
        std::vector<std::pair<std::string, std::string>> texturePaths,
        const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath);
  void Draw(const glm::mat4 &transform, const glm::mat4 &view, const glm::mat4 &projection);

  // Getters for serialization
  const std::string& getModelPath() const { return modelPath; }
  const std::string& getVertexShaderPath() const { return vertexShaderPath; }
  const std::string& getFragmentShaderPath() const { return fragmentShaderPath; }
  const std::vector<std::pair<std::string, std::string>>& getTexturePaths() const { return texturePaths; }

private:
  std::string modelPath;
  std::string vertexShaderPath;
  std::string fragmentShaderPath;
  std::vector<std::pair<std::string, std::string>> texturePaths;
  
  Shader shader;
  std::vector<Mesh> meshes;
  std::unordered_map<std::string, Texture> loadedTextures;

  void loadModel(const std::string &modelPath, std::vector<std::pair<std::string, std::string>> texturePaths);
  void processNode(aiNode *node, const aiScene *scene, std::vector<std::pair<std::string, std::string>> texturePaths);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene, std::vector<std::pair<std::string, std::string>> texturePaths);
  std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);
  std::vector<Texture> loadAllTextures(std::vector<std::pair<std::string, std::string>> texturePaths);
  unsigned int loadTextureFromFile(const char *path);
  glm::mat4 aiMatrixToGlm(const aiMatrix4x4 &matrix);
};
