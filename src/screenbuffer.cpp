#include "screenbuffer.hpp"
#include <glm/glm.hpp>

Screenbuffer::Screenbuffer(const std::string& vert, const std::string& frag, QuadVertices& quad):
	quad(quad),
	shader(shaderProgramFromAsset(vert, frag)),
	upos1(shader.locateUniform("pos1")),
	upos2(shader.locateUniform("pos2"))
{
	shader.use();
}

void Screenbuffer::render(float x1, float y1, float x2, float y2, Texture& texture) {
	shader.use();
	upos1 = glm::vec2(x1, y1);
	upos2 = glm::vec2(x2, y2);
	quad.draw(texture);
}

void Screenbuffer::prepare() {
	xclear(glm::vec3(1.0f), GL_COLOR_BUFFER_BIT);
}
