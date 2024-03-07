#include "Body.hpp"

Body::Body(unsigned int mass, glm::vec2 pos, float radius, Texture2D texture,
           glm::vec2 velocity)
    : Mass(mass), Position(pos), Size(glm::vec2(1.0f) * radius), Texture(texture),
      Velocity(velocity), Acceleration(glm::vec2(0)) {}

void Body::ApplyForces(Body *body) {
  glm::vec2 force = body->Position - Position;
  force *= (double)(Factor * body->Mass /
                    pow(glm::length(Position - body->Position), 2));
  Acceleration += force;
}

void Body::Move(float dt) {
  if (Acceleration.length() > 0.001f) {
    Velocity += Acceleration * dt;
  }
  if (Velocity.length() > 0.001f) {
    Position += Velocity * dt;
  }

  Acceleration = glm::vec2(0);
}
