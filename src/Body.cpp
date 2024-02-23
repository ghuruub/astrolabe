#include "Body.hpp"
#include <iostream>

Body::Body(unsigned int mass, glm::vec2 pos, glm::vec2 size, Texture2D texture,
           glm::vec2 velocity)
    : Mass(mass), Position(pos), Size(size), Texture(texture),
      Velocity(velocity), Acceleration(glm::vec2(0)) {}

void Body::ApplyForces(Body *body) {
  glm::vec2 force = body->Position - Position;
  force *= (double)(Factor * body->Mass /
                    pow(glm::length(Position - body->Position), 2));
  Acceleration += force;
}

void Body::Move(float dt) {
  Velocity += Acceleration * dt;
  Position += Velocity * dt;

  Acceleration = glm::vec2(0);
}
