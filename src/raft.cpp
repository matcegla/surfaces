#include "raft.hpp"
#include "math.hpp"
#include "models.hpp"
#include "physics.hpp"
#include "xgl.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Raft::Raft(glm::vec3 position, const Material &material, glm::vec3 scale,
           int probes, const std::string &vertName, const std::string &fragName,
           CubeVertices &cubev)
    : cubev(cubev), shader(shaderProgramFromAsset(vertName, fragName)),
      upv(shader.locateUniform("trans_pv")),
      umodel(shader.locateUniform("trans_model")),
      physics(position, scale, material.density * volume(scale), probes) {}
void Raft::update(float deltaTime, float time) {
  physics.update(deltaTime, time);
}
void Raft::draw(const glm::mat4 &transPV) {
  auto model = glm::mat4(1.0f);
  model = glm::translate(model, physics.position);
  model = glm::rotate(model, -physics.rotation, glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::scale(model, physics.scale);
  shader.use();
  upv = transPV;
  umodel = model;
  cubev.draw();
}