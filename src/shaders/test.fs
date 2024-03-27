#version 330 core

in vec3 FragPos;

out vec4 FragColor;

uniform float u_time;
uniform float u_seed;
uniform float u_pixelSize;
uniform float u_slowedBy;

uniform vec3 u_offset;
uniform vec3 u_amplitude;
uniform vec3 u_frequency;
uniform vec3 u_phase;

void main() {
  FragColor = vec4(FragPos / 2 + 0.5, 1.0);
}
