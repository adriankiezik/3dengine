#include "shader.h"

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
  std::string vertexShaderSource = loadShaderSource(vertexPath);
  std::string fragmentShaderSource = loadShaderSource(fragmentPath);

  GLuint vertexShader =
      compileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
  GLuint fragmentShader =
      compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

  id = glCreateProgram();

  glAttachShader(id, vertexShader);
  glAttachShader(id, fragmentShader);

  glLinkProgram(id);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::use() { glUseProgram(id); }

void Shader::setBool(const std::string &name, bool value) const
{
  glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
  glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
  glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
  glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
}

void Shader::setUniform1f(const std::string &name, float x) const
{
  int location = glGetUniformLocation(id, name.c_str());
  glUniform1f(location, x);
}

void Shader::setUniform2f(const std::string &name, float x, float y) const
{
  int location = glGetUniformLocation(id, name.c_str());
  glUniform2f(location, x, y);
}

void Shader::setUniform3f(const std::string &name, float x, float y, float z) const
{
  int location = glGetUniformLocation(id, name.c_str());
  glUniform3f(location, x, y, z);
}

void Shader::setUniform4f(const std::string &name, float x, float y, float z,
                          float w) const
{
  int location = glGetUniformLocation(id, name.c_str());
  glUniform4f(location, x, y, z, w);
}

void Shader::setUniformMatrix4fv(const std::string &name, GLsizei count,
                                 GLboolean transpose,
                                 const GLfloat *value) const
{
  unsigned int location = glGetUniformLocation(id, name.c_str());
  glUniformMatrix4fv(location, count, transpose, value);
}

std::string Shader::loadShaderSource(const char *filePath) const
{
  std::ifstream file(filePath);
  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}

GLuint Shader::compileShader(GLenum shaderType,
                             const char *shaderSource) const
{
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderSource, nullptr);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    char *log = new char[logLength];
    glGetShaderInfoLog(shader, logLength, &logLength, log);
    std::cerr << "Shader compilation failed:\n"
              << log << std::endl;
    delete[] log;
  }

  return shader;
}
