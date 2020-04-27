#include "Main.hpp"

/*///////////////////////////////////////////////////////////////////////////////
MAIN
///////////////////////////////////////////////////////////////////////////////*/

int main(int argc, char **argv) {

	// imgui-filebrowser - create file browser instances
	ImGui::FileBrowser fileDialog;
	fileDialog.SetTitle("Load .txt");
	fileDialog.SetTypeFilters({ ".txt" });

	ImGui::FileBrowser fileDialog1;
	fileDialog1.SetTitle("Load .obj");
	fileDialog1.SetTypeFilters({ ".obj" });

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
	//glfwSetCursorPosCallback(window, mouse_position_callback);

	// Set our mouse BUTTON callback function
	//glfwSetMouseButtonCallback(window, mouse_button_callback);

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
		std::cout << "ERROR: GLEW could not start: " << glewGetErrorString(err) << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	std::cout << "GLEW initialized; version ";
	std::cout << glewGetString(GLEW_VERSION) << endl;

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
	std::cout << "OpenGL context version: ";
	std::cout << glMajor << "." << glMinor << endl;
	std::cout << "Supported GLSL version is ";
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	// Load shader program
	shader = new MeshShaderGL("../core/Basic.vs", "../core/Basic.ps", true); //new object, dynamically
	MeshShaderGL* shaderBox = new MeshShaderGL("../core/Box.vs", "../core/Box.ps", true);
	//MeshShaderGL tmpModel; static new object

	// Create/Get mesh data	
	ModelData *modelData = NULL;
	ModelData *modelData1 = NULL;
	ModelData *tempModelData1 = NULL;
	ModelData *tempModelData2 = NULL;
	MeshShaderGL *tempModelDataGL1 = NULL;
	MeshShaderGL *tempModelDataGL2 = NULL;

	//Create/Get shader data
	ModelGL *modelGL = NULL;
	ModelGL *modelGL1 = NULL;
	ModelGL *temp1 = NULL;
	ModelGL *temp2 = NULL;

	//Collider, Actor
	Collide *collide1 = NULL;
	Collide *collide = NULL;
	Actor *anActor;
	Actor *anActor0;

	//Create a vector for Actor objs
	vector<Actor*> objFiles;
	vector<Actor*> objFiles1;

	// MousePicker
	MousePicker *mousePick = new MousePicker(camera);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
	
	// Main Dear ImGui window//
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Main window
		{
			ImGui::Begin("Main Menu");
			ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
			ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
			ImGui::Separator();
			ImGui::Text("Load");
			if (ImGui::Button("Load Scene")) {
				show_file_load_window = true;
			}
			if (ImGui::Button("Load Object")) {
				load_an_obj_file = true;
			}
			ImGui::Separator();
			if (ImGui::Button("Position/Color")) {
				edit_obj_color = true;
			}
			ImGui::Separator();
			ImGui::Text("Change Background Color");
			ImGui::ColorEdit3("Color", (float*)&clear_color); 
			ImGui::End();
		}

		// Edit color of object --------------------------->Still working on this.
		if (edit_obj_color)
		{
			ImGui::Begin("Position/Color", &edit_obj_color);
			static float rotation = 0.0;
			ImGui::SliderFloat("rotation", &rotation, 0, 2 * M_PI);
			static float translation[] = { 0.0, 0.0 };
			ImGui::SliderFloat2("position", translation, -1.0, 1.0);
			static float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			// pass parameters to the shader
			//shader.setUniform("rotation", rotation);
			//shader.setUniform("translation", translation[0], translation[1]);
			// color picker
			ImGui::ColorEdit3("color", color);
			// multiply obj's color with this color
			//shader.setUniform("color", color[0], color[1], color[2]);
			if (ImGui::Button("Close")) {
				edit_obj_color = false;
			}
			ImGui::End();
		}

		// Load .obj File
		if (load_an_obj_file)
		{
			ImGui::Begin("Load .obj", &load_an_obj_file);
			ImGui::Text("Open .obj File:");
			if (ImGui::Button("Open File"))
				fileDialog1.Open();
				fileDialog1.Display();
				if (fileDialog1.HasSelected())
				{
					std::string fileName = fileDialog1.GetSelected().string();
					ext = GetFilePathExtension(fileName);
					if (ext == "obj") {
						string modelFilename1 = fileName;
						modelData1 = loadModel(modelFilename1);
							if (!modelData1) {
								std::cout << "ERROR: Failed to set up model data." << endl;
								glfwTerminate();
								exit(EXIT_FAILURE);
							}
							// increment if object loaded
							how_many_objs1 = how_many_objs1 + 1;
							vector<Vertex>* vertices = modelData1->getMesh(0)->getVertices();
							for (int i = 0; i < vertices->size(); i++) {
								glm::vec3 pos = vertices->at(i).pos;
								//rescaling
								pos += 1.0;
								pos /= 2.0;
								vertices->at(i).color = glm::vec4(pos.x, pos.y, pos.z, 1.0);
							}
							collide1 = new Collide(modelData1);
							modelGL1 = new ModelGL(modelData1);
							anActor = new Actor(modelData1, modelGL1, collide1);
							//add Actor obj here
							objFiles1.push_back(anActor);
							shader->activate();
							shader->setShininess(shiny);
							shader->setMaterialChoice(0);
							glClearColor(0.0f, 0.0f, 0.7f, 1.0f);
							glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
							//load object at 0, 0, 0 coordinates
							modelGL1->translate(glm::vec3(0, 0, 0));
							modelData1 = NULL;
							modelGL1 = NULL;
					}
					fileDialog1.ClearSelected();
					load_an_obj_file = false;
				}
			if (ImGui::Button("Close")) {
				load_an_obj_file = false;
			}
			ImGui::End();
		}

		//.txt Scene Loader
		if (show_file_load_window)
		{
			ImGui::Begin(".txt Scene Loader", &show_file_load_window);
			ImGui::Text("Open .txt Scene File:");
			if (ImGui::Button("Open File"))
				fileDialog.Open();
				fileDialog.Display();
				if (fileDialog.HasSelected())
				{
					string fileName = fileDialog.GetSelected().string();
					ext = GetFilePathExtension(fileName);
					//if a txt file, parse each line, load models
					if (ext == "txt") {
						std::ifstream infile(fileName);
						std::string line;
						while (std::getline(infile, line))
						{
							std::istringstream iss(line);
							string newObj;
							string extenS = "../objs/";
							double x, y, z;
							//if no more lines or bad line
							if (!(iss >> newObj >> x >> y >> z)) {
								break; 
							}
							// process line (newObj, x, y, z)
							newObj = extenS + newObj;
							std::cout << "Loading model: " << newObj << " at: " << x << " " << y << " " << z << endl;
							modelData = loadModel(newObj);
							if (!modelData) {
								std::cout << "ERROR: Failed to set up model data." << endl;
								glfwTerminate();
								exit(EXIT_FAILURE);
							}
							// increment if object loaded
							how_many_objs = how_many_objs + 1;
							vector<Vertex>* vertices = modelData->getMesh(0)->getVertices();
							for (int i = 0; i < vertices->size(); i++) {
								glm::vec3 pos = vertices->at(i).pos;
								pos += 1.0;
								pos /= 2.0;
								vertices->at(i).color = glm::vec4(pos.x, pos.y, pos.z, 1.0);
							}
							modelGL = new ModelGL(modelData);
							collide = new Collide(modelData);
							anActor0 = new Actor(modelData, modelGL, collide);
							//add Actor obj here
							objFiles1.push_back(anActor0);
							shader->activate();
							shader->setShininess(shiny);
							shader->setMaterialChoice(0);
							glClearColor(0.0f, 0.0f, 0.7f, 1.0f);
							glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
							//load object at parsed-in x, y, z coordinates
							modelGL->translate(glm::vec3(x, y, z));
							modelData = NULL;
							modelGL = NULL;
						}
					}

					fileDialog.ClearSelected();
					show_file_load_window = false;
				}
				if (ImGui::Button("Close")) {
					show_file_load_window = false;
				}
			ImGui::End();
		}

		// Rendering!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		ImGui::Render();
		camera->updateBufferSize(framebufferWidth, framebufferHeight);
		shader->activate();
		shader->setViewAndProjection(camera);
		shader->setLight(light);
		shaderBox->activate();
		shaderBox->setViewAndProjection(camera);
		shaderBox->setLight(light);
		shader->activate();

		//cursor ray
		double x, y;
		glfwGetCursorPos(window, &x, &y); //get screen coordinates from cursor
		mousePick->saveCoordX(x);
		mousePick->saveCoordY(y);
		mousePick->updateBufferW(framebufferWidth);
		mousePick->updateBufferH(framebufferHeight);
		mousePick->update();
		cout << "Current Ray: " << glm::to_string(mousePick->getCurrentRay()) << endl;

		/*/////////////////////////////////////////////////////////////////////////////*/
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/*Draw the mesh using ModelGL's draw() method (passing in the shader)*/
		glLineWidth(10.0);
		if (how_many_objs1 >= 1) {
			for (int i = 0; i < objFiles1.size(); i++) {
				temp1 = objFiles1[i]->returnModelGL();
				shader->activate();
				temp1->draw(shader);
				tempModelData1 = objFiles1[i]->returnModelData();
				shaderBox->activate();
				shaderBox->draw_bounds(tempModelData1, temp1->getModel());
			}
		}
		/*if (how_many_objs == 1) {
			for (int i=0; i < objFiles.size(); i++) {
				temp2 = objFiles[i]->returnModelGL();
				temp2->draw(shader);
				tempModelData2 = objFiles[i]->returnModelData();
				shaderBox->draw_bounds(tempModelData2);
			}
		}*/
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	// Cleanup		
	delete shader;

	// Delete/Clear
	delete modelData1;
	delete modelData;
	delete modelGL1;
	delete modelGL;
	delete collide;
	delete collide1;
	//delete anActor;
	//delete anActor0;
	delete temp1;
	delete temp2;
	delete tempModelData1;
	delete tempModelData2;
	delete tempModelDataGL1;
	delete tempModelDataGL2;
	objFiles.clear();
	objFiles1.clear();

	// Teardown dearimgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
		
	// Destroy window and stop GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
