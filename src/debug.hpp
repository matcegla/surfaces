#ifndef SURFACES_DEBUG_HPP
#define SURFACES_DEBUG_HPP

#include <vector>
#include <glm/vec3.hpp>
#include <map>
#include "xgl.hpp"
#include "models.hpp"

struct Debug {
	Debug(const std::string& vertPath, const std::string& fragPath, CubeVertices& cubev, std::map<std::string, glm::vec3> colorTable);
	void point(const glm::vec3& position, const std::string& name);
	void reset();
	void draw(const glm::mat4& transPV);
private:
	CubeVertices& cubev;
	Program shader;
	Uniform upv;
	Uniform umodel;
	Uniform ucolor;
	std::map<std::string, glm::vec3> colorTable;
	std::vector<std::pair<glm::vec3, glm::vec3>> queuePoints;
};

extern Debug* debug;

#endif //SURFACES_DEBUG_HPP
