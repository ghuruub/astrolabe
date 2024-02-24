#include "GameManager.hpp"

#include "stb_image.h"

GameManager::GameManager(unsigned int width, unsigned int height)
    : Width(width), Height(height) {}

void GameManager::Init() {
  Shader shader;
  shader.LoadFromFile("../src/shaders/sprite.vs", "../src/shaders/sprite.fs");

  renderer = new Renderer(shader);

  Texture2D texture;

  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("../src/assets/box.jpg", &width, &height, &nrChannels, 0);
  texture.Generate(width, height, data);
  stbi_image_free(data);

  CreateBody(50000, glm::vec2(Width / 2.0f - 200.0f, Height / 2.0f),
             glm::vec2(100.0f, 100.0f), texture, glm::vec2(0.0f, -250.0f));
  CreateBody(50000, glm::vec2(Width / 2.0f + 200.0f, Height / 2.0f),
             glm::vec2(100.0f, 100.0f), texture, glm::vec2(0.0f, 250.0f));
  CreateBody(10, glm::vec2(Width / 2.0f, Height / 2.0f + 100.0f),
             glm::vec2(50.0f, 50.0f), texture, glm::vec2(20.0f, 0.0f));
}

void GameManager::Update(float dt) {
  ReapplyForces();
  MoveBodies(dt);
}

void GameManager::Render() {
  for (Body *body : bodies) {
    renderer->Render(body->Texture, body->Position, body->Size);
  }
}

void GameManager::ReapplyForces() {
  for (int i = 0; i < bodies.size(); i++) {
    for (int j = i + 1; j < bodies.size(); j++) {
      bodies[i]->ApplyForces(bodies.at(j));
      bodies[j]->ApplyForces(bodies.at(i));
    }
  }
}

void GameManager::MoveBodies(float dt) {
  for (int i = 0; i < bodies.size(); i++) {
    bodies[i]->Move(dt);
  }
}

void GameManager::CreateBody(unsigned int mass, glm::vec2 pos, glm::vec2 size,
                             Texture2D texture, glm::vec2 velocity) {
  Body *body = new Body(mass, pos, size, texture, velocity);
  bodies.push_back(body);
}

void GameManager::RemoveBody(Body *body) {
  for (int i = 0; i < bodies.size(); i++) {
    if (body == bodies[i]) {
      delete bodies[i];
      bodies.erase(bodies.begin() + i);
    }
  }
}
