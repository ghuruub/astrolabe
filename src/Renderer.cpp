#include "Renderer.hpp"
#include <iostream>

Renderer::Renderer(Shader &shader) {
  this->bindedShader = shader;
  initRenderData();
}

Renderer::~Renderer() {
  glDeleteVertexArrays(1, &quadVAO);
}

void Renderer::Render(Texture2D &texture, glm::vec2 pos, glm::vec2 size) {
  bindedShader.Use();

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(pos, 0.0f));
  model = glm::scale(model, glm::vec3(size, 1.0f));

  // TODO remove identity matrix
  // glm::mat4 view = camera.GetViewMatrix();
  glm::mat4 view = glm::mat4(1.0f);

  // TODO remove calculation of projection matrix from this method
  glm::mat4 projection = glm::ortho(0.0f, 1000.0f, 600.0f, 0.0f, -1.0f, 1.0f);

  bindedShader.SetMatrix4("u_model", model);
  bindedShader.SetMatrix4("u_view", view);
  bindedShader.SetMatrix4("u_projection", projection);

  glActiveTexture(GL_TEXTURE0);
  texture.Bind();

  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

void Renderer::initRenderData() {
  unsigned int VBO;
  float vertices[] = {
    // pos      // tex
        -0.5f,  0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 

        -0.5f,  0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, 1.0f, 0.0f
  };

  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(quadVAO);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
