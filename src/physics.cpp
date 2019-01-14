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

ForceApplication2::ForceApplication2(glm::vec2 point, glm::vec2 force):
	point(point),
	force(force)
{}

RaftPhysics::RaftPhysics(glm::vec3 position, glm::vec3 scale, float mass, int probes):
		position(position),
		velocity(),
		scale(scale),
		rotation(0.0f),
		angularVelocity(0.0f),
		mass(mass),
		probes(probes)
{}

void RaftPhysics::update(float deltaTime, float time) {
	auto acceleration = glm::vec2(0.0f, 0.0f);
	auto torque = 0.0f;
	auto forces = computeForces(time);
	for (auto force : forces) {
		acceleration += force.force / mass * deltaTime;
		torque += torqueFromForce(force);
	}
	auto momentOfInertia = (1.0f / 12) * mass * (powf(scale.z, 2) + powf(scale.y, 2));
	auto angularAcceleration = torque / momentOfInertia;
	velocity += deltaTime * acceleration;
	position += deltaTime * glm::vec3(0.0f, velocity.y, velocity.x);
	angularVelocity += deltaTime * angularAcceleration;
	rotation = deltaTime * angularVelocity;
}

std::vector<ForceApplication2> RaftPhysics::computeForces(float time) {
	auto forces = std::vector<ForceApplication2>();
	auto n = probes;
	auto scalePart = scale * glm::vec3(1.0f, 1.0f, 1.0f / n);
	auto vergeLeft = position - scale.z/2 * glm::vec3(0.0f, sinf(rotation), cosf(rotation));
	auto vergeRight = position + scale.z/2 * glm::vec3(0.0f, sinf(rotation), cosf(rotation));
	for (auto i=0; i<n; ++i) {
		// TODO Account for angular velocity. Or maybe account for it inside RaftPart?
		auto part = RaftPart(mix(vergeLeft, vergeRight, (2.0f*i+1)/(2*n)), scalePart, mass/n, *this);
		forces.push_back(part.weight());
		forces.push_back(part.buoyancy(time));
		forces.push_back(part.drag(time));
	}
	return forces;
}

float RaftPhysics::torqueFromForce(ForceApplication2 applied) {
	auto application = applied.point;
	auto axis = map2D(position);
	auto r = glm::length(application - axis);
	auto force = applied.force;
	auto arm = -glm::sign(glm::dot({cosf(rotation), sinf(rotation)}, application - axis));
	auto torque = arm * (force.x * sinf(rotation) - force.y * cosf(rotation)) * r;
	return torque;
}

RaftPart::RaftPart(const glm::vec3& position, const glm::vec3& scale, float mass, const RaftPhysics& whole):
	position(position),
	scale(scale),
	mass(mass),
	whole(whole)
{}

ForceApplication2 RaftPart::weight() {
	auto weight = mass * map2D(gravity);
	return {position, weight};
}

ForceApplication2 RaftPart::buoyancy(float time) {
	auto waveHeight = waveHeightAtPoint(position, time);
	auto submergedHeight = waveHeight > position.y ? scale.y : 0.0f;
	auto area = scale.x * scale.z;
	auto displacedWaterVolume = area * submergedHeight;
	auto buoyancy = -waterDensity * displacedWaterVolume * map2D(gravity);
	return {position, buoyancy};
}

ForceApplication2 RaftPart::drag(float time) {
	// TODO actually implement drag
	return {position, {0.0f, 0.0f}};
}

glm::vec2 map2D(glm::vec3 v) {
	return {v.z, v.y};
}

/*
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

*/
