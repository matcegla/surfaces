#include <utility>

#include "debug.hpp"
#include <glm/gtc/matrix_transform.hpp>

void Debug::point(const glm::vec3 &position, const std::string &name) {
  assert(colorTable.count(name));
  queuePoints.emplace_back(position, colorTable[name]);
}

Debug::Debug(const std::string &vertName, const std::string &fragName,
             CubeVertices &cubev, std::map<std::string, glm::vec3> colorTable)
    : cubev(cubev), shader(shaderProgramFromAsset(vertName, fragName)),
      upv(shader.locateUniform("trans_pv")),
      umodel(shader.locateUniform("trans_model")),
      ucolor(shader.locateUniform("color")), colorTable(std::move(colorTable)) {
}

void Debug::reset() { queuePoints.clear(); }

void Debug::draw(const glm::mat4 &transPV) {
  shader.use();
  upv = transPV;
  for (auto point : queuePoints) {
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, point.first);
    model = glm::scale(model, glm::vec3(0.5f));
    umodel = model;
    ucolor = point.second;
    cubev.draw();
  }
}

Debug *debug = nullptr;
