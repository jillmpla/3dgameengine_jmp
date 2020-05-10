/*/////////////////////////////////////////////////////////////////////////////
NOTE:
Add a copy of [irrKlang.dll] and [ikpMP3.dll] from libs/irrKlang to the build 
folder for Visual Studio to run/debug with sound/music feature.
/////////////////////////////////////////////////////////////////////////////*/

/*C++*/
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
#include "physics/Collide.hpp"
#include "physics/Actor.hpp"
#include "sound/Sound.hpp"
/*Dear ImGui*/
#include "libs/dearimgui/imgui.h"
#include "libs/dearimgui/imgui_impl_glfw.h"
#include "libs/dearimgui/imgui_impl_opengl3.h"
#include "libs/dearimgui/imconfig.h"
#include "libs/dearimgui/imgui_internal.h"
#include "libs/dearimgui/imstb_rectpack.h"
#include "libs/dearimgui/imstb_textedit.h"
#include "libs/dearimgui/imstb_truetype.h"
/*/////////////////////////////////////////////////////////////////////////////*/
#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "libs/stb_image_write.h"
/*imfilebrowser*/
#include "libs/imfilebrowser.h"
/*/////////////////////////////////////////////////////////////////////////////*/

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

int how_many_objs = 0;
int num_sound_objs = 0;

const float TRANSLATION_INC = 0.1;

const float CAMERA_WALK_SPEED = .1f;
const float CAMERA_ROTATE_SPEED = 30.0f;

glm::vec3 up1 = glm::vec3(0, 1, 0);
glm::vec3 eye1 = glm::vec3(0, 0, 1);
glm::vec3 lookAt1 = glm::vec3(0, 0, 0);

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
int materialCh = 0;

MeshShaderGL *shader;
MeshShaderGL* shaderBox;

ModelData* modelData;
ModelData* tempModelData;

ModelGL* modelGL;

Collide* collide;
Actor* anActor;

MousePicker* mousePick = new MousePicker(camera);

ModelGL *temp;
ModelGL *tempMove;

vector<Actor*> objFiles;
vector<Sound*> soundFiles;

bool rightMouseDown = false;

glm::mat4 tempModelMatrix;
glm::mat4 tempTransform;

string fileNameNew;
string fileNameNew1;
char aFileN[64] = "";
char aFileN1[64] = "";
string aFName;
int locationOfSpecificSound;

// State
bool show_file_load_window = false;
bool show_file_save = false;
bool load_an_obj_file = false;
bool show_controls = false;
bool add_sound = false;
bool stop_all_sounds = false;
bool volumeDown = false;
bool volumeUp = false;
bool pauseSound = false;
bool adjust_specific_sound = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

std::string ext;

// Finds file name from file path
string getFileName(const string& s) {

	char sep = '/';

#ifdef _WIN32
	sep = '\\';
#endif

	size_t i = s.rfind(sep, s.length());
	if (i != string::npos) {
		return(s.substr(i + 1, s.length() - i));
	}

	return("");
}

