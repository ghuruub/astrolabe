#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Body.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

class Renderer {
public:
  int Width;
  int Height;


  Renderer(Shader &shader, Camera* camera);
  ~Renderer();

  void Render(glm::vec2 pos, glm::vec2 size);
  void RenderBody(Body* body);
  void RenderTrail(Body* body);

private:
  unsigned int quadVAO;
  Shader bindedShader;
  Shader trailShader;
  Camera* camera;

  void initRenderData();
};

#endif
