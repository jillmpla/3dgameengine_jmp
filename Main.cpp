#include <iostream>
#include <cstring>
#include <thread>
#include <algorithm>
#include <vector>
#include <GL/glew.h>					
#include <GLFW/glfw3.h>
#include <Render/Render.hpp>

using namespace std;

GLFWwindow* window = NULL;
int windowWidth = 800;
int windowHeight = 600;

int framebufferWidth = windowWidth;
int framebufferHeight = windowHeight;

// GLFW error callback
static void error_callback(int error, const char* description) {
    cerr << "ERROR " << error << ": " << description << endl;
}

// GLFW callback when the window changes size
void window_size_callback(GLFWwindow* window, int width, int height) {
    windowWidth = width;
    windowHeight = height;
}

// GLFW callback when the framebuffer changes size
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    framebufferWidth = width;
    framebufferHeight = height;
}

// process input
// close window with escape key
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main() {
	// GLFW setup //

	// Set error callback
	glfwSetErrorCallback(error_callback);

	// (Try to) initialize GLFW
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// MAC-SPECIFIC: Convince Mac that, yes, we want to use OpenGL version 3.3 (and NOT 2.1)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create our GLFW window
	window = glfwCreateWindow(windowWidth, windowHeight,
		"Window",
		NULL, NULL);

	// Did window get created...
	if (!window) {
		// Kill GLFW and exit program
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// We want to draw to this window, so make the OpenGL context associated with this window our current context.
	glfwMakeContextCurrent(window);

	// Basically, turning VSync on (so we will wait until the screen is updated once before swapping the back and front buffers
	glfwSwapInterval(1);

	// Set our callback for when the window changes size
	glfwSetWindowSizeCallback(window, window_size_callback);

	// Set our callback for when the framebuffer changes size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLEW setup //

	// MAC-SPECIFIC: Some issues occur with using OpenGL core and GLEW; so, we'll use the experimental version of GLEW
	glewExperimental = true;

	// (Try to) initalize GLEW
	GLenum err = glewInit();

	// Call glGetError() to clear it out (can occur after glewInit() for some reason)
	glGetError();

	if (GLEW_OK != err) {
		// We couldn't start GLEW, so we've got to go.
		// Kill GLFW and get out of here
		cout << "ERROR: GLEW could not start: " << glewGetErrorString(err) << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	cout << "GLEW initialized; version ";
	cout << glewGetString(GLEW_VERSION) << endl;

	/****************************************************************************************/
	/*string modelFilename = "../Models/teapot.obj";
	cout << "Loading model: " << modelFilename << endl;

	// Create/Get mesh data	
	ModelData* modelData = NULL;

	// Load model		
	modelData = loadModel(modelFilename);

	// Did we load the model?
		if (!modelData) {
			cout << "ERROR: Failed to set up model data." << endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}*/
	/****************************************************************************************/

	// glfw window render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// rendering commands here

		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		glViewport(0, 0, framebufferWidth, framebufferHeight);
		// Swap buffers and poll for window events		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Destroy window and stop GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}