#include "camera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

CameraFPS::CameraFPS(glm::vec3 pos):pos(pos),up(0.0f, 1.0f, 0.0f),front(1.0f, 0.0f, 0.0f),yaw(0.0f),pitch(0.0f),last(0.0f, 0.0f),init(false){}

void CameraFPS::handleCursorPos(float x, float y) {
	if (not init) {
		init = true;
		last = {x, y};
	}
	auto dx = x - last.x;
	auto dy = last.y - y;
	auto sensitivity = 0.1f;
	last = {x, y};
	yaw += dx * sensitivity;
	pitch += dy * sensitivity;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	front = glm::normalize(glm::vec3(
			cosf(glm::radians(pitch)) * cosf(glm::radians(yaw)),
			sinf(glm::radians(pitch)),
			cosf(glm::radians(pitch)) * sinf(glm::radians(yaw))
	));
}

void CameraFPS::handleKeyboard(float x, float y, float z, float deltaTime) {
	auto cameraSpeed = 20.0f;
	pos += cameraSpeed * deltaTime * x * right();
	pos += cameraSpeed * deltaTime * y * up;
	pos += cameraSpeed * deltaTime * z * -front;
}

glm::vec3 CameraFPS::right() { return glm::cross(front, up); }

glm::mat4 CameraFPS::viewMatrix() {
	return glm::lookAt(pos, pos + front, up);
}

glm::mat4 CameraFPS::projectionMatrix(float aspectRatio) {
	return glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 3000.0f);
}

glm::mat4 CameraFPS::viewProjectionMatrix(float aspectRatio) {
	return projectionMatrix(aspectRatio) * viewMatrix();
}
