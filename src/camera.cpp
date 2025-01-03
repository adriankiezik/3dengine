#include "camera.h"
#include <nlohmann/json.hpp>

Camera::Camera()
    : position(glm::vec3(0.0f, 0.0f, 0.0f)),
      worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
      yaw(-90.0f),
      pitch(0.0f),
      movementSpeed(1.0f),
      mouseSensitivity(0.1f),
      fov(65.0f),
      mouseInputEnabled(false),
      keyboardInputEnabled(false)
{
  updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
  return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix() const
{
  return glm::perspective(glm::radians(fov), aspectRatio, 0.01f, 1000.0f);
}

void Camera::processKeyboardInput(GLFWwindow *window, float deltaTime)
{
  if (!keyboardInputEnabled)
    return;

  float velocity = movementSpeed * deltaTime;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    position += front * velocity;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    position -= front * velocity;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    position -= right * velocity;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    position += right * velocity;
}

void Camera::processMouseInput(float xOffset, float yOffset, bool constrainPitch)
{
  if (!mouseInputEnabled)
    return;

  xOffset *= mouseSensitivity;
  yOffset *= mouseSensitivity;

  yaw += xOffset;
  pitch += yOffset;

  if (constrainPitch)
  {
    pitch = glm::clamp(pitch, -89.0f, 89.0f);
  }

  updateCameraVectors();
}

bool Camera::isMouseInputEnabled() const
{
  return mouseInputEnabled;
}

bool Camera::isKeyboardInputEnabled() const
{
  return keyboardInputEnabled;
}

void Camera::toggleMouseInput()
{
  mouseInputEnabled = !mouseInputEnabled;
}

void Camera::toggleKeyboardInput()
{
  keyboardInputEnabled = !keyboardInputEnabled;
}

void Camera::updateCameraVectors()
{
  glm::vec3 newFront;
  newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  newFront.y = sin(glm::radians(pitch));
  newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(newFront);

  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}

glm::vec3 Camera::getPosition() const
{
  return position;
}

void Camera::setPosition(const glm::vec3 &newPosition)
{
  position = newPosition;
}

float Camera::getYaw() const
{
  return yaw;
}

void Camera::setYaw(float newYaw)
{
  yaw = newYaw;
  updateCameraVectors();
}

float Camera::getPitch() const
{
  return pitch;
}

void Camera::setPitch(float newPitch)
{
  pitch = newPitch;
  updateCameraVectors();
}

float Camera::getMovementSpeed() const
{
  return movementSpeed;
}

void Camera::setMovementSpeed(float newSpeed)
{
  movementSpeed = newSpeed;
}

float Camera::getMouseSensitivity() const
{
  return mouseSensitivity;
}

void Camera::setMouseSensitivity(float newSensitivity)
{
  mouseSensitivity = newSensitivity;
}

float Camera::getFov() const
{
  return fov;
}

void Camera::setFov(float newFov)
{
  fov = newFov;
}

float Camera::getAspectRatio() const
{
  return aspectRatio;
}

void Camera::setAspectRatio(float newAspectRatio)
{
  aspectRatio = newAspectRatio;
}

nlohmann::json Camera::toJson() const
{
  return {
    {"position", {position.x, position.y, position.z}},
    {"fov", fov}
  };
}

void Camera::fromJson(const nlohmann::json &json)
{
  if (json.contains("position"))
  {
    const auto &pos = json["position"];
    setPosition(glm::vec3(pos[0], pos[1], pos[2]));
  }
  if (json.contains("fov"))
  {
    setFov(json["fov"]);
  }
}