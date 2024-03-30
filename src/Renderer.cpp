#include "Renderer.hpp"
#include "Body.hpp"
#include "GLFW/glfw3.h"

Renderer::Renderer(Shader &shader, Camera *camera) {
  this->bindedShader = shader;
  this->camera = camera;
  initRenderData();
}

Renderer::~Renderer() { glDeleteVertexArrays(1, &quadVAO); }

void Renderer::RenderBody(Body *body) {
  body->BodyShader->Use();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(body->Position, 0.0f));
  model = glm::scale(model, glm::vec3(glm::vec2(body->Size * 20), 1.0f));

  glm::mat4 view = camera->GetViewMatrix();

  glm::mat4 projection =
      glm::ortho(500.0f / camera->Zoom, -500.0f / camera->Zoom,
                 -300.0f / camera->Zoom, 300.0f / camera->Zoom, -1.0f, 1.0f);

  body->BodyShader->SetMatrix4("u_model", model);
  body->BodyShader->SetMatrix4("u_view", view);
  body->BodyShader->SetMatrix4("u_projection", projection);

  body->BodyShader->SetFloat("u_time", glfwGetTime());
  body->BodyShader->SetInteger("u_seed", body->Seed);
  body->BodyShader->SetFloat("u_pixelSize", body->Size);
  body->BodyShader->SetFloat("u_slowedBy", body->AtmosphereSpeed);

  body->BodyShader->SetVector3f("u_offset", body->Pallete[0]);
  body->BodyShader->SetVector3f("u_amplitude", body->Pallete[1]);
  body->BodyShader->SetVector3f("u_frequency", body->Pallete[2]);
  body->BodyShader->SetVector3f("u_phase", body->Pallete[3]);

  body->BodyShader->SetFloat("u_alpha", body->Alpha);

  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
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
