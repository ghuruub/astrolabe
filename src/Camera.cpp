#include "Camera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include <algorithm>
#include <iostream>

Camera::Camera(glm::vec2 pos, int width, int height)
    : Position(glm::vec3(pos, 0.0f)), ScreenWidth(width), ScreeHeight(height) {}

Camera::Camera(float posX, float posY, int width, int height)
    : Position(glm::vec3(posX, posY, 0.0f)), ScreenWidth(width),
      ScreeHeight(height) {}

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
    Position += velocity * RightUnitVec;
    break;
  case Left:
    Position -= velocity * RightUnitVec;
    break;
  }
}

void Camera::ProcessMouseScroll(float yoffset) {
  if (ZoomBuffer * yoffset < 0) {
    ZoomBuffer = 0;
  }
  ZoomBuffer += yoffset * ZoomFactor;
}

void Camera::Update(float dt) {
  float change = ZoomBuffer * ZoomSpeed * dt * Zoom;
  Zoom += change;
  ZoomBuffer -= change;
  Zoom = std::clamp(Zoom, ZOOM_MIN, ZOOM_MAX);
}
