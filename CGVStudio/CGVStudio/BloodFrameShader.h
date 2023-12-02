#pragma once

#ifndef BloodFrameShader_hpp
#define BloodFrameShader_hpp

#include <stdio.h>
#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include <iostream>
#include <assert.h>
#include "../../src/color.h"
#include "../../src/vector.h"
#include "../../src/matrix.h"
#include "../../src/camera.h"
#include "../../src/baseshader.h"
#include "../../src/phongshader.h"

class OffscreenShader : public BaseShader
{
public:
	OffscreenShader();
	virtual void activate(const BaseCamera& Cam) const;
	void setTexture(const Texture* texture) { this->tex = texture; };
protected:
	void assignLocations();
private:
	const Texture* tex;
	GLuint ShaderProgram;
	GLint screenTextureLoc;
};

#endif /* BloodFrameShader_hpp */