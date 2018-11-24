#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

#include "camera.hpp"
#include "lg.hpp"
#include "xgl.hpp"
#include "physics.hpp"
#include "math.hpp"
#include "models.hpp"
#include "raft.hpp"
#include "water.hpp"
#include "sun.hpp"

auto screen = ScreenInfo{1920, 1080};
auto camera = CameraFPS({470.0f, 5.0f, 500.0f}); // NOLINT(cert-err58-cpp)

int main() {
	auto glfw = GLFW();
	glfw.xhintContextVersion(3, 3);
	glfw.windowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto window = Window(screen.width, screen.height, "Surfaces", nullptr, nullptr);
    window.makeContextCurrent();
    auto icon = Image::load("icon2.png");
	auto iconMeta = GLFWimage{icon.width, icon.height, icon.data};
	window.setWindowIcon(iconMeta);
    loadGLAD();
    glViewport(0, 0, screen.width, screen.height);
    glEnable(GL_DEPTH_TEST);
	window.onFramebufferSize([](GLFWwindow *, int width, int height) {
		lg.info("resizing to ", width, " ", height, "\n");
		glViewport(0, 0, width, height);
		screen.width = width;
		screen.height = height;
	});
	window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	window.onCursorPos([](GLFWwindow*, double xpos, double ypos){ camera.handleCursorPos((float)xpos, (float)ypos); });

	auto cubeVertices = CubeVertices();
	auto sun = Sun({550.0f, 30.0f, 550.0f}, {10.0f, 10.0f, 10.0f}, "standard.vert", "sun.frag", cubeVertices);
	auto water = Water(1000, 1000, "water.vert", "water.frag", sun.position);
	auto raftScale = glm::vec3(10.0f, 0.5f, 10.0f);
	auto raft = Raft({500.0f, 10.0f, 500.0f}, raftScale.x * raftScale.y * raftScale.z * woodDensity, raftScale, "standard.vert", "raft.frag", cubeVertices);

	auto backgroundColor = rgb(0x00, 0x2b, 0x36);
	auto lastTime = (float)glfw.time();
	auto cameraTime = lastTime;
	auto physicsTime = lastTime;
	auto paused = ToggleButton(false);
	auto transparent = ToggleButton(false);

	while (not window.shouldClose()) {

        // handle input

		auto cameraDeltaTime = [&](){
			auto time = (float)glfw.time();
			auto deltaTime = time - lastTime;
			lastTime = time;
			return deltaTime;
		}();
		auto physicsDeltaTime = *paused ? 0.0f : cameraDeltaTime;
		cameraTime += cameraDeltaTime;
		physicsTime += physicsDeltaTime;

		if (window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
            window.setShouldClose(true);
		paused.update(window.getKey(GLFW_KEY_SPACE));
		transparent.update(window.getKey(GLFW_KEY_F3));
		camera.handleKeyboard(window.xkeyjoy(GLFW_KEY_D, GLFW_KEY_A), window.xkeyjoy(GLFW_KEY_E, GLFW_KEY_Q), window.xkeyjoy(GLFW_KEY_S, GLFW_KEY_W), cameraDeltaTime);

		raft.update(physicsDeltaTime, physicsTime);

		auto view = camera.viewMatrix();
		auto projection = glm::perspective(glm::radians(45.0f), (float)screen.width / screen.height, 0.1f, 3000.0f);
		auto transPV = projection * view;

		// render

        glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        water.draw(physicsTime, transPV, camera.pos, *transparent);
        raft.draw(transPV);
        sun.draw(transPV);

        // swap buffers

        window.swapBuffers();
        glfw.pollEvents();

    }

    glfw.terminate();
    return 0;

}