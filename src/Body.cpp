#include "Body.hpp"

Body::Body(unsigned int mass, glm::vec2 pos, glm::vec2 velocity, int ID)
    : Mass(mass), Position(pos), Velocity(velocity),
      Acceleration(glm::vec2(0)) {}

void Body::ApplyForces(Body *body) {
  glm::vec2 force = body->Position - this->Position;
  force *= (double)(this->Factor * this->Mass * body->Mass /
                    pow(glm::length(this->Position - body->Position), 2));
  this->Acceleration += force / (float)this->Mass;
}

void Body::Move(float dt) {
  this->Velocity += this->Acceleration * dt;
  this->Position += this->Velocity * dt;

  this->Acceleration = glm::vec2(0);
}
