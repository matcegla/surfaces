#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <sstream>
#include <fstream>
#include "xgl.hpp"
#include "lg.hpp"

Shader::Shader(GLenum type):id(glCreateShader(type)){}
void Shader::source(GLsizei count, const GLchar* const* text, const GLint* length) { glShaderSource(id, count, text, length); }
void Shader::source(GLsizei count, const std::string& text, const GLint* length) { auto text2 = text.c_str(); source(count, &text2, length); }
void Shader::compile() {
	glCompileShader(id);
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (not success) {
		glGetShaderInfoLog(id, 512, nullptr, infoLog);
		lg.error("shader compilation failed: ", infoLog);
		std::exit(1);
	}
}
void Shader::free() { glDeleteShader(id); }

void Uniform::operator=(float x) { glUniform1f(id, x); } // NOLINT(misc-unconventional-assign-operator)
void Uniform::operator=(int x) { glUniform1i(id, x); } // NOLINT(misc-unconventional-assign-operator)
void Uniform::operator=(const glm::mat4& x) { glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(x)); } // NOLINT(misc-unconventional-assign-operator)
void Uniform::operator=(const glm::vec3& x) { glUniform3f(id, x.x, x.y, x.z); } // NOLINT(misc-unconventional-assign-operator)

Program::Program():id(glCreateProgram()){}
void Program::attach(const Shader& shader) { glAttachShader(id, shader.id); }
void Program::link() {
	glLinkProgram(id);
	int success;
	char infoLog[512];
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (not success) {
		glGetProgramInfoLog(id, 512, nullptr, infoLog);
		lg.error("shader linking failed: ", infoLog);
		std::exit(1);
	}
}
Uniform Program::locateUniform(const char* name) {
	return Uniform{glGetUniformLocation(id, name)};
}
void Program::use() { glUseProgram(id); }

Window::Window(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share):ptr(glfwCreateWindow(width, height, title, monitor, share)) {
	if (ptr == nullptr) {
		lg.error("failed to create GLFW window\n");
		glfwTerminate();
		exit(1);
	}
}
void Window::makeContextCurrent() { glfwMakeContextCurrent(ptr); }
int Window::getKey(int key) { return glfwGetKey(ptr, key); }
float Window::xkey(int key) { return (float)getKey(key); }
float Window::xkeyjoy(int keypos, int keyneg) { return xkey(keypos) - xkey(keyneg); }
int Window::shouldClose() { return glfwWindowShouldClose(ptr); }
void Window::setShouldClose(int value) { glfwSetWindowShouldClose(ptr, value); }
void Window::setInputMode(int mode, int value) { glfwSetInputMode(ptr, mode, value); }
void Window::swapBuffers() { glfwSwapBuffers(ptr); }
void Window::onFramebufferSize(GLFWwindowsizefun callback) { glfwSetWindowSizeCallback(ptr, callback); }
void Window::onCursorPos(GLFWcursorposfun callback) { glfwSetCursorPosCallback(ptr, callback); }
void Window::setWindowIcon(int count, const GLFWimage* images) {
	glfwSetWindowIcon(ptr, count, images);
}
void Window::setWindowIcon(GLFWimage& ref) {
	setWindowIcon(1, &ref);
}

VAO::VAO():id(0){ glGenVertexArrays(1, &id); }
void VAO::bind() { glBindVertexArray(id); }

VBO::VBO():id(0) { glGenBuffers(1, &id); }
void VBO::bindBuffer(GLenum target) { glBindBuffer(target, id); }
void VBO::xbindAndBufferStatic(const std::vector<float>& vertices) {
	xbindAndBufferStatic(vertices.data(), (unsigned)vertices.size());
}
void VBO::xbindAndBufferStatic(const float* vertices, unsigned n) {
	bindBuffer(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER, n * sizeof(float), vertices, GL_STATIC_DRAW);
}

EBO::EBO():id(0) { glGenBuffers(1, &id); }
void EBO::bindBuffer(GLenum target) { glBindBuffer(target, id); }
void EBO::xbindAndBufferStatic(const std::vector<unsigned>& indices) {
	xbindAndBufferStatic(indices.data(), (unsigned)indices.size());
}
void EBO::xbindAndBufferStatic(const unsigned* indices, unsigned n) {
	bindBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, n * sizeof(unsigned), indices, GL_STATIC_DRAW);
}

