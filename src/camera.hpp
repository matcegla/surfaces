#ifndef SURFACES_CAMERA_HPP
#define SURFACES_CAMERA_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct CameraFPS {
	explicit CameraFPS(glm::vec3 pos);

	void handleCursorPos(float x, float y);
	void handleKeyboard(float x, float y, float z, float deltaTime);
	glm::vec3 right();
	glm::mat4 viewMatrix();

	glm::vec3 pos, up, front;
	float yaw, pitch;
	glm::vec2 last;
	bool init;
};

#endif //SURFACES_CAMERA_HPP
