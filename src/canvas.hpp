#ifndef SURFACES_CANVAS_HPP
#define SURFACES_CANVAS_HPP

#include "camera.hpp"
#include "xgl.hpp"

std::pair<GLFW, Window> canvasNoCallback(int width, int height);
template <ScreenInfo *screen, CameraFPS *camera>
std::pair<GLFW, Window> canvas() {
  auto [glfw, window] = canvasNoCallback(screen->width, screen->height);
  window.onFramebufferSize([](GLFWwindow *, int width, int height) {
    glViewport(0, 0, width, height);
    screen->width = width;
    screen->height = height;
  });
  window.onCursorPos([](GLFWwindow *, double x, double y) {
    camera->handleCursorPos((float)x, (float)y);
  });
  return {glfw, window};
}

#endif // SURFACES_CANVAS_HPP
