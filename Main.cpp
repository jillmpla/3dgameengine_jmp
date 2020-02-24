//C++
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <thread>
#include <algorithm>
#include <vector>
/*/////////////////////////////////////////////////////////////////////////////*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
/*/////////////////////////////////////////////////////////////////////////////*/
#include "Base/ShaderGL.hpp"
#include "Base/ModelLoader.hpp"
#include "Base/MeshShaderGL.hpp"
#include "Base/ModelGL.hpp"
#include "Base/MeshGL.hpp"
#include "Base/Camera.hpp"
#include "Base/Light.hpp"
/*Dear ImGui*/
#include "dearimgui/imgui.h"
#include "dearimgui/imgui_impl_glfw.h"
#include "dearimgui/imgui_impl_opengl3.h"
#include "dearimgui/imconfig.h"
#include "dearimgui/imgui_internal.h"
#include "dearimgui/imstb_rectpack.h"
#include "dearimgui/imstb_textedit.h"
#include "dearimgui/imstb_truetype.h"
/*JSON*/
#include "json.hpp"
using json = nlohmann::json;

using namespace std;

/*///////////////////////////////////////////////////////////////////////////////
GLOBALS
///////////////////////////////////////////////////////////////////////////////*/

GLFWwindow *window = NULL;
int windowWidth = 1920;
int windowHeight = 1080;

int framebufferWidth = windowWidth;
int framebufferHeight = windowHeight;

bool firstMouseMove = true;
double mouseX = -1;
double mouseY = -1;
bool leftMouseButtonDown = false;

ModelGL *modelGL = NULL;
const float TRANSLATION_INC = 0.1;

const float CAMERA_WALK_SPEED = .1f;
const float CAMERA_ROTATE_SPEED = 30.0f;

glm::vec3 eye1 = glm::vec3(-2, 2, 2);
glm::vec3 lookAt1 = glm::vec3(1, 0, 0);
glm::vec3 up1 = glm::vec3(0, 1, 0);
float fov1 = 60.0f;
float nearPlane1 = 0.01f;
float farPlane1 = 30.0f;
int bufferWidth1 = framebufferWidth;
int bufferHeight1 = framebufferHeight;

Camera *camera = new Camera (eye1, lookAt1, up1, fov1, nearPlane1, farPlane1, bufferWidth1, bufferHeight1);

glm::vec3 pos1 = glm::vec3(0, 3, 2);
glm::vec3 color1 = glm::vec3(1, 1, 1);
bool isPtLt = true;

Light *light = new Light(pos1, color1, isPtLt);

float shiny = 10.0;

MeshShaderGL *shader;

/*///////////////////////////////////////////////////////////////////////////////
FUNCTIONS - GLFW Callbacks
///////////////////////////////////////////////////////////////////////////////*/

// GLFW error callback
static void error_callback(int error, const char* description) {
	cerr << "ERROR " << error << ": " << description << endl;
}

