#include "canvas.hpp"

std::pair<GLFW, Window> canvasNoCallback(int width, int height) {
  auto glfw = GLFW();
  glfw.xhintContextVersion(3, 3);
  glfw.windowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  auto icon = Image::load("assets/icon.png");
  auto iconMeta = new GLFWimage{icon.width, icon.height,
                                icon.data}; // TODO don't leak memory
  auto window = Window{width, height, "Surfaces", nullptr, nullptr};
  window.makeContextCurrent();
  window.setWindowIcon(*iconMeta);
  window.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  loadGLAD();
  glViewport(0, 0, width, height);
  glEnable(GL_DEPTH_TEST);
  return {glfw, window};
}
