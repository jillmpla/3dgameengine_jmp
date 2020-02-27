//C++
#include <cstdio>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
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
#include "base/ShaderGL.hpp"
#include "base/ModelLoader.hpp"
#include "base/MeshShaderGL.hpp"
#include "base/ModelGL.hpp"
#include "base/MeshGL.hpp"
#include "base/Camera.hpp"
#include "base/Light.hpp"
/*Dear ImGui*/
#include "libs/dearimgui/imgui.h"
#include "libs/dearimgui/imgui_impl_glfw.h"
#include "libs/dearimgui/imgui_impl_opengl3.h"
#include "libs/dearimgui/imconfig.h"
#include "libs/dearimgui/imgui_internal.h"
#include "libs/dearimgui/imstb_rectpack.h"
#include "libs/dearimgui/imstb_textedit.h"
#include "libs/dearimgui/imstb_truetype.h"
/*JSON*/
#include "libs/json.hpp"
using json = nlohmann::json;
/*TinyGLTF*/
#define TINYGLTF_IMPLEMENTATION
#include "libs/tiny_gltf.h"
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libs/stb_image_write.h"
/*PXRender* - try this out...maybe or delete
#define PX_RENDER
#include "libs/px_render.h"
#define PX_RENDER_GLTF_IMPLEMENTATION
#include "libs/px_render_gltf.h"
#define PX_RENDER_IMGUI_IMPLEMENTATION
#include "libs/px_render_imgui.h"*/
/*imfilebrowser*/
#include "libs/imfilebrowser.h"

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

//tinyglft///////////////////////////////////////////////////////////////////////
static std::string GetFilePathExtension(const std::string& FileName) {
	if (FileName.find_last_of(".") != std::string::npos)
		return FileName.substr(FileName.find_last_of(".") + 1);
	return "";
}

bool ret = false;
tinygltf::Model model;
tinygltf::TinyGLTF loader;
std::string errp;
std::string warn;
std::string ext;

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
			modelGL->reset();
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

// GLFW mouse movement callback
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

/*///////////////////////////////////////////////////////////////////////////////
FUNCTIONS - Main
///////////////////////////////////////////////////////////////////////////////*/

