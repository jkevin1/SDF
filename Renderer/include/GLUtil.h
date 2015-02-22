#pragma once

#include <gl\glew.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <list>

namespace GLUtil {

	//Creates a shader object from glsl source
	GLuint createShader(GLenum type, const char* src);

	//Creates a shader object from a source file
	GLuint loadShader(GLenum type, const char* filename);

	//Creates a shader program from a list of shader objects
	GLuint createProgram(std::list<GLuint> shaders);


}