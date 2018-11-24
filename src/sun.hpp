#ifndef SURFACES_SUN_HPP
#define SURFACES_SUN_HPP

#include "xgl.hpp"
#include "models.hpp"

struct Sun {
	CubeVertices& cubev;
	Program shader;
	Uniform upv, umodel;
	glm::vec3 position;
	glm::vec3 scale;
	Sun(glm::vec3 position, glm::vec3 scale, const std::string& vertPath, const std::string& fragPath, CubeVertices& cubev);
	void draw(const glm::mat4& transPV);
};


#endif //SURFACES_SUN_HPP
