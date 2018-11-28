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
#include "lg.hpp"
#include "screenbuffer.hpp"
#include "inter.hpp"

auto screen = ScreenInfo{1600, 800};
auto camera = CameraFPS({470.0f, 5.0f, 500.0f}); // NOLINT(cert-err58-cpp)

int main() {
	auto [glfw, window] = canvas<&screen, &camera>();
	auto aspectRatio = screen.aspectRatio();
	auto time = Time();
	auto paused = ToggleButton(false);
	auto transparent = ToggleButton(true);
	auto cubeVertices = CubeVertices();
	auto quadVertices = QuadVertices();
	auto screenbuffer = Screenbuffer("screen.vert", "screen.frag", quadVertices);
	auto inter = Inter(screen.width, screen.height);
	auto inter2 = Inter(screen.width, screen.height);
	auto inter3 = Inter(screen.width, screen.height);

	auto sun = Sun({550.0f, 30.0f, 550.0f}, {10.0f, 10.0f, 10.0f}, "standard.vert", "sun.frag", cubeVertices);
	auto water = Water(1000, 1000, "water.vert", "water.frag", sun.position);
	auto raftScale = glm::vec3(10.0f, 0.5f, 10.0f);
	auto raft = Raft({500.0f, 10.0f, 500.0f}, volume(raftScale) * woodDensity, raftScale, 64, "standard.vert", "raft.frag", cubeVertices);

	while (not window.shouldClose()) {

        // handle input
		time.handle(*paused, (float)glfw.time());
		if (window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
            window.setShouldClose(true);
		paused.update(window.getKey(GLFW_KEY_SPACE));
		transparent.update(window.getKey(GLFW_KEY_F3));
		camera.handleKeyboard(window.xkeyjoy(GLFW_KEY_D, GLFW_KEY_A), window.xkeyjoy(GLFW_KEY_E, GLFW_KEY_Q), window.xkeyjoy(GLFW_KEY_S, GLFW_KEY_W), time.delta.camera);
		raft.update(time.delta.physics, time.current.physics,
			200000.0f * glm::vec2(window.xkeyjoy(GLFW_KEY_RIGHT, GLFW_KEY_LEFT), window.xkeyjoy(GLFW_KEY_UP, GLFW_KEY_DOWN)),
			1000000.0f * window.xkeyjoy(GLFW_KEY_COMMA, GLFW_KEY_PERIOD)
		);

		// render

		auto transPV = camera.viewProjectionMatrix(aspectRatio);

		inter.bind();
		xclear(rgb(0x00, 0x2b, 0x36), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        water.draw(time.current.physics, transPV, camera.pos, *transparent);
        raft.draw(transPV);
        sun.draw(transPV);
		inter.unbind();

		inter2.bind();
		xclear(rgb(0x00, 0x2b, 0x36), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		water.draw(time.current.physics, transPV, camera.pos, not *transparent);
		raft.draw(transPV);
		sun.draw(transPV);
		inter2.unbind();

		inter3.bind();
		xclear(glm::vec3(0.0f), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		raft.draw(transPV);
		inter3.unbind();

		screenbuffer.prepare();
		screenbuffer.render(0.0f, 0.0f, 1.0f, 1.0f, inter.texture);
		screenbuffer.render(0.75f, 0.75f, 1.0f, 1.0f, inter2.texture);
		screenbuffer.render(0.5f, 0.75f, 0.75f, 1.0f, inter3.texture);

        window.swapBuffers();
        glfw.pollEvents();

    }

    glfw.terminate();
    return 0;
}
