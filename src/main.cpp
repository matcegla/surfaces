#include <glm/gtc/matrix_transform.hpp>
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
#include "debug.hpp"

auto monitor = ScreenInfo{1600, 800};
auto camera = CameraFPS({470.0f, 5.0f, 500.0f}); // NOLINT(cert-err58-cpp)

int main() {
	auto [glfw, window] = canvas<&monitor, &camera>();
	auto aspectRatio = monitor.aspectRatio();
	auto time = Time();
	auto paused = ToggleButton(false);
	auto transparent = ToggleButton(false);
	auto wireframe = ToggleButton(false);
	auto physicsdebug = ToggleButton(false);
	auto slowmo = ToggleButton(false);
	auto cubeVertices = CubeVertices();
	auto quadVertices = QuadVertices();
	auto screen = Screenbuffer("screen", "screen", quadVertices);
	auto screenExtract = Screenbuffer("screen", "screen_bloom_extract", quadVertices);
	auto screenBlur = Screenbuffer("screen", "screen_bloom_blur", quadVertices);
	auto inter = Inter(monitor.width, monitor.height);
	auto globalDebug = Debug("debug_point", "debug_point", cubeVertices, {
		{"gravity", {1, 0, 0}},
		{"velocity", {0, 0, 0}},
		{"buoyancy", {0, 0, 1}},
		{"drag", {0, 1, 0}},
		{"part velocity", {.5, .5, .5}},
		{"linear velocity", {1, 1, 1}},
		{"angular movement normal", {1, 0.2, 1}},
	});
	::debug = &globalDebug;

	auto sun = Sun({550.0f, 30.0f, 550.0f}, {10.0f, 10.0f, 10.0f}, "standard", "sun", cubeVertices);
	auto water = Water(1000, 1000, "water", "water", sun.position);
	auto raft = Raft({500.0f, 10.0f, 500.0f}, wood, {10.0f, 0.5f, 10.0f}, 8, "standard", "raft", cubeVertices);

	while (not window.shouldClose()) {

        // handle input
        debug->reset();
		time.handle(*paused, *slowmo, (float)glfw.time());
		if (window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
            window.setShouldClose(true);
		paused.update(window.getKey(GLFW_KEY_SPACE));
		transparent.update(window.getKey(GLFW_KEY_F3));
		wireframe.update(window.getKey(GLFW_KEY_F4));
		physicsdebug.update(window.getKey(GLFW_KEY_F5));
		slowmo.update(window.getKey(GLFW_KEY_LEFT_ALT));
		camera.handleKeyboard(window.xkeyjoy(GLFW_KEY_D, GLFW_KEY_A), window.xkeyjoy(GLFW_KEY_E, GLFW_KEY_Q), window.xkeyjoy(GLFW_KEY_S, GLFW_KEY_W), time.camera.delta);
		raft.update(time.physics.delta, time.physics.current);

		// render

		auto transPV = camera.viewProjectionMatrix(aspectRatio);

		inter.bind();
		xclear(rgb(0x00, 0x2b, 0x36), GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (*wireframe)
        	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        water.draw(time.physics.current, transPV, camera.pos, *transparent);
		raft.draw(transPV);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        sun.draw(transPV);
        if (*physicsdebug)
        	globalDebug.draw(transPV);
		inter.unbind();

		screen.prepare();
		screen.render(0.0f, 0.0f, 1.0f, 1.0f, inter.texture);
//		screenBlur.render(0.75f, 0.75f, 1.0f, 1.0f, inter.texture);
//		screenExtract.render(0.75f, 0.5f, 1.0f, 0.75f, inter.texture);

        window.swapBuffers();
        glfw.pollEvents();

    }

    glfw.terminate();
    return 0;
}
