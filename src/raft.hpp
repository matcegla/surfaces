#ifndef SURFACES_RAFT_HPP
#define SURFACES_RAFT_HPP

#include "models.hpp"
#include "physics.hpp"

struct Raft {
	CubeVertices& cubev;
	Program shader;
	Uniform upv, umodel;
	RaftPhysics physics;
	Raft(glm::vec3 position, float mass, glm::vec3 scale, const std::string& vertPath, const std::string& fragPath, CubeVertices& cubev);
	void update(float deltaTime, float time, glm::vec2 externalForce);
	void draw(const glm::mat4& transPV);
};

#endif //SURFACES_RAFT_HPP
