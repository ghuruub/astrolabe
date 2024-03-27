#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <vector>
#include "Body.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "glm/fwd.hpp"

class GameManager {
public:
  unsigned int Width, Height;
  std::vector<Body *> bodies;
  Renderer *renderer;
  Camera *camera;

  GameManager(unsigned int width, unsigned int height);
  ~GameManager();

  void Init();
  void Update(float dt);
  void Render();
  void ReapplyForces();
  void MoveBodies(float dt);
  Body* CreateBody(unsigned int mass, glm::vec2 pos, float radius,
                  glm::vec2 velocity);
  void RemoveBody(Body *body);
  void ProcessMouseAction(int buttom, int action, int mod, double xpos,
                          double ypos);
  void ProcessKeyAction(int keycode, float dt);
  void ProcessScrollAction(double yoffset, float time);

private:
  bool lButtonDown = false;
  glm::vec3 clickPos;
  Body* bodyBuffer;
};
#endif
