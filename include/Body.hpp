#ifndef BODY_H
#define BODY_H

#include <glm/glm.hpp>

class Body {
public:
  int Mass;           // (0.01, 1000)e24 kg
  glm::vec2 Position; // distance from sun (1, 1500)e9 km

  Body(unsigned int mass, glm::vec2 pos, glm::vec2 velocity);

  void ApplyForces(Body *body);
  void Move(float dt);
  void Destroy(Body* body);

private:
  glm::vec2 Velocity;
  glm::vec2 Acceleration;
  const int Factor = 6674; // 6.674e-11
};

#endif // BODY_H
