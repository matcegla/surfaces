#ifndef SURFACES_MODELS_HPP
#define SURFACES_MODELS_HPP

#include "xgl.hpp"

struct CubeVertices {
  VBO vbo;
  VAO vao;
  CubeVertices();
  void draw();
  static const float rawData[6 * 6 * 3];
};

struct QuadVertices {
  VBO vbo;
  VAO vao;
  QuadVertices();
  void draw(Texture &texture);
  static const float rawData[2 * 3 * 4];
};

#endif // SURFACES_MODELS_HPP
