#include "WindowManager.h"
#include <stdio.h>
#include <stdlib.h>

static void glfwErrorCallback(int error, const char* description) {
	fprintf(stderr, "GLFW error: %s\n", description);
}

void Window::setContextVersion(int major, int minor, bool compat, bool forward) {
	static bool first = true;
	if (first) {
		glfwSetErrorCallback(glfwErrorCallback);
		if (!glfwInit()) {
			fprintf(stderr, "Failed to initialize GLFW\n");
			exit(EXIT_FAILURE);
		}
		atexit(glfwTerminate);
		first = false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, compat ? GLFW_OPENGL_COMPAT_PROFILE : GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, forward ? GL_TRUE : GL_FALSE);
}

void Window::enableDebugContext(bool debug) {
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, debug ? GL_TRUE : GL_FALSE);
}

void Window::setSamples(int samples) {
	glfwWindowHint(GLFW_SAMPLES, samples);
}

int Window::getScreenWidth() {
	return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
}

int Window::getScreenHeight() {
	return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
}

GLFWwindow* Window::create(const char* title, int width, int height, int flags) {
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, (flags & FLAG_RESIZEABLE) ? GL_TRUE : GL_FALSE);
	glfwWindowHint(GLFW_DECORATED, (flags & FLAG_BORDERLESS) ? GL_FALSE : GL_TRUE);
	GLFWmonitor* monitor = (flags & FLAG_FULLSCREEN) ? glfwGetPrimaryMonitor() : nullptr;
	GLFWwindow* window = glfwCreateWindow(width, height, title, monitor, nullptr);
	if (!window) return nullptr;
	if (flags & FLAG_CENTERED) {
		glfwGetWindowSize(window, &width, &height);
		glfwSetWindowPos(window, (getScreenWidth() / 2) - (width / 2), (getScreenHeight() / 2) - (height / 2));
	}
	if (flags & FLAG_VISIBLE) glfwShowWindow(window);
	return window;
}

bool Window::createContext(GLFWwindow* window) {
	glfwMakeContextCurrent(window);
	glewExperimental = true;
	return glewInit() == GLEW_OK;
}