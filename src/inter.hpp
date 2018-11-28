#ifndef SURFACES_INTER_HPP
#define SURFACES_INTER_HPP

#include "xgl.hpp"

struct Inter {
	Inter(int width, int height);
	void bind();
	void unbind();
	Texture texture;
	RBO rbo;
	FBO fbo;
};

#endif //SURFACES_INTER_HPP
