#include "wave.hpp"
#include <cmath>

float waveHeightAtPoint(glm::vec3 vertex_pos, float time) {
	float wavePresence = (sinf((vertex_pos.x + vertex_pos.z + time)/16) + 1) / 2;
	float x = vertex_pos.z / 8 + vertex_pos.x / 32 + time / 2;
	float height = 4 * wavePresence * (sinf(x) + sinf(2*x) + sinf(3*x));
	return height;
}
