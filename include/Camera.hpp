#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

enum CameraMovement { Up, Down, Left, Right };

class Camera {
public:
  glm::vec3 Position;
  glm::vec3 UpUnitVec = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 FrontUnitVec = glm::vec3(0.0f, 0.0f, 1.0f);
  glm::vec3 RightUnitVec = glm::vec3(1.0f, 0.0f, 0.0f);

  int ScreenWidth;
  int ScreeHeight;

  float Zoom = 1.0f;
  float ZoomBuffer = 0.0f;

  Camera(glm::vec2 pos, int width, int height);
  Camera(float posX, float posY, int width, int height);

  glm::mat4 GetViewMatrix();
  void ProcessKeyboard(CameraMovement direction, bool srint, float dt);
  void ProcessMouseScroll(float yoffset, float time);

private:
  const float ZOOM_MIN = 0.01f;
  const float ZOOM_MAX = 1.0f;
  const float ZoomSpeed = .1f;

  const float MovementSpeed = 1000.0f;
  const float MovementSpeedSprint = 3000.0f;
};

#endif
