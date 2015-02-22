#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Texture {
    GLuint id;
    unsigned width, height;
public:
    Texture(const char* filename); //cannot be called before an opengl context is created, uses default filter
    Texture(const char* filename, GLenum minFilter, GLenum maxFilter);  //cannot be called before an opengl context is created

    virtual ~Texture();

    void bind(unsigned texture);

    unsigned getWidth();
    unsigned getHeight();
};

#endif // TEXTURE_H
