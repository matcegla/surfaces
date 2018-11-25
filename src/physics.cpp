#include "physics.hpp"
#include "wave.hpp"
#include "math.hpp"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

const glm::vec3 gravity = {0.0f, -9.80665, 0.0f}; // NOLINT(cert-err58-cpp)
const float waterDensity = 998.23; // kg/m^3
const float airDensity = 1.225; // kg/m^3
const float woodDensity = 600.0f;

RaftPhysics::RaftPhysics(glm::vec3 position, glm::vec3 scale, float mass):
		position(position),
		velocity(),
		scale(scale),
		rotation(0.0f),
		angularVelocity(0.0f),
		mass(mass)
{}
glm::vec2 RaftPhysics::map2d(glm::vec3 v) { return glm::vec2(v.z, v.y); }
glm::vec2 RaftPhysics::forceAtPoint(glm::vec3 position, glm::vec2 velocity, glm::vec3 scale, float rotation, float mass, float time) {
	auto waveHeight = waveHeightAtPoint(position, time);
	auto lowerEdgeY = position.y - scale.y / 2;
	auto submergedHeight = glm::clamp(waveHeight - lowerEdgeY, 0.0f, scale.y);
	auto area = scale.x * scale.z;
	auto displacedWaterVolume = area * submergedHeight;

	auto weight = mass * map2d(gravity);
	auto buoyancy = - waterDensity * displacedWaterVolume * map2d(gravity);
	auto drag = dragAtPoint(position, velocity, scale, rotation, time);

	auto total = weight + buoyancy + drag;
	return total;
}
glm::vec2 RaftPhysics::dragAtPoint(glm::vec3 position, glm::vec2 velocity, glm::vec3 scale, float rotation, float time) {
	auto waveHeight = waveHeightAtPoint(position, time);
	auto fluidDensity = position.y > waveHeight ? airDensity : waterDensity;
	static const std::pair<float, float> inclinedCoefficients[] = {
			{ 0.0f, 0.1f},
			{25.0f, 0.4f},
			{35.0f, 0.7f},
			{40.0f, 0.6f},
			{45.0f, 0.7f},
			{50.0f, 0.8f},
			{55.0f, 0.85f},
			{60.0f, 0.9f},
			{70.0f, 1.0f},
			{80.0f, 1.1f},
			{90.0f, 1.1f},
	}; // http://www.iawe.org/Proceedings/BBAA7/X.Ortiz.pdf
	auto relangle = [&]{
		auto normed = fmodf(fmodf(rotation - glm::angle(velocity, glm::vec2(1.0f, 0.0f)), 2*M_PIf32) + 2*M_PIf32, 2*M_PIf32);
		if (normed < M_PI_2f32)
			return normed;
		else if (normed < M_PI)
			return M_PIf32 - normed;
		else if (normed < 1.5f * M_PIf32)
			return normed - M_PIf32;
		else
			return 2*M_PIf32 - normed;
	}();
	auto relh = scale.z * sinf(relangle);
	auto relarea = scale.x * relh;
	// TODO check if angles are correct
	auto dragCoefficient = [&]{
		for (auto i=0; i+1<(int)sizeof(inclinedCoefficients)/(int)sizeof(inclinedCoefficients[0]); ++i) {
			auto degrelangle = glm::degrees(relangle);
			auto datum1 = inclinedCoefficients[i];
			auto datum2 = inclinedCoefficients[i+1];
			if (datum1.first <= degrelangle and degrelangle <= datum2.first)
				return glm::mix(datum1.second, datum2.second, (degrelangle - datum1.first) / (datum2.first - datum1.first));
		}
		return 0.0f;
	}();
	auto drag = -enorm(velocity) * 0.5f * fluidDensity * powf(glm::length(velocity), 2) * dragCoefficient * relarea;
	return drag;
}
glm::vec2 RaftPhysics::rotate90(glm::vec2 v) { return glm::vec2(-v.y, v.x); }

float RaftPhysics::computeTorque(glm::vec3 application, glm::vec3 axis, glm::vec2 velocity, glm::vec3 scale, float rotation, float angularVelocity, float mass, float time) {
	auto r = glm::length(application - axis);
	auto vel = velocity + angularVelocity * r * enorm(rotate90(map2d(application - axis)));
	auto force = forceAtPoint(application, vel, scale, rotation, mass, time);
	auto arm = -glm::sign(glm::dot(glm::vec3(0.0f, sinf(rotation), cosf(rotation)), application - axis));
	auto torque = arm * (force.x * sinf(rotation) - force.y * cosf(rotation)) * r;
	return torque;
}
float RaftPhysics::computeAngularAcceleration(float time) {
	auto n = 8;
	auto partScale = scale * glm::vec3(1.0f, 1.0f, 1.0f / n);
	auto leftVerge = position - scale.z/2 * glm::vec3(0.0f, sinf(rotation), cosf(rotation));
	auto rightVerge = position + scale.z/2 * glm::vec3(0.0f, sinf(rotation), cosf(rotation));
	auto torque = 0.0f;
	for (auto i=0; i<n; ++i)
		torque += computeTorque(mix(leftVerge, rightVerge, (2.0f*i+1)/(2*n)), position, velocity, partScale, rotation, angularVelocity, mass/n, time);
	auto momentOfInertia = (1.0f / 12) * mass * (powf(scale.z, 2) + powf(scale.y, 2));
	auto angularAcceleration = torque / momentOfInertia;
	return angularAcceleration;
}
void RaftPhysics::update(float deltaTime, float time, glm::vec2 externalForce) {
		auto force = externalForce + forceAtPoint(position, velocity, scale, rotation, mass, time);
		auto angularAcceleration = computeAngularAcceleration(time);
		velocity += deltaTime * force / mass;
		position += deltaTime * glm::vec3(0.0f, velocity.y, velocity.x);
		angularVelocity += deltaTime * angularAcceleration;
		rotation += deltaTime * angularVelocity;
	}