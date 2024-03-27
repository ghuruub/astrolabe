#ifndef BODY_H
#define BODY_H

#include "Shader.hpp"
#include <glm/glm.hpp>

class Body {
public:
  int Mass; // (0.01, 1000)e24 kg
  int Size;
  bool Immovable;

  glm::vec2 Position; // distance from sun (1, 1500)e9 km
  glm::vec2 Velocity;

  Shader *BodyShader;
  int Seed;
  int AtmosphereSpeed;

  Body(unsigned int mass, glm::vec2 pos, float radius, glm::vec2 velocity, Shader* shader);

  void ApplyForces(Body *body);
  void Move(float dt);
  void Destroy(Body *body);
  bool CheckCollision(Body *body);

private:
  glm::vec2 Acceleration;
  const float Factor = 4; // 66.74; // 6.674e-11
};

#endif // BODY_H