// GLFW key callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {

		case GLFW_KEY_SPACE:
			modelGL->reset(); //call reset on modelGL
			break;
			
		case GLFW_KEY_U:
			modelGL->rotate(5.0f, glm::vec3(0, 0, 1)); //rotate modelGL by positive 5 degrees, z axis
			break;

		case GLFW_KEY_O:
			modelGL->rotate(-5.0f, glm::vec3(0, 0, 1)); //rotate modelGL by negative 5 degrees, z axis
			break;

		case GLFW_KEY_E:
			modelGL->rotate(5.0f, glm::vec3(1, 0, 0)); //rotate modelGL by positive 5 degrees, x axis
			break;

		case GLFW_KEY_R:
			modelGL->rotate(-5.0f, glm::vec3(1, 0, 0)); //rotate modelGL by negative 5 degrees, x axis
			break;

		case GLFW_KEY_T:
			modelGL->rotate(5.0f, glm::vec3(0, 1, 0)); //rotate modelGL by positive 5 degrees, y axis
			break;

		case GLFW_KEY_Y:
			modelGL->rotate(-5.0f, glm::vec3(0, 1, 0)); //rotate modelGL by negative 5 degrees, y axis
			break;

		case GLFW_KEY_I:
			modelGL->translate(glm::vec3(0, TRANSLATION_INC, 0)); //translate modelGL by positive TRANSLATION_INC in Y
			break;

		case GLFW_KEY_K:
			modelGL->translate(glm::vec3(0, -TRANSLATION_INC, 0)); //translate modelGL by negative TRANSLATION_INC in Y
			break;

		case GLFW_KEY_L:
			modelGL->translate(glm::vec3(TRANSLATION_INC, 0, 0)); //translate modelGL by positive TRANSLATION_INC in X
			break;

		case GLFW_KEY_J:
			modelGL->translate(glm::vec3(-TRANSLATION_INC, 0, 0)); //translate modelGL by negative TRANSLATION_INC in X
			break;

		case GLFW_KEY_W:
			camera->forward(CAMERA_WALK_SPEED);
			break;

		case GLFW_KEY_S:
			camera->forward(-CAMERA_WALK_SPEED);
			break;

		case GLFW_KEY_D:
			camera->strafeRight(CAMERA_WALK_SPEED);
			break;

		case GLFW_KEY_A:
			camera->strafeRight(-CAMERA_WALK_SPEED);
			break;

		case GLFW_KEY_C:
			if (light->getIsPointLight()) {
				light->setIsPointLight(false);
			}
			else {
				light->setIsPointLight(true);
			}
			break;

		case GLFW_KEY_0:
			shader->setMaterialChoice(0);
			break;

		case GLFW_KEY_1:
			shader->setMaterialChoice(1);
			break;

		case GLFW_KEY_V:
			shiny *= 5.0;
			shader->setShininess(shiny);

			break;

		case GLFW_KEY_B:
			shiny /= 5.0;
			if (shiny < 10.0) {
				shiny = 10.0;
				
			}
			shader->setShininess(shiny);
			break;

		case GLFW_KEY_N:
			glm::vec3 blue1 = glm::vec3(0, 0, 1);
			light->setColor(blue1); //blue
			break;

		case GLFW_KEY_M:
			glm::vec3 white1 = glm::vec3(1, 1, 1);
			light->setColor(white1); //white
			break;

		default:
			break;
		}
	}
}

/*// GLFW mouse movement callback
static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouseMove) {
		mouseX = xpos;
		mouseY = ypos;
		firstMouseMove = false;
	}

	double xDiff = xpos - mouseX;
	double yDiff = ypos - mouseY;

	xDiff /= framebufferWidth;
	yDiff /= framebufferHeight;

	camera->rotateRight(-xDiff * CAMERA_ROTATE_SPEED);
	camera->rotateUp(-yDiff * CAMERA_ROTATE_SPEED);

	mouseX = xpos;
	mouseY = ypos;
}

// GLFW callback for mouse BUTTONS
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			cout << "MOUSE LEFT DOWN" << endl;
			leftMouseButtonDown = true;
		}
		else if (action == GLFW_RELEASE) {
			cout << "MOUSE LEFT UP" << endl;
			leftMouseButtonDown = false;
		}
	}
}*/

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

/*///////////////////////////////////////////////////////////////////////////////
FUNCTIONS - Main
///////////////////////////////////////////////////////////////////////////////*/