GLFW::GLFW() { glfwInit(); }
void GLFW::windowHint(int hint, int value) { glfwWindowHint(hint, value); }
void GLFW::pollEvents() { glfwPollEvents(); }
void GLFW::terminate() { glfwTerminate(); }
double GLFW::time() { return glfwGetTime(); }
void GLFW::xhintContextVersion(int major, int minor) {
	windowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	windowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
}

Image Image::load(const std::string& path) {
	Image image;
	stbi_set_flip_vertically_on_load(true);
	image.data = stbi_load(path.c_str(), &image.width, &image.height, &image.channelCount, 0);
	if (not image.data) {
		lg.error("failed to load image ", path);
		std::exit(1);
	}
	return image;
}
void Image::free() { stbi_image_free(data); }
Image::Image():width(0),height(0),channelCount(0),data(nullptr){}

Texture::Texture():id(0){ glGenTextures(1, &id); }
void Texture::bind(GLenum target) { glBindTexture(target, id); }
void Texture::xactivateAndBind(GLenum slot, GLenum target) {
	glActiveTexture(slot);
	glBindTexture(target, id);
}

void Texture::unbind(GLenum target) {
	glBindTexture(target, 0);
}

void Texture::image2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels) {
	glTexImage2D(target, level, internalFormat, width, height, border, format, type, pixels);
}

void Texture::parameter(GLenum target, GLenum pname, GLint param) {
	glTexParameteri(target, pname, param);
}

std::string readFile(const std::string& path) {
	std::ostringstream oss;
	std::ifstream file(path);
	file >> oss.rdbuf();
	return oss.str();
}
Shader shaderFromFile(const std::string& path, GLenum type) {
	auto src = readFile(path);
	auto shader = Shader(type);
	shader.source(1, src, nullptr);
	shader.compile();
	return shader;
}
Program shaderProgramFromShaders(const Shader& vertex, const Shader& fragment) {
	auto program = Program();
	program.attach(vertex);
	program.attach(fragment);
	program.link();
	return program;
}
Program shaderProgramFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
	auto vertex = shaderFromFile(vertexPath, GL_VERTEX_SHADER);
	auto fragment = shaderFromFile(fragmentPath, GL_FRAGMENT_SHADER);
	auto program = shaderProgramFromShaders(vertex, fragment);
	vertex.free();
	fragment.free();
	return program;
}
Texture textureFromFile(const std::string& path, GLenum format) {
	auto img = Image::load(path);
	auto tex = Texture();
	tex.bind(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, format, GL_UNSIGNED_BYTE, img.data);
	img.free();
	glGenerateMipmap(GL_TEXTURE_2D);
	return tex;
}
void loadGLAD() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		lg.error("failed to initialize GLAD\n");
		std::exit(-1);
	}
}

void xclear(glm::vec3 backgroundColor, GLbitfield mask) {
	glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
	glClear(mask);
}

ToggleButton::ToggleButton(bool initialState):state(initialState),pressed(false){}
bool ToggleButton::update(int keyresult) {
		if (pressed and keyresult != GLFW_PRESS)
			pressed = false;
		else if (not pressed and keyresult == GLFW_PRESS) {
			pressed = true, state = not state;
			return true;
		}
		return false;
	}
bool ToggleButton::operator*() { return state; }

float ScreenInfo::aspectRatio() {
	return (float)width / height;
}

FBO::FBO():id(0){
	glGenFramebuffers(1, &id);
}

void FBO::bind(GLenum target) {
	glBindFramebuffer(target, id);
}

void FBO::unbind(GLenum target) {
	glBindFramebuffer(target, 0);
}

void FBO::texture2D(GLenum target, GLenum attachment, GLenum textarget, Texture &texture, GLint level) {
	glFramebufferTexture2D(target, attachment, textarget, texture.id, level);
}

void FBO::renderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, RBO& renderbuffer) {
	glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer.id);
}

GLenum FBO::checkStatus(GLenum target) {
	return glCheckFramebufferStatus(target);
}

void FBO::xassertComplete(GLenum target) {
	if (checkStatus(target) != GL_FRAMEBUFFER_COMPLETE) {
		lg.error("framebuffer not complete");
		std::exit(-1);
	}
}

RBO::RBO():id(0){
	glGenRenderbuffers(1, &id);
}

void RBO::bind(GLenum target) {
	glBindRenderbuffer(target, id);
}

void RBO::unbind(GLenum target) {
	glBindRenderbuffer(target, 0);
}

void RBO::storage(GLenum target, GLenum internalFormat, GLsizei width, GLsizei height) {
	glRenderbufferStorage(target, internalFormat, width, height);
}
