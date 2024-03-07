#include "Camera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <algorithm>

Camera::Camera(glm::vec2 pos) : Position(glm::vec3(pos, 0.0f)) {}

Camera::Camera(float posX, float posY)
    : Position(glm::vec3(posX, posY, 0.0f)) {}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(Position, Position + FrontUnitVec, UpUnitVec);
}

void Camera::ProcessKeyboard(CameraMovement dir, bool sprint, float dt) {
  float velocity = (sprint ? MovementSpeedSprint : MovementSpeed) * dt;
  switch (dir) {
  case Up:
    Position += velocity * UpUnitVec;
    break;
  case Down:
    Position -= velocity * UpUnitVec;
    break;
  case Right:
    Position -= velocity * RightUnitVec;
    break;
  case Left:
    Position += velocity * RightUnitVec;
    break;
  }
}

void Camera::ProcessMouseScroll(float yoffset) {
  Zoom -= yoffset * ZoomSpeed;
  Zoom = std::clamp(Zoom, ZOOM_MIN, ZOOM_MAX);
}
