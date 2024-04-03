#include "Body.hpp"
#include "GLFW/glfw3.h"
#include <iostream>
#include <random>

Body::Body(unsigned int mass, glm::vec2 pos, float radius, glm::vec2 velocity)
    : Mass(mass), Position(pos), Size(radius), Velocity(velocity),
      Acceleration(glm::vec2(0)), Immovable(true), AtmosphereSpeed(3) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(1, INT_MAX);
  Seed = dist(gen);

  Pallete = GeneratePallete();
}

void Body::ApplyForces(Body *body) {
  if (Mass == 0) {
    return;
  }
  glm::vec2 force = body->Position - Position;
  force *= (double)(Factor * body->Mass /
                    pow(glm::length(Position - body->Position), 2));
  Acceleration += force;
}

void Body::Move(float dt) {
  if (Immovable && !shadow) {
    return;
  }
  if (Acceleration.length() > 0.001f) {
    Velocity += Acceleration * dt;
  }
  if (Velocity.length() > 0.001f) {
    Position += Velocity * dt;
  }

  if (glfwGetTime() - previousPositionTime > HistoryPeriod && !shadow) {
    previousPositionTime = glfwGetTime();
    PositionHistory.push_back(Position);
    if (PositionHistory.size() > HistoryCapacity) {
      PositionHistory.erase(PositionHistory.begin());
    }
  }

  Acceleration = glm::vec2(0);
}

bool Body::CheckCollision(Body *body) {
  float distance = (Position - body->Position).length();
  bool collision = distance < (body->Size + Size);
  return collision;
}

void Body::Collide(Body* body) {
}

std::array<glm::vec3, 4> Body::GeneratePallete() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dist(0, 1);

  float pi = 3.14159265358979323f;

  std::array<glm::vec3, 4> pallete;

  for (int i = 0; i < 3; i++) {
    float offset = dist(gen);
    float amplitude = dist(gen);
    float frequency = dist(gen) * pi / 2;
    float phase = dist(gen) * pi * 2;

    pallete[0][i] = offset;
    pallete[1][i] = amplitude;
    pallete[2][i] = frequency;
    pallete[3][i] = phase;
  }

  return pallete;
}

Body::Body(const Body &body) {
  Mass = body.Mass;
  Size = body.Size;
  Immovable = true;

  Position = body.Position;
  Velocity = body.Velocity;

  Seed = body.Seed;
  AtmosphereSpeed = body.AtmosphereSpeed;
  Pallete = body.Pallete;

  shadow = true;
}
