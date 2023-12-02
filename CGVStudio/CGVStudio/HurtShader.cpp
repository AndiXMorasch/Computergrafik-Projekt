#include "HurtShader.h"

const char* RVertexShaderCode =
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

const char* RFragmentShaderCode =
"#version 400\n"
"in vec3 Pos;"
"in vec3 Normal;"
"out vec4 FragColor;"
"uniform float Time;"
"void main()"
"{"
"	 float theta = Time*2;"
"	 vec3 dir1 = vec3(sin(theta),0,cos(theta));"
"	 vec3 dir2 = vec3(sin(theta),0,cos(theta));"
"    float diffuse1 = pow(dot(Normal,dir1),2.0);"
"	 float diffuse2 = pow(dot(Normal,dir2),2.0);"
"	 vec3 col1 = diffuse1 * vec3(1,0,0);"
"	 vec3 col2 = diffuse2 * vec3(1,0,0);"
"	 FragColor = vec4(col1 + col2, 1.0);"
"}";

// Shaderinspiration: http://shdr.bkcore.com/#1/dZI9b8MgEIb/CmLCKUpt11OiTpW6terUpakiamOHyAYLcNImyn/vAbGdz8EycM/d+97BHhcq7xourcGzL9xqngsjlEQrUa1aVNaK2flCbpj+E7JCG54/ofJd6YbVcNxJUcI6YMiKhsMh0EoUqGFCkmgh9wuJemDFLUPPHpyk8TSeu5j7fN1C6ASibk1yZYjHIxpTA5XCJpqf4WmPjwTgY240CAQDhSjLznCn0qotKZQlx2aoE49oOo1Dzhmf3ubTE35wlavalR+UJsFgAr6OaI+lI5b2GEA0GStW9fJVs+pF1UqHVjPiBR58AYqSoH/A9O7VMWu1+OksD8KtMsICdR2Rl7faMNufvjFAf89jGWpUwetPwbe3w61Wa547sSF+5x3dfDIhCm0HC2LHyakZGFnYDlPNXHPAX9gKs81I3/kwNT/fj+Opu+ILv5AIOX683xT7qngW/ubRdDsmJV+6YU/XBh/+AQ==

HurtShader::HurtShader()
{
	ShaderProgram = createShaderProgram(RVertexShaderCode, RFragmentShaderCode);

	assignLocations();
}

// hier die benötigten Locations assignen
void HurtShader::assignLocations()
{
	ModelMatLoc = glGetUniformLocation(ShaderProgram, "ModelMat");
	ModelViewProjLoc = glGetUniformLocation(ShaderProgram, "ModelViewProjMat");
	TimeLoc = glGetUniformLocation(ShaderProgram, "Time");
}

// hier die Locations mit Daten befüllen
void HurtShader::activate(const BaseCamera& Cam) const
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