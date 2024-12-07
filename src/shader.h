#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <glad.h>
#include <iostream>
#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
  unsigned int id;

  Shader(const char *vertexPath, const char *fragmentPath);

  void use();

  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setMat4(const std::string &name, const glm::mat4 &mat) const;
  void setUniform3f(const std::string &name, float x, float y, float z) const;
  void setUniform4f(const std::string &name, float x, float y, float z,
                    float w) const;
  void setUniformMatrix4fv(const std::string &name, GLsizei count,
                           GLboolean transpose, const GLfloat *value) const;

private:
  std::string loadShaderSource(const char *filePath) const;
  GLuint compileShader(GLenum shaderType, const char *shaderSource) const;
};

#endif