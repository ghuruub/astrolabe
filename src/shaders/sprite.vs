#version 330 core
layout (location = 0) in vec2 data;

out vec3 FragPos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
  FragPos = vec3(vec4(data, 0, 1.0));
  gl_Position = u_projection * u_view * u_model * vec4(FragPos, 1.0);
}