int main(int argc, char **argv) {

	// If an argument is passed in, load a 3D file.
	// Otherwise, create a simple quad.
	string modelFilename = "../Objects/teddy.obj";
	if (argc >= 2) {
		modelFilename = string(argv[1]);
	}

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
	const char* glsl_version = "#version 330"; /*OpenGL 3.3, GLSL 330*/
	
	// Create our GLFW window
	window = glfwCreateWindow(	windowWidth, windowHeight, 
								"3D Game Engine",
								NULL, NULL);

	// Were we able to make it?
	if (!window) {
		// Kill GLFW and exit program
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// We want to draw to this window, so make the OpenGL context associated with this window our current context.
	glfwMakeContextCurrent(window);

	// Basically, turning VSync on (so we will wait until the screen is updated once before swapping the back and front buffers
	glfwSwapInterval(1);

	// Set our keyboard callback function, so that we can process keyboard input!
	glfwSetKeyCallback(window, key_callback);
	
	//////////////////////////////// Set our mouse callback function for when the mouse MOVES
	////////////////////////////////glfwSetCursorPosCallback(window, mouse_position_callback);

	/////////////////////////////// Set our mouse BUTTON callback function
	///////////////////////////////glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Set our callback for when the window changes size
	glfwSetWindowSizeCallback(window, window_size_callback);

	// Set our callback for when the framebuffer changes size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Hide cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	
	// GLEW setup //
	
	// MAC-SPECIFIC: Use the experimental version of GLEW
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

	/*/////////////////////////////////////////////////////////////////////////////*/
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Our state
	bool show_another_window = true;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	/*/////////////////////////////////////////////////////////////////////////////*/

	GLint glMajor, glMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
	glGetIntegerv(GL_MINOR_VERSION, &glMinor);
	cout << "OpenGL context version: ";
	cout << glMajor << "." << glMinor << endl;
	cout << "Supported GLSL version is ";
	cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	// Load shader program
	shader = new MeshShaderGL("../Basic.vs", "../Basic.ps", true);

	// Create/Get mesh data	
	ModelData *modelData = NULL;
	
	if (modelFilename != "") {
		cout << "Loading model: " << modelFilename << endl;

		// Load model		
		modelData = loadModel(modelFilename);

		// Did we load the model?
		if (!modelData) {
			cout << "ERROR: Failed to set up model data." << endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		// Override color per-vertex		
		vector<Vertex>* vertices = modelData->getMesh(0)->getVertices();
		for (int i = 0; i < vertices->size(); i++) {
			glm::vec3 pos = vertices->at(i).pos;
			pos += 1.0;
			pos /= 2.0;
			vertices->at(i).color = glm::vec4(pos.x, pos.y, pos.z, 1.0);
		}		
	}
	else {	
		cout << "Creating mesh from scratch..." << endl;

		// Create the mesh data
		vector<Vertex> verts;
		vector<unsigned int> ind;

		// Add four corners
		verts.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.0), glm::vec4(1.0, 0.0, 0.0, 1.0), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec4(1, 0, 0, 1)));
		verts.push_back(Vertex(glm::vec3(0.5, -0.5, 0.0), glm::vec4(0.0, 1.0, 0.0, 1.0), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec4(1, 0, 0, 1)));
		verts.push_back(Vertex(glm::vec3(0.5, 0.5, 0.0), glm::vec4(0.0, 0.0, 1.0, 1.0), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec4(1, 0, 0, 1)));
		verts.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.0), glm::vec4(1.0, 1.0, 1.0, 1.0), glm::vec3(0, 0, 1), glm::vec2(0, 0), glm::vec4(1, 0, 0, 1)));

		// Add indices for two triangles
		ind.push_back(0);
		ind.push_back(1);
		ind.push_back(2);

		ind.push_back(0);
		ind.push_back(2);
		ind.push_back(3);

		// Create mesh data
		MeshData *mesh = new MeshData(verts, ind);

		// Create a simple model and add mesh to it
		modelData = new ModelData();
		modelData->addMesh(mesh, 0);
	}

	modelGL = new ModelGL(modelData);

	// Don't need local data anymore, so we can delete it
	delete modelData;

	// Set the background color to a shade of blue
	glClearColor(0.0f, 0.0f, 0.7f, 1.0f);	

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable shader program
	shader->activate();
	shader->setShininess(shiny);
	shader->setMaterialChoice(0);
	
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//io.MousePos = my_mouse_pos;

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetMouseCursor(ImGuiMouseCursor_None);
		ImGui:(io.WantCaptureMouse, true);

		/*With previous implementations then only relevant variable to tell who draws the cursor was io.MouseDrawCursor:

		imgui: io.MouseDrawCursor is true
		glfw : io.MouseDrawCursor is false
		Now NoMouseCursorChange is also relevant :

		imgui: io.MouseDrawCursor is true and NoMouseCursorChange is false
		glfw : io.MouseDrawCursor is false and NoMouseCursorChange is true*/

		// 1st window
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Menu");                                    // Create a window called "Menu" and append into it.
			ImGui::Text("This is some useful text.");                // Display some text (you can use a format strings too)
			ImGui::Checkbox("Another Window", &show_another_window); // Edit bools storing our window open/close state

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color);  // floats representing a color

			if (ImGui::Button("Button"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 2nd window
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// Rendering!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		ImGui::Render();
		camera->updateBufferSize(framebufferWidth, framebufferHeight);
		shader->setViewAndProjection(camera);
		shader->setLight(light);
		/*/////////////////////////////////////////////////////////////////////////////*/
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		/*Clear the framebuffer*/
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/*Draw the mesh using ModelGL's draw() method (passing in the shader)*/
		modelGL->draw(shader);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	// Cleanup		
	delete shader;

	// Delete instance of ModelGL
	delete modelGL;

	// Teardown dearimgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
		
	// Destroy window and stop GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
