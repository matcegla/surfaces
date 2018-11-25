#include "camera.hpp"
#include "xgl.hpp"
#include "physics.hpp"
#include "math.hpp"
#include "models.hpp"
#include "raft.hpp"
#include "water.hpp"
#include "sun.hpp"
#include "canvas.hpp"
#include "time.hpp"

auto screen = ScreenInfo{1920, 1080};
auto camera = CameraFPS({470.0f, 5.0f, 500.0f}); // NOLINT(cert-err58-cpp)

int main() {
	auto [glfw, window] = canvas<&screen, &camera>();
	auto time = Time();
	auto paused = ToggleButton(false);
	auto transparent = ToggleButton(false);
	auto cubeVertices = CubeVertices();

	auto sun = Sun({550.0f, 30.0f, 550.0f}, {10.0f, 10.0f, 10.0f}, "standard.vert", "sun.frag", cubeVertices);
	auto water = Water(1000, 1000, "water.vert", "water.frag", sun.position);
	auto raftScale = glm::vec3(10.0f, 0.5f, 10.0f);
	auto raft = Raft({500.0f, 10.0f, 500.0f}, volume(raftScale) * woodDensity, raftScale, "standard.vert", "raft.frag", cubeVertices);

	while (not window.shouldClose()) {

        // handle input
		time.handle(*paused, (float)glfw.time());
		if (window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
            window.setShouldClose(true);
		paused.update(window.getKey(GLFW_KEY_SPACE));
		transparent.update(window.getKey(GLFW_KEY_F3));
		camera.handleKeyboard(window.xkeyjoy(GLFW_KEY_D, GLFW_KEY_A), window.xkeyjoy(GLFW_KEY_E, GLFW_KEY_Q), window.xkeyjoy(GLFW_KEY_S, GLFW_KEY_W), time.delta.camera);
		raft.update(time.delta.physics, time.current.physics);

		// render
		auto transPV = camera.viewProjectionMatrix(screen.aspectRatio());
		xclear(rgb(0x00, 0x2b, 0x36));
        water.draw(time.current.physics, transPV, camera.pos, *transparent);
        raft.draw(transPV);
        sun.draw(transPV);

        // swap buffers
        window.swapBuffers();
        glfw.pollEvents();

    }

    glfw.terminate();
    return 0;
}
