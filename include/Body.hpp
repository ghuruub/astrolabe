#ifndef BODY_H
#define BODY_H

#include <array>
#include <vector>
#include <glm/glm.hpp>

class Body {
public:
  int Mass; // (0.01, 1000)e24 kg
  int Size;
  bool Immovable;

  glm::vec2 Position; // distance from sun (1, 1500)e9 km
  glm::vec2 Velocity;

  std::vector<glm::vec2> PositionHistory;
  int HistoryCapacity = 10;
  float HistoryPeriod = 0.3;

  int Seed;
  int AtmosphereSpeed;
  std::array<glm::vec3, 4> Pallete;
  float Alpha = 1;

  Body(unsigned int mass, glm::vec2 pos, float radius, glm::vec2 velocity);
  Body(const Body &body);

  void ApplyForces(Body *body);
  void Move(float dt);
  void Destroy(Body *body);
  bool CheckCollision(Body *body);
  void Collide(Body *body);

  static std::array<glm::vec3, 4> GeneratePallete();

private:
  glm::vec2 Acceleration;
  const float Factor = 4;
  bool shadow = false;

  float previousPositionTime = 0;
};

#endif // BODY_H
