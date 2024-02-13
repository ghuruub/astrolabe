#include <vector>
#include "Body.hpp"

class GameManager {
public:
  unsigned int width, height;

  void Init();
  void Update(float dt);
  void ReapplyForces();
  void Render();
  std::vector<Body *> bodies;
};
