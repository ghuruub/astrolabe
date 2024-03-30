#include "GameManager.hpp"
#include "Body.hpp"
#include "Camera.hpp"
#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
#include "imgui.h"

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
  camera = new Camera(glm::vec2(0.0f, 0.0f), Width, Height);
  renderer = new Renderer(shader, camera);
}

void GameManager::Update(float dt) {
  ReapplyForces();
  MoveBodies(dt);
}

void GameManager::Render() {
  for (Body *body : bodies) {
    renderer->RenderBody(body);
  }

  if (bodyCreation) {
    renderer->RenderBody(bodyBuffer);
    BodyCreationEffects();
  }
}

void GameManager::ReapplyForces() {
  for (int i = 0; i < bodies.size(); i++) {
    if (bodies[i]->Immovable) {
      continue;
    }
    for (int j = i + 1; j < bodies.size(); j++) {
      if (bodies[j]->Immovable) {
        continue;
      }
      bodies[i]->ApplyForces(bodies.at(j));
      bodies[j]->ApplyForces(bodies.at(i));
    }
  }
}

void GameManager::ReapplyForcesTo(Body *body) {
  for (int i = 0; i < bodies.size(); i++) {
    body->ApplyForces(bodies[i]);
  }
}

void GameManager::MoveBodies(float dt) {
  for (int i = 0; i < bodies.size(); i++) {
    bodies[i]->Move(dt);
  }
}

Body *GameManager::CreateBody(unsigned int mass, glm::vec2 pos, float radius,
                              glm::vec2 velocity) {
  Shader *shader = new Shader();
  shader->LoadFromFile("../src/shaders/sprite.vs", "../src/shaders/planet.fs");

  Body *body = new Body(mass, pos, radius, velocity, shader);
  return body;
}

void GameManager::RemoveBody(Body *body) {
  for (int i = 0; i < bodies.size(); i++) {
    if (body == bodies[i]) {
      delete bodies[i];
      bodies.erase(bodies.begin() + i);
      return;
    }
  }
  delete body;
}

void GameManager::ProcessKeyAction(int keycode, float dt, bool shift) {
  if (keycode == GLFW_KEY_W) {
    camera->ProcessKeyboard(Up, shift, dt);
  }
  if (keycode == GLFW_KEY_A) {
    camera->ProcessKeyboard(Left, shift, dt);
  }
  if (keycode == GLFW_KEY_S) {
    camera->ProcessKeyboard(Down, shift, dt);
  }
  if (keycode == GLFW_KEY_D) {
    camera->ProcessKeyboard(Right, shift, dt);
  }
}

void GameManager::ProcessScrollAction(double yoffset, float time) {
  camera->ProcessMouseScroll(yoffset, time);
}

void GameManager::ProcessMouseAction(int button, int action, double xpos,
                                     double ypos) {
  // From screen to world coordinates
  glm::vec3 pos = glm::vec3(xpos, -ypos, 0);
  pos -= glm::vec3(Width / 2.0f, Height / -2.0f, 0);
  pos /= camera->Zoom;
  pos += camera->Position;

  // add state
  if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT &&
      !bodyCreation) {
    leftClickPos = pos;
    rightClickPos = leftClickPos;
    bodyCreation = true;
    bodyBuffer = CreateBody(0, leftClickPos, 6, glm::vec3(0));
    while (!previousColor.empty()) {
      previousColor.pop();
    }
  }

  if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT &&
      bodyCreation) {
    glm::vec2 velocity = leftClickPos - pos;
    rightClickPos = pos;
    bodyBuffer->Velocity = velocity * bodySpeedSensitivity;
  }
}

void GameManager::DrawUI() {
  if (bodyCreation) {
    glm::vec3 screenPos = leftClickPos;
    screenPos -= camera->Position;
    screenPos *= camera->Zoom;

    ImGui::SetNextWindowPos(
        ImVec2(screenPos.x + 100 + Width / 2.0, -screenPos.y + Height / 2.0),
        ImGuiCond_Appearing);

    ImGui::Begin("Body creation");

    ImGui::Checkbox("Massive", &uiMassive);

    if (uiMassive && bodyBuffer->Mass < 10000) {
      bodyBuffer->Mass = 10000;
    } else if (!uiMassive && bodyBuffer->Mass > 100) {
      bodyBuffer->Mass = 100;
    }

    if (uiMassive) {
      ImGui::SliderInt("Mass", &bodyBuffer->Mass, 10000, 1000000);
    } else {
      ImGui::SliderInt("Mass", &bodyBuffer->Mass, 0, 100);
    }
    ImGui::SliderInt("Size", &bodyBuffer->Size, 2, 60);
    ImGui::SliderInt("Shadows", &shadowsAmout, 3, 100);

    if (ImGui::Button("Regenerate")) {
      previousColor.push(bodyBuffer->Pallete);
      bodyBuffer->Pallete = bodyBuffer->GeneratePallete();
    }
    ImGui::SameLine();
    bool disabled;
    if (previousColor.empty()) {
      ImGui::BeginDisabled();
      disabled = true;
    }
    if (ImGui::Button("Previous")) {
      bodyBuffer->Pallete = previousColor.top();
      previousColor.pop();
    }
    if (disabled) {
      ImGui::EndDisabled();
    }

    if (ImGui::Button("Confirm")) {
      bodyBuffer->Immovable = false;
      bodies.push_back(bodyBuffer);
      bodyCreation = false;
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      RemoveBody(bodyBuffer);
      bodyCreation = false;
    }

    ImGui::End();
  }
}

void GameManager::BodyCreationEffects() {
  if (!bodyCreation || bodyBuffer->Velocity == glm::vec2(0)) {
    return;
  }
  std::vector<Body> shadows;
  for (int i = 0; i < shadowsAmout * 5; i++) {
    if (i == 0) {
      shadows.push_back(Body(*bodyBuffer));
    } else {
      shadows.push_back(Body(shadows[i - 1]));
    }
    if (i % 5 == 0) {
      shadows[i].Alpha = 1 - 0.3f * i / (shadowsAmout * 5);
    } else {
      shadows[i].Alpha = 0;
    }
    ReapplyForcesTo(&shadows[i]);
    if (i != 0) {
      shadows[i].Position = shadows[i - 1].Position;
    }
    shadows[i].Move(0.1f);
    renderer->RenderBody(&shadows[i]);
  }
}