int main(int argc, char **argv) {

	// imgui-filebrowser - create a file browser instance
	ImGui::FileBrowser fileDialog;
	fileDialog.SetTitle("Load .gltf or .glb");
	fileDialog.SetTypeFilters({ ".gltf", ".glb" });

	// If an argument is passed in, load a 3D file.
	// Otherwise, create a simple quad.
	string modelFilename = "../objs/teddy.obj";
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
	
	// Create GLFW window
	window = glfwCreateWindow(	windowWidth, windowHeight, 
								"Beary",
								NULL, NULL);

	// Check that is was created
	if (!window) {
		// Kill GLFW and exit program
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Draw to this window.
	glfwMakeContextCurrent(window);

	// Change window's icon, so it's more Beary-themed
	GLFWimage images[1];
	images[0].pixels = stbi_load("../pngs/teddy-bear-icon.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);

	// Basically, turning VSync on (so we will wait until the screen is updated once before swapping the back and front buffers
	glfwSwapInterval(1);

	// Set our keyboard callback function, so that we can process keyboard input!
	glfwSetKeyCallback(window, key_callback);
	
	// Set our mouse callback function for when the mouse MOVES
	glfwSetCursorPosCallback(window, mouse_position_callback);

	// Set our mouse BUTTON callback function
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Set our callback for when the window changes size
	glfwSetWindowSizeCallback(window, window_size_callback);

	// Set our callback for when the framebuffer changes size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	glfwSetCursor(window, cursor);
	
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

	/*/////////////////////////////////////////////////////////////////////////////*/
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Dear ImGui style
	ImGui::StyleColorsDark();

	// Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// State
	bool show_file_load_window = false;
	bool save_file_load_window = false;
	bool load_an_obj_file = false;
	bool edit_obj_color = false;
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
	shader = new MeshShaderGL("../base/Basic.vs", "../base/Basic.ps", true);

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
	
	// Main Dear ImGui window////////////////////////////////////////////////////////////////////////////////////////////////////////
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//ImGui::SetNextWindowSize(ImVec2(200, 200), ImGuiCond_FirstUseEver);

		// Main window
		{
			//static float f = 0.0f;

			ImGui::Begin("Main Menu");
			ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
			ImGui::Separator();
			ImGui::Text("Save/Load");
			if (ImGui::Button("Save .gltf/.glb File")) {
				save_file_load_window = true;
			}
			if (ImGui::Button("Load .gltf/.glb File")) {
				show_file_load_window = true;
			}
			if (ImGui::Button("Load .obj")) {
				load_an_obj_file = true;
			}
			ImGui::Separator();
			ImGui::Text("Change Background Color");
			ImGui::ColorEdit3("Color", (float*)&clear_color); 
			ImGui::End();
		}

		// Edit color of object
		if (edit_obj_color)
		{
			ImGui::Begin("Load an Object", &edit_obj_color);
			ImGui::Text("");
			if (ImGui::Button("Close")) {
				edit_obj_color = false;
			}
			ImGui::End();
		}

		// Load .obj File
		if (load_an_obj_file)
		{
			ImGui::Begin("Load .obj", &load_an_obj_file);
			ImGui::Text("");
			if (ImGui::Button("Close")) {
				load_an_obj_file = false;
			}
			ImGui::End();
		}

		// .gltf/.glb File Save Window
		if (save_file_load_window)
		{
			ImGui::Begin(".gltf/.glb File Save", &save_file_load_window);
			ImGui::Text("Save .gltf/.glb File:");
			if (ImGui::Button("Save File")) {
				// empty
			}
			if (ImGui::Button("Close")) {
				save_file_load_window = false;
			}
			ImGui::End();
		}

		//.gltf/.glb File Loader Window
		if (show_file_load_window)
		{
			ImGui::Begin(".gltf/.glb File Loader", &show_file_load_window);
			ImGui::Text("Open .gltf/.glb File:");
			if (ImGui::Button("Open File"))
				fileDialog.Open();
				fileDialog.Display();
				if (fileDialog.HasSelected())
				{
					string fileName = fileDialog.GetSelected().string();
					ext = GetFilePathExtension(fileName);
					bool ret = false;

					if (ext == "glb") {
						// binary glTF - .glb
						ret = loader.LoadBinaryFromFile(&model, &errp, &warn, fileName);

						std::cout << "loaded .glb file has:\n"
							<< model.accessors.size() << " accessors\n"
							<< model.animations.size() << " animations\n"
							<< model.buffers.size() << " buffers\n"
							<< model.bufferViews.size() << " bufferViews\n"
							<< model.materials.size() << " materials\n"
							<< model.meshes.size() << " meshes\n"
							<< model.nodes.size() << " nodes\n"
							<< model.textures.size() << " textures\n"
							<< model.images.size() << " images\n"
							<< model.skins.size() << " skins\n"
							<< model.samplers.size() << " samplers\n"
							<< model.cameras.size() << " cameras\n"
							<< model.scenes.size() << " scenes\n"
							<< model.lights.size() << " lights\n";

					}
					else {
						// ASCII glTF - .gltf
						ret = loader.LoadASCIIFromFile(&model, &errp, &warn, fileName);

						std::cout << "loaded .gltf file has:\n"
							<< model.accessors.size() << " accessors\n"
							<< model.animations.size() << " animations\n"
							<< model.buffers.size() << " buffers\n"
							<< model.bufferViews.size() << " bufferViews\n"
							<< model.materials.size() << " materials\n"
							<< model.meshes.size() << " meshes\n"
							<< model.nodes.size() << " nodes\n"
							<< model.textures.size() << " textures\n"
							<< model.images.size() << " images\n"
							<< model.skins.size() << " skins\n"
							<< model.samplers.size() << " samplers\n"
							<< model.cameras.size() << " cameras\n"
							<< model.scenes.size() << " scenes\n"
							<< model.lights.size() << " lights\n";

					}

					if (!warn.empty()) {
						std::cerr << "glTF parse warning: " << warn << std::endl;
					}

					if (!errp.empty()) {
						std::cerr << "glTF parse error: " << errp << std::endl;
					}

					if (!ret) {
						std::cerr << "Failed to load glTF: " << fileName << std::endl;
						return false;
					}
					fileDialog.ClearSelected();
				}
				if (ImGui::Button("Close")) {
					show_file_load_window = false;
				}
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
