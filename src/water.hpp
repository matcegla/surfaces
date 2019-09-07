#ifndef SURFACES_WATER_HPP
#define SURFACES_WATER_HPP

#include "xgl.hpp"

struct Water {
  VAO vao;
  VBO vbo;
  EBO ebo;
  Program shader;
  Uniform utime, upv, umodel, uviewpos;
  std::vector<float> vertices;
  std::vector<unsigned> indices;
  Water(int width, int depth, const std::string &vertName,
        const std::string &fragName, glm::vec3 sunPos);
  void draw(float time, const glm::mat4 &transPV, glm::vec3 viewPos,
            bool transparent);
};

#endif // SURFACES_WATER_HPP
