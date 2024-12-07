#include "model.h"
#include "glad.h"
#include <iostream>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <assimp/matrix4x4.h>
#include <glm/gtc/matrix_transform.hpp>

Model::Model(const std::string &path, const std::string &textureDirectory)
{
  this->textureDirectory = textureDirectory;
  loadModel(path, textureDirectory);
}

void Model::Draw(unsigned int shaderProgram)
{
  for (Mesh &mesh : meshes)
  {
    mesh.Draw(shaderProgram);
  }
}

void Model::loadModel(const std::string &path, const std::string &textureDirectory)
{
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));
  this->textureDirectory = textureDirectory;

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
  // Process each mesh in the node
  for (unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  // Recursively process each child node
  for (unsigned int i = 0; i < node->mNumChildren; i++)
  {
    processNode(node->mChildren[i], scene);
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

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  glm::vec3 scaleFactor(0.1f, 0.1, 0.1f);

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

  if (mesh->mMaterialIndex >= 0)
  {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  }

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName)
{
  std::vector<Texture> textures;

  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
  {
    aiString str;
    mat->GetTexture(type, i, &str);

    std::string textureFileName = std::string(str.C_Str());
    size_t lastSlash = textureFileName.find_last_of("/\\"); // Find the last slash or backslash
    if (lastSlash != std::string::npos)
    {
      textureFileName = textureFileName.substr(lastSlash + 1);
    }

    std::string correctedPath = textureDirectory + "/" + textureFileName;

    if (loadedTextures.find(correctedPath) != loadedTextures.end())
    {
      textures.push_back(loadedTextures[correctedPath]);
    }
    else
    {
      Texture texture;
      texture.id = loadTextureFromFile(correctedPath.c_str(), textureDirectory);
      texture.type = typeName;
      texture.path = correctedPath;
      textures.push_back(texture);
      loadedTextures[correctedPath] = texture;
    }
  }

  return textures;
}

unsigned int Model::loadTextureFromFile(const char *path, const std::string &directory)
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
