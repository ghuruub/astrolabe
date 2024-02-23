#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
  public:
    Camera();

    glm::mat4 GetViewMatrix();
    
};

#endif 
