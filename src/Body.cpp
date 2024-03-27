#include "Body.hpp"
#include <chrono>

Body::Body(unsigned int mass, glm::vec2 pos, float radius, glm::vec2 velocity,
           Shader *shader)
    : Mass(mass), Position(pos), Size(radius), Velocity(velocity),
      Acceleration(glm::vec2(0)), Immovable(true), AtmosphereSpeed(3), BodyShader(shader) {
  Seed = std::chrono::duration_cast<std::chrono::seconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();
}

void Body::ApplyForces(Body *body) {
  glm::vec2 force = body->Position - Position;
  force *= (double)(Factor * body->Mass /
                    pow(glm::length(Position - body->Position), 2));
  Acceleration += force;
}

void Body::Move(float dt) {
  if (Immovable) {
    return;
  }
  if (Acceleration.length() > 0.001f) {
    Velocity += Acceleration * dt;
  }
  if (Velocity.length() > 0.001f) {
    Position += Velocity * dt;
  }

  Acceleration = glm::vec2(0);
}

bool Body::CheckCollision(Body *body) {
  return (Position - body->Position).length() < (body->Size + Size);
}
