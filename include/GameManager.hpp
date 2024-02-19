#include <vector>
#include "Body.hpp"

class GameManager {
public:
  unsigned int width, height;
  std::vector<Body*> bodies;

  ~GameManager();

  void Init();
  void Update(float dt);
  void Render();
  void ReapplyForces();
  void ProceedMouseAction();
  void CreateBody(unsigned int mass, glm::vec2 pos, glm::vec2 velocity);
  void RemoveBody(Body* body);
};
