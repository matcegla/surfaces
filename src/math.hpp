#ifndef SURFACES_MATH_HPP
#define SURFACES_MATH_HPP

#include <glm/vec2.hpp>
#include <random>

glm::vec2 enorm(glm::vec2 v);
glm::vec3 rgb(int r, int g, int b);
extern std::minstd_rand rng; // NOLINT(cert-err58-cpp)
float uniform(float a, float b);
float volume(glm::vec3 v);

#endif // SURFACES_MATH_HPP
