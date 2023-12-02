#include "BloodFrameShader.h"

const char* OVertexShaderCode =
"#version 400\n"
"layout(location=0) in vec4 inPos;"
"layout(location=2) in vec2 inTexCoords;"
"out vec2 texCoords;"
"void main()"
"{"
"    gl_Position = vec4(inPos.x, -inPos.z, 0, 1.0);"
"	 texCoords = vec2(inTexCoords.x, -inTexCoords.y);"
"}";

const char* OFragmentShaderCode =
"#version 400\n"
"in vec2 texCoords;"
"out vec4 FragColor;"
"uniform sampler2D screenTexture;"
"float rand(vec2 coords)"
"{"
"	return fract(sin(dot(coords.xy, vec2(12.9898, 78.233))) * 43758.5453);"
"}"
"void main()"
"{"
"	 float randomValue = rand(texCoords);"
"	 if(texCoords.x < 0.5 && (randomValue < (0.3 - texCoords.x)) || texCoords.x > 0.5 && (randomValue < (texCoords.x - 0.7))) {"
"		FragColor = vec4(1, 0, 0, 1);"
"	 } else if(-texCoords.y < 0.5 && (randomValue < (0.3 + texCoords.y)) || -texCoords.y > 0.5 && (randomValue < (-texCoords.y - 0.7))) {"
"		FragColor = vec4(1, 0, 0, 1);"
"	 } else {"
"		FragColor = vec4(texture(screenTexture, texCoords).rgb, 1);"
"	 }"
"}";

OffscreenShader::OffscreenShader()
{
	ShaderProgram = createShaderProgram(OVertexShaderCode, OFragmentShaderCode);

	assignLocations();
}

// hier die benötigten Locations assignen
void OffscreenShader::assignLocations()
{
	screenTextureLoc = glGetUniformLocation(ShaderProgram, "screenTexture");
}

// hier die Locations mit Daten befüllen
void OffscreenShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	tex->activate(0);
	glUniform1i(screenTextureLoc, 0);
}

