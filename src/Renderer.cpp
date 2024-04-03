#include "Renderer.hpp"
#include "Body.hpp"
#include "GLFW/glfw3.h"

Renderer::Renderer(Shader &shader, Camera *camera) {
  this->bindedShader = shader;
  this->camera = camera;
  
  Width = camera->ScreenWidth;
  Height = camera->ScreeHeight;

  trailShader.LoadFromFile("../src/shaders/sprite.vs",
                           "../src/shaders/trail.fs");
  initRenderData();
}

Renderer::~Renderer() { glDeleteVertexArrays(1, &quadVAO); }

void Renderer::RenderBody(Body *body) {
  RenderTrail(body);

  bindedShader.Use();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(body->Position, 0.0f));
  model = glm::scale(model, glm::vec3(glm::vec2(body->Size * 20), 1.0f));

  glm::mat4 view = camera->GetViewMatrix();

  glm::mat4 projection =
      glm::ortho((Width / 2.0f) / camera->Zoom, -(Width / 2.0f) / camera->Zoom,
                 -(Height / 2.0f) / camera->Zoom, (Height / 2.0f) / camera->Zoom, -1.0f, 1.0f);

  bindedShader.SetMatrix4("u_model", model);
  bindedShader.SetMatrix4("u_view", view);
  bindedShader.SetMatrix4("u_projection", projection);

  bindedShader.SetFloat("u_time", glfwGetTime());
  bindedShader.SetInteger("u_seed", body->Seed);
  bindedShader.SetFloat("u_pixelSize", body->Size);
  bindedShader.SetFloat("u_slowedBy", body->AtmosphereSpeed);

  bindedShader.SetVector3f("u_offset", body->Pallete[0]);
  bindedShader.SetVector3f("u_amplitude", body->Pallete[1]);
  bindedShader.SetVector3f("u_frequency", body->Pallete[2]);
  bindedShader.SetVector3f("u_phase", body->Pallete[3]);

  bindedShader.SetFloat("u_alpha", body->Alpha);

  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void Renderer::RenderTrail(Body *body) {
  if (body->Immovable) {
    return;
  }

  trailShader.Use();

  glm::mat4 model = glm::mat4(1.0f);

  glm::mat4 view = camera->GetViewMatrix();

  glm::mat4 projection =
      glm::ortho((Width / 2.0f) / camera->Zoom, -(Width / 2.0f) / camera->Zoom,
                 -(Height / 2.0f) / camera->Zoom, (Height / 2.0f) / camera->Zoom, -1.0f, 1.0f);

  trailShader.SetMatrix4("u_view", view);
  trailShader.SetMatrix4("u_projection", projection);

  for (int i = body->PositionHistory.size() - 1; i >= 0; i--) {
    model = glm::translate(model, glm::vec3(body->PositionHistory[i], 0.0f));
    model = glm::scale(model, glm::vec3(glm::vec2(15), 1.0f));
    trailShader.SetMatrix4("u_model", model);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    model = glm::mat4(1.0f);
  }
}

void Renderer::initRenderData() {
  unsigned int VBO;
  float vertices[] = {
      // pos
      -0.5f, 0.5f,  //
      0.5f,  -0.5f, //
      -0.5f, -0.5f, //
      -0.5f, 0.5f,  //
      0.5f,  0.5f,  //
      0.5f,  -0.5f  //
  };

  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(quadVAO);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
