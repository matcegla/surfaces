#include "raft.hpp"
#include "models.hpp"
#include "xgl.hpp"
#include "physics.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Raft::Raft(glm::vec3 position, float mass, glm::vec3 scale, int probes, const std::string& vertPath, const std::string& fragPath, CubeVertices& cubev):
		cubev(cubev),
		shader(shaderProgramFromFiles(vertPath, fragPath)),
		upv(shader.locateUniform("trans_pv")),
		umodel(shader.locateUniform("trans_model")),
		physics(position, scale, mass, probes)
{}
void Raft::update(float deltaTime, float time, glm::vec2 externalForce, float externalTorque) {
	physics.update(deltaTime, time, externalForce, externalTorque);
}
void Raft::draw(const glm::mat4& transPV) {
	auto model = glm::mat4(1.0f);
	model = glm::translate(model, physics.position);
	model = glm::rotate(model, -physics.rotation, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, physics.scale);
	shader.use();
	upv = transPV;
	umodel = model;
	cubev.draw();
}