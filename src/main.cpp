#ifdef WIN32
#include <GL/glew.h>
#include <glfw/glfw3.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#include <glfw/glfw3.h>
#endif
#include <stdio.h>
#include "Application.h"
#include "freeimage.h"
#include "irrKlang.h"

using namespace irrklang;
using namespace std;

#pragma comment(lib, "irrKlang.lib")

void PrintOpenGLVersion();

int main(int argc, char** argv) {

	ISoundEngine* engine = createIrrKlangDevice();
	if (engine) {
		engine->play2D(ASSET_DIRECTORY "gamesound.mp3", true);
	}

	FreeImage_Initialise();
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

#ifdef __APPLE__
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

	const int WindowWidth = 800;
	const int WindowHeight = 600;

	GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "Slither Snake", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: can not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

#if WIN32
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	PrintOpenGLVersion();

	// Initialize TextRenderer
	TextRenderer* textRenderer = new TextRenderer();

	{
		Application App(window, engine, textRenderer);
		App.start();

		float previousTime;
		float currentTime = glfwGetTime();

		while (!glfwWindowShouldClose(window)) {

			// previousTime ersetzen mit currentTime
			previousTime = currentTime;
			// currentTime neuen Wert zuweisen
			currentTime = glfwGetTime();
			// deltaTime ist die Different zwischen currentTime und previousTime
			float dTime = currentTime - previousTime;

			glfwPollEvents();
			App.update(dTime);
			App.draw();

			glfwSwapBuffers(window);
		}
		App.end();
	}

	engine->drop();
	glfwTerminate();
	return 0;
}

void PrintOpenGLVersion()
{
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
}