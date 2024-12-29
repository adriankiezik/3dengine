#include "object.h"

Object::Object(const Model &model, const glm::vec3 &position,
               const glm::vec3 &rotation, const glm::vec3 &scale,
               const std::string &name)
    : model(model), position(position), rotation(rotation), scale(scale), name(name) {}

void Object::Draw(const glm::vec3 &cameraPos, const glm::mat4 &view, const glm::mat4 &projection)
{
  glm::mat4 transform = glm::mat4(1.0f);

  transform = glm::translate(transform, position);
  transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
  transform = glm::scale(transform, scale);

  model.Draw(transform, view, projection);
}

void Object::setPosition(const glm::vec3 &newPosition)
{
  position = newPosition;
}

void Object::setRotation(const glm::vec3 &newRotation)
{
  rotation = newRotation;
}

void Object::setScale(const glm::vec3 &newScale)
{
  scale = newScale;
}

void Object::setName(const std::string &newName) {
    name = newName;
}

glm::vec3 Object::getPosition() const
{
  return position;
}

glm::vec3 Object::getRotation() const
{
  return rotation;
}

glm::vec3 Object::getScale() const
{
  return scale;
}

const Model &Object::getModel() const
{
  return model;
}

const std::string &Object::getName() const {
    return name;
}
