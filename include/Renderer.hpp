#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Texture2D.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

class Renderer {
public:
  Renderer(Shader &shader, Camera* camera);
  ~Renderer();

  void Render(Texture2D &texture, glm::vec2 pos, glm::vec2 size);

private:
  unsigned int quadVAO;
  Shader bindedShader;
  Camera* camera;

  void initRenderData();
};

#endif
