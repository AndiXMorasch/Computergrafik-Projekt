#include "PortalShader.h"

const char* PVertexShaderCode =
"#version 400\n"
"layout(location=0) in vec4 VertexPos;"
"layout(location=1) in vec4 VertexNormal;"
"out vec3 Pos;"
"out vec3 Normal;"
"uniform mat4 ModelViewProjMat;"
"uniform mat4 ModelMat;"
"void main()"
"{"
"	 Pos = (ModelMat * VertexPos).xyz;"
"	 Normal = (ModelMat * VertexNormal).xyz;"
"    gl_Position = ModelViewProjMat * VertexPos;"
"}";

const char* PFragmentShaderCode =
"#version 400\n"
"in vec3 Pos;"
"in vec3 Normal;"
"out vec4 FragColor;"
"uniform float Time;"
"void main()"
"{"
"	 float theta = Time / 25;"
"    float diffuse1 = pow(sin(theta),2.0);"
"	 float diffuse2 = pow(cos(theta),2.0);"
"	 vec3 col1 = (1.0f - diffuse1) * vec3(0);"
"	 vec3 col2 = (1.0f - diffuse2) * vec3(1);"
"	 FragColor = vec4(col1 + col2, 0.2f);"
"}";

// Shaderinspiration: http://shdr.bkcore.com/#1/dZI9b8MgEIb/CmLCKUpt11OiTpW6terUpakiamOHyAYLcNImyn/vAbGdz8EycM/d+97BHhcq7xourcGzL9xqngsjlEQrUa1aVNaK2flCbpj+E7JCG54/ofJd6YbVcNxJUcI6YMiKhsMh0EoUqGFCkmgh9wuJemDFLUPPHpyk8TSeu5j7fN1C6ASibk1yZYjHIxpTA5XCJpqf4WmPjwTgY240CAQDhSjLznCn0qotKZQlx2aoE49oOo1Dzhmf3ubTE35wlavalR+UJsFgAr6OaI+lI5b2GEA0GStW9fJVs+pF1UqHVjPiBR58AYqSoH/A9O7VMWu1+OksD8KtMsICdR2Rl7faMNufvjFAf89jGWpUwetPwbe3w61Wa547sSF+5x3dfDIhCm0HC2LHyakZGFnYDlPNXHPAX9gKs81I3/kwNT/fj+Opu+ILv5AIOX683xT7qngW/ubRdDsmJV+6YU/XBh/+AQ==

PortalShader::PortalShader()
{
	ShaderProgram = createShaderProgram(PVertexShaderCode, PFragmentShaderCode);

	assignLocations();
}

// hier die benötigten Locations assignen
void PortalShader::assignLocations()
{
	ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
	ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
	TimeLoc = glGetUniformLocation(ShaderProgram, "Time");
}

// hier die Locations mit Daten befüllen
void PortalShader::activate(const BaseCamera& Cam) const
{
	BaseShader::activate(Cam);

	// always update matrices
	Matrix ModelView = Cam.getViewMatrix() * ModelTransform;
	Matrix ModelViewProj = Cam.getProjectionMatrix() * ModelView;
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, modelTransform().m);
	glUniformMatrix4fv(ModelViewProjLoc, 1, GL_FALSE, ModelViewProj.m);

	time += 0.01f;
	glUniform1f(TimeLoc, time);
}