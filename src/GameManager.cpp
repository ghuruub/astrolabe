#include "GameManager.hpp"
#include "Camera.hpp"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GameManager::GameManager(unsigned int width, unsigned int height)
    : Width(width), Height(height) {}

GameManager::~GameManager() {
  delete camera;
  delete renderer;
  for (Body *body : bodies) {
    delete body;
  }
}

void GameManager::Init() {
  Shader shader;
  //shader.LoadFromFile("../src/shaders/sprite.vs", "../src/shaders/sprite.fs");

  camera = new Camera(glm::vec2(0.0f, 0.0f), Width, Height);
  renderer = new Renderer(shader, camera);

  CreateBody(40000, glm::vec2(0.0f, 0.0f), 7, glm::vec2(0.0f, 0.0f));
}

void GameManager::Update(float dt) {
  ReapplyForces();
  MoveBodies(dt);
}

void GameManager::Render() {
  for (Body *body : bodies) {
    renderer->RenderBody(body);
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

Body *GameManager::CreateBody(unsigned int mass, glm::vec2 pos, float radius,
                              glm::vec2 velocity) {
  Shader* shader = new Shader();
  shader->LoadFromFile("../src/shaders/sprite.vs", "../src/shaders/planet.fs");

  Body *body = new Body(mass, pos, radius, velocity, shader);
  bodies.push_back(body);
  return body;
}

void GameManager::RemoveBody(Body *body) {
  for (int i = 0; i < bodies.size(); i++) {
    if (body == bodies[i]) {
      delete bodies[i];
      bodies.erase(bodies.begin() + i);
    }
  }
}

void GameManager::ProcessKeyAction(int keycode, float dt) {
  bool sprint = false;
  if (keycode == GLFW_KEY_LEFT_SHIFT) {
    sprint = true;
  }
  if (keycode == GLFW_KEY_W) {
    camera->ProcessKeyboard(Up, sprint, dt);
  }
  if (keycode == GLFW_KEY_A) {
    camera->ProcessKeyboard(Left, sprint, dt);
  }
  if (keycode == GLFW_KEY_S) {
    camera->ProcessKeyboard(Down, sprint, dt);
  }
  if (keycode == GLFW_KEY_D) {
    camera->ProcessKeyboard(Right, sprint, dt);
  }
}

void GameManager::ProcessScrollAction(double yoffset, float time) {
  camera->ProcessMouseScroll(yoffset, time);
}

void GameManager::ProcessMouseAction(int button, int action, int mod,
                                     double xpos, double ypos) {
  // From screen to world coordinates
  glm::vec3 pos = glm::vec3(xpos, -ypos, 0);
  pos -= glm::vec3(Width / 2.0f, Height / -2.0f, 0);
  pos /= camera->Zoom;
  pos += camera->Position;

  // add state
  if (action == GLFW_PRESS && !lButtonDown) {
    clickPos = lButtonDown ? clickPos : pos;
    lButtonDown = true;
    bodyBuffer = CreateBody(80, clickPos, 50, glm::vec3(0));
  }

  if (action == GLFW_RELEASE) {
    glm::vec3 velocity = clickPos - pos ;
    velocity *= 4;
    lButtonDown = false;

    bodyBuffer->Velocity = velocity;
    bodyBuffer->Immovable = false;
  }
}
