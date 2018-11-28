#include "water.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Water::Water(int width, int depth, const std::string& vertPath, const std::string& fragPath, glm::vec3 sunPos):
		vao(),
		vbo(),
		ebo(),
		shader(shaderProgramFromFiles(vertPath, fragPath)),
		utime(shader.locateUniform("time")),
		upv(shader.locateUniform("trans_pv")),
		umodel(shader.locateUniform("trans_model")),
		uviewpos(shader.locateUniform("view_pos")),
		vertices(),
		indices((unsigned)2*3*width*depth)
{
	for (auto x=0; x<width+1; ++x) {
		for (auto z=0; z<depth+1; ++z) {
			vertices.push_back((float)x);
			vertices.push_back(0);
			vertices.push_back((float)z);
			if (x < width and z < depth) {
				indices[3*(x*depth+z)] = ((unsigned)((width+1)*x+z));
				indices[3*(x*depth+z)+1] = ((unsigned)((width+1)*x+z+1));
				indices[3*(x*depth+z)+2] = ((unsigned)((width+1)*(x+1)+z));
				indices[3*(x*depth+z)+3*width*depth] = ((unsigned)((width+1)*x+z+1));
				indices[3*(x*depth+z)+3*width*depth+1] = ((unsigned)((width+1)*(x+1)+z));
				indices[3*(x*depth+z)+3*width*depth+2] = ((unsigned)((width+1)*(x+1)+z+1));
			}
		}
	}

	vao.bind();
	vbo.xbindAndBufferStatic(vertices);
	ebo.xbindAndBufferStatic(indices);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	shader.use();
	umodel = glm::mat4(1.0f);
	shader.locateUniform("sun.pos") = sunPos;
	shader.locateUniform("sun.color") = glm::vec3(1.0f);
}
void Water::draw(float time, const glm::mat4& transPV, glm::vec3 viewPos, bool transparent) {
	vao.bind();
	shader.use();
	utime = time;
	upv = transPV;
	uviewpos = viewPos;
	glDrawElements(GL_TRIANGLES, (unsigned)indices.size() / (transparent ? 2 : 1), GL_UNSIGNED_INT, nullptr);
	vao.unbind();
}
