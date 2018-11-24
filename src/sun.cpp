#include "sun.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Sun::Sun(glm::vec3 position, glm::vec3 scale, const std::string& vertPath, const std::string& fragPath, CubeVertices& cubev):
		cubev(cubev),
		shader(shaderProgramFromFiles(vertPath, fragPath)),
		upv(shader.locateUniform("trans_pv")),
		umodel(shader.locateUniform("trans_model")),
		position(position),
		scale(scale)
{}
void Sun::draw(const glm::mat4& transPV) {
	auto model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, scale);
	shader.use();
	upv = transPV;
	umodel = model;
	cubev.draw();
}