#version 330 core

in vec3 FragPos;

out vec4 FragColor;

uniform float u_time;
uniform int u_seed;
uniform float u_pixelSize;
uniform float u_slowedBy;

uniform vec3 u_offset;
uniform vec3 u_amplitude;
uniform vec3 u_frequency;
uniform vec3 u_phase;

vec3 offset = vec3(0.193, 0.287, 0.335);
vec3 amplitude = vec3(0.759, 0.782, 0.844);
vec3 frequency = vec3(0.968, 0.018, 0.242);
vec3 phase = vec3(1.274, 4.403, 3.709);

vec3 gradient(float t) {
  return offset + amplitude * cos(6.28318 * (frequency * t + phase));
}

float fade(float t) {
  return t * t * t * (t * (6 * t - 15) + 10);
}

vec3 GetConstantVector(int x, int y, int z) {
  int w = 8 * u_seed * (~0);
  int s = w / 2;
  int a = x;
  int b = y;
  int c = z;

  a *= 3284157443; b ^= a << s | a >> w-s;
  b *= 1911520717; a ^= b << s | b >> w-s;
  a *= 2048419325;

  c *= 3284157443; b ^= c << s | c >> w-s;
  b *= 1911520717; c ^= b << s | b >> w-s;
  c *= 2048419325;

  float phi = (a) * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
  float theta = (c) * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
  vec3 v;
  v.y = cos(theta);
  v.x = cos(phi) * sin(theta); 
  v.z = sin(phi) * sin(theta);
  return v;
}

float mix2d(float p00, float p01, float p10, float p11, vec3 pos) {
  float m0 = mix(p00, p01, fade(pos.y));
  float m1 = mix(p10, p11, fade(pos.y));

  return mix(m0, m1, fade(pos.x));
}

float noise(vec3 pos) {
  int ix = int(floor(pos.x));
  int iy = int(floor(pos.y));
  int iz = int(floor(pos.z));

  vec3 ipos = vec3(ix, iy, iz);
  vec3 fpos = pos - ipos;

  // vectors to vertices
  vec3 v000 = -vec3(0, 0, 0) + fpos;
  vec3 v100 = -vec3(1, 0, 0) + fpos;
  vec3 v010 = -vec3(0, 1, 0) + fpos;
  vec3 v110 = -vec3(1, 1, 0) + fpos;
  vec3 v001 = -vec3(0, 0, 1) + fpos;
  vec3 v101 = -vec3(1, 0, 1) + fpos;
  vec3 v011 = -vec3(0, 1, 1) + fpos;
  vec3 v111 = -vec3(1, 1, 1) + fpos;

  // random vectors in vertices
  vec3 r000 = GetConstantVector(ix + 0, iy + 0, iz + 0);
  vec3 r100 = GetConstantVector(ix + 1, iy + 0, iz + 0);
  vec3 r010 = GetConstantVector(ix + 0, iy + 1, iz + 0);
  vec3 r110 = GetConstantVector(ix + 1, iy + 1, iz + 0);
  vec3 r001 = GetConstantVector(ix + 0, iy + 0, iz + 1);
  vec3 r101 = GetConstantVector(ix + 1, iy + 0, iz + 1);
  vec3 r011 = GetConstantVector(ix + 0, iy + 1, iz + 1);
  vec3 r111 = GetConstantVector(ix + 1, iy + 1, iz + 1);

  // dot products xyz
  float d000 = dot(v000, r000);
  float d100 = dot(v100, r100);
  float d010 = dot(v010, r010);
  float d110 = dot(v110, r110);
  float d001 = dot(v001, r001);
  float d101 = dot(v101, r101);
  float d011 = dot(v011, r011);
  float d111 = dot(v111, r111);

  // mixing values in vertices of cube
  float upper_mix = mix2d(d000, d010, d100, d110, fpos);
  float lower_mix = mix2d(d001, d011, d101, d111, fpos);

  return mix(upper_mix, lower_mix, fade(fpos.z));
}

void main() {
  // multiply by 2 because the size of quad is 1x1 (instead of 2x2)
  vec3 pos = FragPos * 2;

  //float noiseSize = 7;
  float noiseSize = u_pixelSize * 7 / 10;

  // pixelize coordinates
  pos *= u_pixelSize;
  pos = floor(pos);
  pos /= u_pixelSize;

  // fixing offset by 1 pixel
  pos += vec3(0.5 / u_pixelSize, 0.5 / u_pixelSize, 0);

  // drawing only those that lie in circle
  if (length(pos) > 0.99) {
    FragColor = vec4(0.0);
  } else {
    // genetate noise 
    float col = fade(noise(vec3(vec2(pos / 2 + 0.5) * noiseSize, u_time / u_slowedBy)) / 2.0 + 0.5);

    FragColor = vec4(gradient(col), 1.0);
  }
}
