#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
  Camera(glm::vec3 position);

  glm::vec3 getPosition() const;
  void setPosition(const glm::vec3 &position);

  float getYaw() const;
  void setYaw(float yaw);

  float getPitch() const;
  void setPitch(float pitch);

  float getMovementSpeed() const;
  void setMovementSpeed(float speed);

  float getMouseSensitivity() const;
  void setMouseSensitivity(float sensitivity);

  float getFov() const;
  void setFov(float fov);

  glm::mat4 getViewMatrix() const;
  glm::mat4 getProjectionMatrix(float aspectRatio) const;

  void processKeyboardInput(GLFWwindow *window, float deltaTime);
  void processMouseInput(float xOffset, float yOffset, bool constrainPitch = true);

  bool isMouseInputEnabled() const;
  bool isKeyboardInputEnabled() const;

  void toggleMouseInput();
  void toggleKeyboardInput();

private:
  bool mouseInputEnabled;
  bool keyboardInputEnabled;

  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  float yaw;
  float pitch;
  float movementSpeed;
  float mouseSensitivity;
  float fov;

  void updateCameraVectors();
};

#endif
