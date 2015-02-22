#include "gldemo.h"
#include <stdio.h>
#include <stdlib.h>

void GLDemo::glfwErrorCallback(int error, const char* description) {
	fprintf(stderr, "GLFW error: %s\n", description);
}

void GLDemo::glfwResizeCallback(GLFWwindow* win, int width, int height) {
	GLDemo* instance = (GLDemo*)glfwGetWindowUserPointer(win);
	instance->width = width;
	instance->height = height;
	instance->onResize(width, height);
}

void GLDemo::glfwMouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
	GLDemo* instance = (GLDemo*)glfwGetWindowUserPointer(win);
	if (action == GLFW_PRESS)
		instance->onMousePress(button);
	else
		instance->onMouseRelease(button);
}

void GLDemo::glfwMouseMoveCallback(GLFWwindow *win, double x, double y) {
	GLDemo* instance = (GLDemo*)glfwGetWindowUserPointer(win);
	instance->onMouseMove(x, y);
}

void GLDemo::glfwKeyCallback(GLFWwindow* win, int key, int scancode, int action, int mods) {
	GLDemo* instance = (GLDemo*)glfwGetWindowUserPointer(win);
	if (action == GLFW_PRESS)
		instance->onKeyPress(key);
	else
		instance->onKeyRelease(key);
}

void GLDemo::recreate() {
	window = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : nullptr, 0);
	if (!window) {
		fprintf(stderr, "Failed to create window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Glew failed to initialize \n");
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetFramebufferSizeCallback(window, glfwResizeCallback);
	glfwSetMouseButtonCallback(window, glfwMouseButtonCallback);
	glfwSetCursorPosCallback(window, glfwMouseMoveCallback);
	glfwSetKeyCallback(window, glfwKeyCallback);

	glfwSetWindowUserPointer(window, this);
}

GLDemo::GLDemo(int gl_major_version, int gl_minor_version, bool debug) : window(nullptr) {
	glfwSetErrorCallback(glfwErrorCallback);
	if (!glfwInit()) exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major_version);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor_version);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, debug ? GL_TRUE : GL_FALSE);

	//Set defaults
	title = "";
	antialias = false;
	fullscreen = false;
	width = 640;
	height = 480;
}

void GLDemo::setWindowed(int width, int height, bool resizeable) {
	if (window) glfwDestroyWindow(window);
	window = nullptr;

	fullscreen = false;
	this->width = width;
	this->height = height;
	glfwWindowHint(GLFW_RESIZABLE, resizeable ? GL_TRUE : GL_FALSE);
}

void GLDemo::setFullscreen(bool borderless) {
	if (window) glfwDestroyWindow(window);
	window = nullptr;

	fullscreen = !borderless;
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	width = mode->width;
	height = mode->height;

	if (borderless) {
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	}
}

GLDemo::~GLDemo() {
	if (window)	glfwDestroyWindow(window);
	glfwTerminate();
}

void GLDemo::run() {
	if (!window) recreate();
	init();

	time = 0.0;
	frames = 0;
	curr = glfwGetTime();

	while (!glfwWindowShouldClose(window)) {
		prev = curr;
		curr = glfwGetTime();
		double dt = curr - prev;
		
		frames++;
		time += dt;

		if (time > 1.0) {
			printf("Average Frame Time: %fms\n", 1000.0 / double(frames));
			frames = 0;
			time -= 1.0;
		}

		update(dt);
		render();

		glfwSwapBuffers(window);
		glfwPollEvents();
		if (!window) recreate();
	}

	close();
}

void GLDemo::stop() {
	glfwSetWindowShouldClose(window, GL_TRUE);
}

//TODO: remove these, theyre just here for testing, add empty bodies to virtual functions in header
#include <math.h>
void GLDemo::render() {
	 static float time = 0.0f;
	 time += 0.01f;
	 glClearColor((sinf(time) + 1) / 2, (cosf(time) + 1) / 2, 0.0f, 0.0f);
	 glClear(GL_COLOR_BUFFER_BIT);
}

void GLDemo::onResize(int width, int height) {
	printf("Window Resized: [%dx%d]\n", width, height);
}

void GLDemo::onMousePress(int button) {
	printf("Mouse Pressed: %d\n", button);
}

void GLDemo::onMouseRelease(int button) {
	printf("Mouse Released: %d\n", button);
}

void GLDemo::onMouseMove(double x, double y) {
	printf("Mouse Moved: (%f, %f)\n", x, y);
}

void GLDemo::onKeyPress(int key) {
	printf("Key Pressed: %d\n", key);
}

void GLDemo::onKeyRelease(int key) {
	switch (key) {
	case GLFW_KEY_1:
		setWindowed(1024, 768, true);
		break;
	case GLFW_KEY_2:
		setWindowed(320, 240, false);
		break;
	case GLFW_KEY_3:
		setFullscreen();
		break;
	case GLFW_KEY_4:
		setFullscreen(true);
		break;
	default:
		printf("Key Released: %d\n", key);
	}
}