static std::string GetFilePathExtension(const std::string& FileName) {
	if (FileName.find_last_of(".") != std::string::npos)
		return FileName.substr(FileName.find_last_of(".") + 1);
	return "";
}

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
			if (how_many_objs >= 1) {
				for (int i = 0; i < objFiles.size(); i++) {
					tempMove = objFiles[i]->returnModelGL();
					tempMove->reset();
				}
			}
			break;

		case GLFW_KEY_U:
			if (how_many_objs >= 1) {
				for (int i = 0; i < objFiles.size(); i++) {
					tempMove = objFiles[i]->returnModelGL();
					tempMove->rotate(5.0f, glm::vec3(0, 0, 1)); //rotate modelGL by positive 5 degrees, z axis
				}
			}
			break;

		case GLFW_KEY_O:
			if (how_many_objs >= 1) {
				for (int i = 0; i < objFiles.size(); i++) {
					tempMove = objFiles[i]->returnModelGL();
					tempMove->rotate(-5.0f, glm::vec3(0, 0, 1)); //rotate modelGL by negative 5 degrees, z axis
				}
			}
			break;

		case GLFW_KEY_E:
			if (how_many_objs >= 1) {
				for (int i = 0; i < objFiles.size(); i++) {
					tempMove = objFiles[i]->returnModelGL();
					tempMove->rotate(5.0f, glm::vec3(1, 0, 0)); //rotate modelGL by positive 5 degrees, x axis
				}
			}
			break;

		case GLFW_KEY_R:
			if (how_many_objs >= 1) {
				for (int i = 0; i < objFiles.size(); i++) {
					tempMove = objFiles[i]->returnModelGL();
					tempMove->rotate(-5.0f, glm::vec3(1, 0, 0)); //rotate modelGL by negative 5 degrees, x axis
				}
			}
			break;

		case GLFW_KEY_T:
			if (how_many_objs >= 1) {
				for (int i = 0; i < objFiles.size(); i++) {
					tempMove = objFiles[i]->returnModelGL();
					tempMove->rotate(5.0f, glm::vec3(0, 1, 0)); //rotate modelGL by positive 5 degrees, y axis
				}
			}
			break;

		case GLFW_KEY_Y:
			if (how_many_objs >= 1) {
				for (int i = 0; i < objFiles.size(); i++) {
					tempMove = objFiles[i]->returnModelGL();
					tempMove->rotate(-5.0f, glm::vec3(0, 1, 0)); //rotate modelGL by negative 5 degrees, y axis
				}
			}
			break;

		case GLFW_KEY_I:
			if (how_many_objs >= 1) {
				for (int i = 0; i < objFiles.size(); i++) {
					tempMove = objFiles[i]->returnModelGL();
					tempMove->translate(glm::vec3(0, TRANSLATION_INC, 0)); //translate modelGL by positive TRANSLATION_INC in Y
				}
			}
			break;

		case GLFW_KEY_K:
			if (how_many_objs >= 1) {
				for (int i = 0; i < objFiles.size(); i++) {
					tempMove = objFiles[i]->returnModelGL();
					tempMove->translate(glm::vec3(0, -TRANSLATION_INC, 0)); //translate modelGL by negative TRANSLATION_INC in Y
				}
			}
			break;

		case GLFW_KEY_L:
			if (how_many_objs >= 1) {
				for (int i = 0; i < objFiles.size(); i++) {
					tempMove = objFiles[i]->returnModelGL();
					tempMove->translate(glm::vec3(TRANSLATION_INC, 0, 0)); //translate modelGL by positive TRANSLATION_INC in X
				}
			}
			break;

		case GLFW_KEY_J:
			if (how_many_objs >= 1) {
				for (int i = 0; i < objFiles.size(); i++) {
					tempMove = objFiles[i]->returnModelGL();
					tempMove->translate(glm::vec3(-TRANSLATION_INC, 0, 0)); //translate modelGL by negative TRANSLATION_INC in X
				}
			}
			break;

		// Camera keys
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

		// Extra
		case GLFW_KEY_C:
			if (light->getIsPointLight()) {
				light->setIsPointLight(false);
			}
			else {
				light->setIsPointLight(true);
			}
			break;

		case GLFW_KEY_0:
			materialCh = 0;
			shader->setMaterialChoice(materialCh);
			break;
		
		//////////////////////////original look//////////////////////////
		case GLFW_KEY_1:
			materialCh = 1;
			shader->setMaterialChoice(materialCh);
			break;
		/////////////////////////////////////////////////////////////////

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

		//////////////////////////original look//////////////////////////
		case GLFW_KEY_M:
			glm::vec3 white1 = glm::vec3(1, 1, 1);
			light->setColor(white1);
			break;
		/////////////////////////////////////////////////////////////////

		default:
			break;
		}
	}
}

//Code modified from:////////////////////////////////////////////////////////////////////////////////////////////////////////
//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool intersect(glm::vec3 rayOrg, glm::vec3 rayDir, glm::vec3 centerTemp, glm::vec3 sizeTemp)
{
	glm::vec3 min;
	glm::vec3 max;
	min = centerTemp - sizeTemp/2.0f;
	max = centerTemp + sizeTemp/2.0f;

	cout << "min" << glm::to_string(min) << "max" << glm::to_string(max) << endl;

	float tmin = (min.x - rayOrg.x) / rayDir.x;
	float tmax = (max.x - rayOrg.x) / rayDir.x;

	if (tmin > tmax) swap(tmin, tmax);

	float tymin = (min.y - rayOrg.y) / rayDir.y;
	float tymax = (max.y - rayOrg.y) / rayDir.y;

	if (tymin > tymax) swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (min.z - rayOrg.z) / rayDir.z;
	float tzmax = (max.z - rayOrg.z) / rayDir.z;

	if (tzmin > tzmax) swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return true;
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
// GLFW callback mouse position ------------> Last functionality still a work in progress. <------------
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (rightMouseDown == true) {
		double v, w;
		glfwGetCursorPos(window, &v, &w); //get screen coordinates from cursor
		mousePick->saveCoordX(v);
		mousePick->saveCoordY(w);
		mousePick->updateBufferW(framebufferWidth);
		mousePick->updateBufferH(framebufferHeight);
		mousePick->update();
		glm::vec3 currentRayAtTheMoment = mousePick->getCurrentRay(); //mouse ray
		currentRayAtTheMoment = glm::normalize(currentRayAtTheMoment);

		glm::vec3 currentEye = camera->getEye();
		if (how_many_objs >= 1) {
			for (int i = 0; i < objFiles.size(); i++) {
				ModelData* tempModelDataM = objFiles[i]->returnModelData(); //get modeldata
				glm::vec3 centerTemp = tempModelDataM->getCenterBB();
				glm::vec3 sizeTemp = tempModelDataM->getSizeBB();
				bool hitObject = intersect(currentEye, currentRayAtTheMoment, centerTemp, sizeTemp);
			}
		}
	}
}

// GLFW callback mouse button
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		rightMouseDown = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		rightMouseDown = false;
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