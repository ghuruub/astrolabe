#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <array>
#include <stack>
#include <vector>
#include "Body.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "glm/fwd.hpp"

class GameManager {
public:
  unsigned int Width, Height;
  std::vector<Body *> bodies;
  bool DrawTrails = false;
  Renderer *renderer;
  Camera *camera;

  GameManager(unsigned int width, unsigned int height);
  ~GameManager();

  void Init();
  void ChangeResolution(int width, int height);
  void Render();
  void DrawUI();
  void Update(float dt);
  void ReapplyForces();
  void ReapplyForcesTo(Body* body);
  void MoveBodies(float dt);
  void CheckCollisions();
  Body* CreateBody(unsigned int mass, glm::vec2 pos, float radius,
                  glm::vec2 velocity);
  void BodyCreationEffects();
  void RemoveBody(Body *body);
  void ProcessMouseAction(int buttom, int action, double xpos,
                          double ypos);
  void ProcessKeyAction(int keycod, float dt, bool shift = false);
  void ProcessScrollAction(double yoffset, float time);

private:
  bool bodyCreation = false;
  Body* bodyBuffer;
  int shadowsAmout = 3;
  std::vector<Body> shadows;
  glm::vec3 leftClickPos;
  glm::vec3 rightClickPos;
  float bodySpeedSensitivity = 1;

  // ui
  bool uiMassive = false;
  std::stack<std::array<glm::vec3, 4>> previousColor;
};
#endif
