#ifndef SURFACES_INTERFRAMEBUFFER_HPP
#define SURFACES_INTERFRAMEBUFFER_HPP

#include <string>
#include "xgl.hpp"
#include "models.hpp"

struct InterFramebuffer {
	InterFramebuffer(int width, int height, const std::string& vert, const std::string& frag, QuadVertices& quad);
	void prepare();
	void copy();
	Texture texture;
	RBO rbo;
	FBO fbo;
	QuadVertices& quad;
	Program shader;
};

#endif //SURFACES_INTERFRAMEBUFFER_HPP
