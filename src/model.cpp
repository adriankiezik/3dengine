#include "model.h"
#include "glad.h"
#include <iostream>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <assimp/matrix4x4.h>
#include <glm/gtc/matrix_transform.hpp>

Model::Model(const std::string &modelPath,
             std::vector<std::pair<std::string, std::string>> texturePaths,
             const std::string &vertexShaderPath,
             const std::string &fragmentShaderPath)
    : shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str()),
      modelPath(modelPath),
      vertexShaderPath(vertexShaderPath),
      fragmentShaderPath(fragmentShaderPath),
      texturePaths(texturePaths)
{
  loadModel(modelPath, texturePaths);
}

void Model::Draw(const glm::mat4 &transform, const glm::mat4 &view, const glm::mat4 &projection)
{
  shader.use();
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);
  shader.setMat4("model", transform);

  shader.setUniform3f("lightDirection", 1.0f, -1.0f, -1.0f); // Sunlight direction
  shader.setUniform3f("lightColor", 1.0f, 1.0f, 1.0f);       // White sunlight

  // Extract camera position from view matrix
  glm::mat4 invView = glm::inverse(view);
  glm::vec3 cameraPos(invView[3]);
  shader.setUniform3f("viewPosition", cameraPos.x, cameraPos.y, cameraPos.z);

  for (Mesh &mesh : meshes)
  {
    // Bind the normal map texture
    for (unsigned int i = 0; i < mesh.textures.size(); i++)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      std::string name = mesh.textures[i].type;

      // Ensure the normal map texture is assigned correctly
      if (name == "texture_normal")
      {
        glUniform1i(glGetUniformLocation(shader.id, "texture_normal"), i);
      }
      else
      {
        glUniform1i(glGetUniformLocation(shader.id, name.c_str()), i);
      }

      glBindTexture(GL_TEXTURE_2D, mesh.textures[i].id);
    }

    mesh.Draw(shader);
  }
}

void Model::loadModel(const std::string &modelPath, std::vector<std::pair<std::string, std::string>> texturePaths)
{
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return;
  }

  processNode(scene->mRootNode, scene, texturePaths);
}

void Model::processNode(aiNode *node, const aiScene *scene, std::vector<std::pair<std::string, std::string>> texturePaths)
{
  // Process each mesh in the node
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene, texturePaths));
  }

  // Recursively process each child node
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    processNode(node->mChildren[i], scene, texturePaths);
  }

  // Apply the node transformation (including scale, rotation, and translation)
  glm::mat4 nodeTransformation = aiMatrixToGlm(node->mTransformation);

  // Apply a scaling factor to the transformation matrix (for example, 0.1f to make the model smaller)
  glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)); // Adjust scale factor as needed
  nodeTransformation = scalingMatrix * nodeTransformation;

  // Apply the transformation to the meshes
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    // Apply the node transformation to the vertices (for example) in the mesh
    for (unsigned int j = 0; j < mesh->mNumVertices; j++)
    {
      glm::vec4 vertex(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1.0f);
      glm::vec4 transformedVertex = nodeTransformation * vertex;
      mesh->mVertices[j].x = transformedVertex.x;
      mesh->mVertices[j].y = transformedVertex.y;
      mesh->mVertices[j].z = transformedVertex.z;
    }
  }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, std::vector<std::pair<std::string, std::string>> texturePaths)
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  glm::vec3 scaleFactor(0.1f, 0.1f, 0.1f);

  // Process vertices
  for (unsigned int i = 0; i < mesh->mNumVertices; i++)
  {
    Vertex vertex;
    vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z) * scaleFactor;
    vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

    if (mesh->mTextureCoords[0])
    {
      vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
    }
    else
    {
      vertex.texCoords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }

  // Process indices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++)
  {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
    {
      indices.push_back(face.mIndices[j]);
    }
  }

  std::vector<Texture> textures = loadAllTextures(texturePaths);

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadAllTextures(std::vector<std::pair<std::string, std::string>> texturePaths)
{
  std::vector<Texture> textures;

  for (const auto &[type, path] : texturePaths)
  {
    Texture texture;
    texture.id = loadTextureFromFile(path.c_str());
    texture.type = type;
    texture.path = path;
    textures.push_back(texture);
  }

  return textures;
}

unsigned int Model::loadTextureFromFile(const char *path)
{
  std::string filename = std::string(path);

  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

  if (data)
  {
    GLenum format = (nrComponents == 1) ? GL_RED : (nrComponents == 3) ? GL_RGB
                                                                       : GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else
  {
    std::cerr << "Texture failed to load at path: " << filename << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}

glm::mat4 Model::aiMatrixToGlm(const aiMatrix4x4 &matrix)
{
  return glm::mat4(
      matrix.a1, matrix.a2, matrix.a3, matrix.a4,
      matrix.b1, matrix.b2, matrix.b3, matrix.b4,
      matrix.c1, matrix.c2, matrix.c3, matrix.c4,
      matrix.d1, matrix.d2, matrix.d3, matrix.d4);
}
