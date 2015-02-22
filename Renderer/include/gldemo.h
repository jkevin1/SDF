#ifndef GLDEMO_H
#define GLDEMO_H

#include <gl\glew.h>
#include <GLFW\glfw3.h>

class GLDemo {
	//Pointer to the window
	GLFWwindow* window;

	//Window settings
	const char* title;
	bool antialias, fullscreen;
	int width, height;

	//Variables used for calculating frame time
	double time, prev, curr;
	int frames;

	static void glfwErrorCallback(int error, const char* description);
	static void glfwResizeCallback(GLFWwindow* win, int width, int height);
	static void glfwMouseButtonCallback(GLFWwindow *win, int button, int action, int mods);
	static void glfwMouseMoveCallback(GLFWwindow *win, double x, double y);
	static void glfwKeyCallback(GLFWwindow* win, int key, int scancode, int action, int mods);

	void recreate();
public:
	//Constructor for a standard window
	GLDemo(int gl_major_version, int gl_minor_version, bool debug = false);

	//Virtual destructor
	virtual ~GLDemo();

	void setTitle(const char* title);
	void setAntialias(bool antialias);
	void setVSync(bool vsync);
	void setWindowed(int width, int height, bool resizeable = true);
	void setFullscreen(bool borderless = false);	//Uses native resolution

	//Functions to start and stop the demo
	void run();
	void stop();

	//Virtual functions called each frame
	virtual void init() { }
	virtual void update(double time) { }
	virtual void render();
	virtual void close() { }

	//TODO: add utility functions, e.g. show/hide cursor

	virtual void onResize(int width, int height);
	virtual void onMousePress(int button);
	virtual void onMouseRelease(int button);
	virtual void onMouseMove(double x, double y);
	virtual void onKeyPress(int key);
	virtual void onKeyRelease(int key);
	//TODO: add more callback functions, e.g. onClose()
};

#endif