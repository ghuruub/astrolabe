#version 330 core
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D u_sprite;

void main() {
  FragColor = texture(u_sprite, TexCoords);
}
