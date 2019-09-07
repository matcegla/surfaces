#include "wave.hpp"
#include <cmath>

float waveHeightAtPoint(glm::vec3 vertex_pos, float time) {
  float wavePresence = (sin((vertex_pos.x + vertex_pos.z + time) / 16) + 1) / 2;
  float x = vertex_pos.z / 8 + vertex_pos.x / 32 + time / 2;
  float height = 4 * wavePresence * (sin(x) + sin(2 * x) + sin(3 * x));
  return height;
}
