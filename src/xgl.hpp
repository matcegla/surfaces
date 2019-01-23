#ifndef SURFACES_XGL_HPP
#define SURFACES_XGL_HPP

#include <glad/glad.h>
#include <string>
#include <glm/detail/type_mat.hpp>
#include <glm/vec3.hpp>
#include <GLFW/glfw3.h>
#include <vector>

struct Image {
	void free();
	static Image load(const std::string& path);
	int width, height, channelCount;
	unsigned char* data;
private:
	Image();
};
struct Shader {
	explicit Shader(GLenum type);
	void source(GLsizei count, const GLchar* const* text, const GLint* length);
	void source(GLsizei count, const std::string& text, const GLint* length);
	void compile();
	void free();
	unsigned id;
};
struct Uniform {
	void operator=(int x); // NOLINT(misc-unconventional-assign-operator)
	void operator=(float x); // NOLINT(misc-unconventional-assign-operator)
	void operator=(const glm::vec2& x); // NOLINE(misc-unconventional-assign-operator)
	void operator=(const glm::vec3& x); // NOLINT(misc-unconventional-assign-operator)
	void operator=(const glm::mat4& x); // NOLINT(misc-unconventional-assign-operator)
	int id;
};
struct Program {
	Program();
	void attach(const Shader& shader);
	void link();
	void use();
	Uniform locateUniform(const char* name);
	unsigned id;
};
struct Window {
	Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share);
	void makeContextCurrent();
	int getKey(int key);
	float xkey(int key);
	float xkeyjoy(int keypos, int keyneg);
	int shouldClose();
	void setShouldClose(int value);
	void setInputMode(int mode, int value);
	void setWindowIcon(int count, const GLFWimage* images);
	void setWindowIcon(GLFWimage& ref);
	void swapBuffers();
	void onFramebufferSize(GLFWwindowsizefun callback);
	void onCursorPos(GLFWcursorposfun callback);
	GLFWwindow* ptr;
};
struct Texture {
	Texture();
	void bind(GLenum target);
	void unbind(GLenum target);
	void image2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
	void parameter(GLenum target, GLenum pname, GLint param);
	void xactivateAndBind(GLenum slot, GLenum target);
	unsigned id;
};
struct VAO {
	VAO();
	void bind();
	unsigned id;

	void unbind();
};
struct VBO {
	VBO();
	void bindBuffer(GLenum target);
	template <unsigned n> void xbindAndBufferStatic(const float(&vertices)[n]) {
		xbindAndBufferStatic(vertices, n);
	}
	void xbindAndBufferStatic(const std::vector<float>& vertices);
	void xbindAndBufferStatic(const float* vertices, unsigned n);
	unsigned id;
};
struct EBO {
	EBO();
	void bindBuffer(GLenum target);
	template <unsigned n> void xbindAndBufferStatic(unsigned(&indices)[n]) {
		xbindAndBufferStatic(indices, n);
	}
	void xbindAndBufferStatic(const std::vector<unsigned>& indices);
	void xbindAndBufferStatic(const unsigned* indices, unsigned n);
	unsigned id;
};
struct RBO {
	RBO();
	void bind(GLenum target);
	void unbind(GLenum target);
	void storage(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height);
	unsigned id;
};
struct FBO {
	FBO();
	void bind(GLenum target);
	void unbind(GLenum target);
	void texture2D(GLenum target, GLenum attachment, GLenum textarget, Texture& texture, GLint level);
	void renderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, RBO& renderbuffer);
	GLenum checkStatus(GLenum target);
	void xassertComplete(GLenum target);
	unsigned id;
};
struct GLFW {
	GLFW();
	void windowHint(int hint, int value);
	void pollEvents();
	void terminate();
	double time();
	void xhintContextVersion(int major, int minor);
};
struct ScreenInfo {
	int width, height;
	float aspectRatio();
};

std::string readFile(const std::string& path);
Shader shaderFromFile(const std::string& path, GLenum type);
Program shaderProgramFromShaders(const Shader& vertex, const Shader& fragment);
Program shaderProgramFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
Program shaderProgramFromAsset(const std::string& vertexName, const std::string& fragmentName);
Texture textureFromFile(const std::string& path, GLenum format);
void loadGLAD();
void xclear(glm::vec3 backgroundColor, GLbitfield mask);

struct ToggleButton {
	bool state;
	bool pressed;
	explicit ToggleButton(bool initialState);
	bool update(int keyresult);
	bool operator*();
};

#endif //SURFACES_XGL_HPP