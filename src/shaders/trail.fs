#version 330 core

in vec3 FragPos;

out vec4 FragColor;

uniform float u_time;
uniform float u_creation;
uniform int u_capacity;

void main() {
  vec2 pos = vec2(FragPos);
  float pixelSize = 4;
  pos *= pixelSize;
  pos = floor(pos);
  pos /= pixelSize;

  pos += vec2(0.5 / pixelSize);

  if (length(pos) < 0.5) {
    FragColor = vec4(vec3(0.7), 1.0);
  }
}
