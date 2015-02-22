#pragma once

#include <gl\glew.h>
#include <GLFW\glfw3.h>

//Flag to make the window resizeable
#define FLAG_RESIZEABLE 1 << 0
//Flag to create a borderless window
#define FLAG_BORDERLESS 1 << 1
//Flag to make the window fullscreen
#define FLAG_FULLSCREEN 1 << 2
//Flag to make the window visible immediately
#define FLAG_VISIBLE	1 << 3
//Flag to center the window
#define FLAG_CENTERED	1 << 4

namespace Window {
	/* TODO:
	add more flags
	add more functions
	...	*/

	//Initializes for a specific GL context version. Must be called first
	void setContextVersion(int major, int minor, bool compat, bool forward);

	//Enables debug context, must be called before window creation
	void enableDebugContext(bool debug);

	//Sets the number of samples for MSAA, must be called before window creation
	void setSamples(int samples);

	//Returns the width of the primary monitor in pixels
	int getScreenWidth();

	//Returns the height of the primary monitor in pixels
	int getScreenHeight();

	//Creates a window with the specified parameters
	GLFWwindow* create(const char* title, int width, int height, int flags);

	//Makes the context current and loads GLEW
	bool createContext(GLFWwindow* window);
}