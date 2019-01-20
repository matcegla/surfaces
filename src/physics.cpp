#include "physics.hpp"
#include "wave.hpp"
#include "math.hpp"
#include "lg.hpp"
#include "debug.hpp"
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

ForceApplication2::ForceApplication2(glm::vec2 point, glm::vec2 force):
	point(point),
	force(force)
{}

std::ostream& operator<<(std::ostream& out, const ForceApplication2& app) {
	return out << app.force << " applied to " << app.point;
}

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
		acceleration += force.force / mass;
		torque += torqueFromForce(force);
	}
	auto momentOfInertia = (1.0f / 12) * mass * (powf(scale.z, 2) + powf(scale.y, 2));
	auto angularAcceleration = torque / momentOfInertia;
	velocity += deltaTime * acceleration;
	position += deltaTime * glm::vec3(0.0f, velocity.y, velocity.x);
	angularVelocity += deltaTime * angularAcceleration;
	rotation += deltaTime * angularVelocity;
}

std::vector<ForceApplication2> RaftPhysics::computeForces(float time) {
	auto forces = std::vector<ForceApplication2>();
	auto n = probes;
	auto scalePart = scale * glm::vec3(1.0f, 1.0f, 1.0f / n);
	auto vergeLeft = position - scale.z/2 * glm::vec3(0.0f, sinf(rotation), cosf(rotation));
	auto vergeRight = position + scale.z/2 * glm::vec3(0.0f, sinf(rotation), cosf(rotation));
	for (auto i=0; i<n; ++i) {
		auto positionPart = mix(vergeLeft, vergeRight, (2.0f*i+1)/(2*n));
		auto armLength = glm::length(position - positionPart);
		auto armSign = i < n/2 ? -1 : +1;
		auto angTraj = glm::vec2(-sinf(rotation), cosf(rotation));
		auto linearVelocity = angularVelocity * armLength * armSign * angTraj;
		auto part = RaftPart(positionPart, velocity + linearVelocity, scalePart, rotation, mass/n);
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

RaftPart::RaftPart(const glm::vec3& position, const glm::vec2& velocity, const glm::vec3& scale, float rotation, float mass):
	position(position),
	velocity(velocity),
	scale(scale),
	rotation(rotation),
	mass(mass)
{}

ForceApplication2 RaftPart::weight() {
	auto weight = mass * map2D(gravity);
	debug->point(position + map3D(weight) / (5 * mass), "gravity");
	return {position, weight};
}

ForceApplication2 RaftPart::buoyancy(float time) {
	auto waveHeight = waveHeightAtPoint(position, time);
	auto submergedHeight = waveHeight > position.y ? scale.y : 0.0f;
	auto area = scale.x * scale.z;
	auto displacedWaterVolume = area * submergedHeight;
	auto buoyancy = -water.density * displacedWaterVolume * map2D(gravity);
	debug->point(position + map3D(buoyancy) / (5 * mass), "buoyancy");
	return {position, buoyancy};
}

ForceApplication2 RaftPart::drag(float time) {
	static const std::pair<float, float> inclinedCoefficients[] = {
		// TODO enter more precise values
		// http://www.iawe.org/Proceedings/BBAA7/X.Ortiz.pdf
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
	};
	auto touchPosition = map2D(position) + glm::vec2(cosf(rotation), sinf(rotation)) * scale.y / 2.0f;
	auto underwater = touchPosition.y > waveHeightAtPoint(position, time);
	auto fluidDensity = (underwater ? air : water).density;
	// TODO check correctness of angle calculation
	auto angle = acuteAngle(velocity, glm::vec2(cosf(rotation), sinf(rotation)));
	auto relativeArea = scale.x * scale.z * sinf(angle);
	// TODO check correctness of datum selection
	auto datum = std::lower_bound(begin(inclinedCoefficients), end(inclinedCoefficients), std::pair(glm::degrees(angle), 0.0f));
	if (datum == end(inclinedCoefficients))
		--datum;
	// TODO interpolate between data
	auto dragCoefficient = datum->second;
	auto drag = -enorm(velocity) * 0.5f * fluidDensity * powf(glm::length(velocity), 2) * dragCoefficient * relativeArea;
	debug->point(position + map3D(drag) / (5 * mass), "drag");
	return {touchPosition, drag};
}

glm::vec2 map2D(glm::vec3 v) {
	return {v.z, v.y};
}

glm::vec3 map3D(glm::vec2 v) {
	return {0, v.y, v.x};
}

float acuteAngle(const glm::vec2 &a, const glm::vec2 &b) {
	auto alpha = atan2f(a.y, a.x);
	auto beta = atan2f(b.y, b.x);
	if (alpha < beta)
		std::swap(alpha, beta);
	auto d = alpha - beta;
	if (d > M_PI)
		d = 2*M_PIf32 - d;
	return d;
}

const glm::vec3 gravity = {0.0f, -9.80665, 0.0f}; // NOLINT(cert-err58-cpp)
const Material water = {998.23 };
const Material air =   {  1.225};
const Material wood =  {600.0  };
