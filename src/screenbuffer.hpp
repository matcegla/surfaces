#ifndef SURFACES_INTERFRAMEBUFFER_HPP
#define SURFACES_INTERFRAMEBUFFER_HPP

#include <string>
#include "xgl.hpp"
#include "models.hpp"

struct Screenbuffer {
	Screenbuffer(const std::string& vert, const std::string& frag, QuadVertices& quad);
	void prepare();
	void render(float x1, float y1, float x2, float y2, Texture& texture);
	QuadVertices& quad;
	Program shader;
	Uniform upos1;
	Uniform upos2;
};

#endif //SURFACES_INTERFRAMEBUFFER_HPP
