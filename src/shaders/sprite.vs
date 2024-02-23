#version 330 core
layout (location = 0) in vec4 data;

out vec2 TexCoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
  TexCoords = data.zw;
  gl_Position = u_projection * u_view * u_model * vec4(data.xy, 0, 1.0);
}
