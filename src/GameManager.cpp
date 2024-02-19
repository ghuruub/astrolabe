#include "GameManager.hpp"

void GameManager::Update(float dt) {
  ReapplyForces();
}

void GameManager::ReapplyForces() {
  for (int i = 0; i < bodies.size(); i++) {
    for (int j = i; j < bodies.size(); j++) {
      bodies.at(i)->ApplyForces(bodies.at(j));
      bodies.at(j)->ApplyForces(bodies.at(i));
    }
  }
}

void GameManager::CreateBody(unsigned int mass, glm::vec2 pos, glm::vec2 velocity) {
  Body body(mass, pos, velocity);
  bodies.push_back(&body);
}

void GameManager::RemoveBody(Body* body) {
  for (int i = 0; i < bodies.size(); i++) {
    if (body == bodies[i]) {
      delete bodies[i];
      bodies.erase(bodies.begin() + i);
    } 
  }
}
