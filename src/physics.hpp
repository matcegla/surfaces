#ifndef SURFACES_PHYSICS_HPP
#define SURFACES_PHYSICS_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <iostream>

struct Material {
	float density; // kg/m^3
};

struct ForceApplication2 {
	glm::vec2 point;
	glm::vec2 force;
	ForceApplication2(glm::vec2 point, glm::vec2 force);
	friend std::ostream& operator<<(std::ostream& out, const ForceApplication2& app);
};

struct RaftPhysics {
	glm::vec3 position;
	glm::vec2 velocity;
	glm::vec3 scale;
	float rotation;
	float angularVelocity;
	float mass;
	int probes;
	RaftPhysics(glm::vec3 position, glm::vec3 scale, float mass, int probes);
	void update(float deltaTime, float time);
	std::vector<ForceApplication2> computeForces(float time);
	float torqueFromForce(ForceApplication2 applied);
};

struct RaftPart {
	glm::vec3 position;
	glm::vec2 velocity;
	glm::vec3 scale;
	float rotation;
	float mass;
	RaftPart(const glm::vec3& position, const glm::vec2& velocity, const glm::vec3 &scale, float rotation, float mass);
	ForceApplication2 weight();
	ForceApplication2 buoyancy(float time);
	ForceApplication2 drag(float time);
};

glm::vec2 map2D(glm::vec3 v);

extern const glm::vec3 gravity;
extern const Material water;
extern const Material air;
extern const Material wood;

#endif //SURFACES_PHYSICS_HPP
