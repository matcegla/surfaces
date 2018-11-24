#include "math.hpp"
#include <glm/glm.hpp>

glm::vec2 enorm(glm::vec2 v) { return glm::length(v) == 0.0f ? glm::vec2() : glm::normalize(v); }
glm::vec3 rgb(int r, int g, int b) { return glm::vec3(r, g, b) / 255.0f; }