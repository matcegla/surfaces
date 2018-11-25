#include "interframebuffer.hpp"

InterFramebuffer::InterFramebuffer(int width, int height, const std::string& vert, const std::string& frag, QuadVertices& quad):
	texture(),
	rbo(),
	fbo(),
	quad(quad),
	shader(shaderProgramFromFiles(vert, frag))
{
	texture.bind(GL_TEXTURE_2D);
	texture.image2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	texture.parameter(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture.parameter(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	texture.unbind(GL_TEXTURE_2D);
	rbo.bind(GL_RENDERBUFFER);
	rbo.storage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	rbo.unbind(GL_RENDERBUFFER);
	fbo.bind(GL_FRAMEBUFFER);
	fbo.texture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
	fbo.renderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	fbo.xassertComplete(GL_FRAMEBUFFER);
	fbo.unbind(GL_FRAMEBUFFER);
}

void InterFramebuffer::prepare() {
	fbo.bind(GL_FRAMEBUFFER);
	glEnable(GL_DEPTH_TEST);

}

void InterFramebuffer::copy() {
	fbo.unbind(GL_FRAMEBUFFER);
	xclear(glm::vec3(1.0f), GL_COLOR_BUFFER_BIT);
	shader.use();
	glDisable(GL_DEPTH_TEST);
	quad.draw(texture);
}
