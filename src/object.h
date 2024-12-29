#ifndef OBJECT_H
#define OBJECT_H

#include "model.h"
#include "script.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

class Object
{
public:
  Object(const Model &model, const glm::vec3 &position = glm::vec3(0.0f),
         const glm::vec3 &rotation = glm::vec3(0.0f),
         const glm::vec3 &scale = glm::vec3(1.0f),
         const std::string &name = "New Object");

  void Draw(const glm::vec3 &cameraPos, const glm::mat4 &view, const glm::mat4 &projection);

  void setPosition(const glm::vec3 &newPosition);
  void setRotation(const glm::vec3 &newRotation);
  void setScale(const glm::vec3 &newScale);
  void setName(const std::string &newName);

  glm::vec3 getPosition() const;
  glm::vec3 getRotation() const;
  glm::vec3 getScale() const;
  const Model &getModel() const;
  const std::string &getName() const;

  void addScript(const Script &script);
  void addScript(const std::string &scriptPath); // Add script from path
  void removeScript(const std::string &scriptName);
  const std::vector<Script> &getScripts() const { return scripts; }

private:
  std::string name;
  Model model;

  std::vector<Script> scripts;

  glm::vec3 position;
  glm::vec3 rotation; // in degrees
  glm::vec3 scale;
};

#endif
