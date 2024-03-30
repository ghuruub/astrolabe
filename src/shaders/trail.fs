#version 330 core

in vec3 FragPos;

out vec4 FragColor;

void main() {
  if (length(FragPos) < 0.5) {
    FragColor = vec4(vec3(0.7), 1.0);
  }
}
