#ifndef SURFACES_SUN_HPP
#define SURFACES_SUN_HPP

#include "models.hpp"
#include "xgl.hpp"

struct Sun {
  CubeVertices &cubev;
  Program shader;
  Uniform upv, umodel;
  glm::vec3 position;
  glm::vec3 scale;
  Sun(glm::vec3 position, glm::vec3 scale, const std::string &vertName,
      const std::string &fragName, CubeVertices &cubev);
  void draw(const glm::mat4 &transPV);
};

#endif // SURFACES_SUN_HPP
