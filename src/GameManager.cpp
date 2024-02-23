#include "GameManager.hpp"

#include "stb_image.h"
#include <iostream>

void GameManager::Init() {
  Shader shader;
  shader.LoadFromFile("../src/shaders/sprite.vs", "../src/shaders/sprite.fs");

  renderer = new Renderer(shader);

  Texture2D texture;

  int width, height, nrChannels;
  unsigned char *data =
      stbi_load("../src/box.jpg", &width, &height, &nrChannels, 0);
  texture.Generate(width, height, data);
  stbi_image_free(data);

  CreateBody(4, glm::vec2(.4f, .4f), glm::vec2(1.0f, 1.0f), texture,
             glm::vec2(0.0f, 0.0f));
}

void GameManager::Update(float dt) { ReapplyForces(); }

void GameManager::Render() {
  // for (Body *body : bodies) {
  //   renderer->Render(body->Texture, body->Position, body->Size);
  // }
  renderer->Render(bodies[0]->Texture, bodies[0]->Position, bodies[0]->Size);
}

void GameManager::ReapplyForces() {
  for (int i = 0; i < bodies.size(); i++) {
    for (int j = i + 1; j < bodies.size(); j++) {
      bodies[i]->ApplyForces(bodies.at(j));
      bodies[j]->ApplyForces(bodies.at(i));
    }
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
