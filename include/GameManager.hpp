#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <vector>
#include "Body.hpp"
#include "Renderer.hpp"

class GameManager {
public:
  unsigned int Width, Height;
  std::vector<Body *> bodies;
  Renderer *renderer;

  GameManager(unsigned int width, unsigned int height);

  void Init();
  void Update(float dt);
  void Render();
  void ReapplyForces();
  void MoveBodies(float dt);
  void ProceedMouseAction();
  void CreateBody(unsigned int mass, glm::vec2 pos, glm::vec2 size,
                  Texture2D texture, glm::vec2 velocity);
  void RemoveBody(Body *body);
};
#endif
