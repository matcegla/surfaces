#ifndef SURFACES_PHYSICS_HPP
#define SURFACES_PHYSICS_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

extern const glm::vec3 gravity;
extern const float waterDensity;
extern const float airDensity;
extern const float woodDensity;

struct RaftPhysics {
	glm::vec3 position;
	glm::vec2 velocity;
	glm::vec3 scale;
	float rotation;
	float angularVelocity;
	float mass;
	RaftPhysics(glm::vec3 position, glm::vec3 scale, float mass);
	static glm::vec2 map2d(glm::vec3 v);
	static glm::vec2 forceAtPoint(glm::vec3 position, glm::vec2 velocity, glm::vec3 scale, float rotation, float mass, float time);
	static glm::vec2 dragAtPoint(glm::vec3 position, glm::vec2 velocity, glm::vec3 scale, float rotation, float time);
	static glm::vec2 rotate90(glm::vec2 v);

	static float computeTorque(glm::vec3 application, glm::vec3 axis, glm::vec2 velocity, glm::vec3 scale, float rotation, float angularVelocity, float mass, float time);
	float computeAngularAcceleration(float time);
	void update(float deltaTime, float time);
};

#endif //SURFACES_PHYSICS_HPP
