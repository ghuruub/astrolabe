#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <vector>
#include "Body.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"

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
  void CreateBody(unsigned int mass, glm::vec2 pos, float radius,
                  Texture2D texture, glm::vec2 velocity);
  void RemoveBody(Body *body);
  void ProcessMouseAction();
  void ProcessKeyAction(int keycode, float dt);
  void ProcessScrollAction(double yoffset);
};
#endif
