//C++
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <cstring>
#include <thread>
#include <algorithm>
#include <vector>
/*/////////////////////////////////////////////////////////////////////////////*/
#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
#ifndef M_PIl
#define M_PIl (3.14159265358979323846264338327950288)
#endif
/*/////////////////////////////////////////////////////////////////////////////*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
/*/////////////////////////////////////////////////////////////////////////////*/
#include "core/ShaderGL.hpp"
#include "core/ModelLoader.hpp"
#include "core/MeshShaderGL.hpp"
#include "core/ModelGL.hpp"
#include "core/MeshGL.hpp"
#include "core/Camera.hpp"
#include "core/Light.hpp"
#include "core/MousePicker.hpp"
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
/*imfilebrowser*/
#include "libs/imfilebrowser.h"

using namespace std;

/*///////////////////////////////////////////////////////////////////////////////
GLOBALS
///////////////////////////////////////////////////////////////////////////////*/

GLFWwindow* window = NULL;
int windowWidth = 1920;
int windowHeight = 1080;

int framebufferWidth = windowWidth;
int framebufferHeight = windowHeight;

bool firstMouseMove = true;
double mouseX = -1;
double mouseY = -1;
bool leftMouseButtonDown = false;
static double cursor_pos_x = 0;
static double cursor_pos_y = 0;
static double delta_x = 0;
static double delta_y = 0;

ModelGL* modelGL = NULL;
int is_there_an_obj = 0;
int how_many_objs = 0;

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

Camera* camera = new Camera(eye1, lookAt1, up1, fov1, nearPlane1, farPlane1, bufferWidth1, bufferHeight1);

glm::vec3 pos1 = glm::vec3(0, 3, 2);
glm::vec3 color1 = glm::vec3(1, 1, 1);
bool isPtLt = true;

Light* light = new Light(pos1, color1, isPtLt);

float shiny = 10.0;

MeshShaderGL *shader;

MousePicker *mousePick;

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

		/*//case GLFW_KEY_SPACE:
			//modelGL->reset();
			//break;

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
			break;*/

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
}*/

/*// GLFW callback mouse button
static void mouse_button_callback(GLFWwindow* window, int button, int action,
	int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		double x, y;
		glfwGetCursorPos(window, &x, &y); //get screen coordinates from cursor
		mousePick->saveCoordX(x);
		mousePick->saveCoordY(y);
		mousePick->updateBufferW(framebufferWidth);
		mousePick->updateBufferH(framebufferHeight);
		mousePick->update();
		cout << "Current Ray: " << glm::to_string(mousePick->getCurrentRay()) << endl;
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