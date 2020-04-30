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
	bool show_file_save = false;
	bool load_an_obj_file = false;
	bool show_controls = false;
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

	// Create/Get mesh data	
	ModelData *modelData;
	ModelData *tempModelData;

	//Create/Get shader data
	ModelGL *modelGL;

	//Collider, Actor
	Collide *collide;
	Actor *anActor;

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
			if (ImGui::Button("Load Scene")) {
				show_file_load_window = true;
			}
			if (ImGui::Button("Save Scene")) {
				show_file_save = true;
			}
			ImGui::Separator();
			if (ImGui::Button("Load Object")) {
				load_an_obj_file = true;
			}
			ImGui::Separator();
			if (ImGui::Button("Controls")) {
				show_controls = true;
			}
			ImGui::Separator();
			ImGui::Text("Change Background Color");
			ImGui::ColorEdit3("Color", (float*)&clear_color); 
			ImGui::End();
		}

		// Show Controls //////////////////////////////////////////////////////////////////////////////////
		if (show_controls) {
			ImGui::Begin("Controls", &show_controls);
			ImGui::Text("Movement & Camera Controls");
			ImGui::Separator();
			ImGui::TextColored(ImVec4(0.5f, 0.0f, 1.0f, 1.0f), "SPACE"); ImGui::SameLine(); ImGui::TextWrapped("Reset position(s) to 0,0,0.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.5f, 0.0f, 1.0f, 1.0f), "U"); ImGui::SameLine(); ImGui::TextWrapped("Rotate by positive 5 degrees, z axis.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.5f, 0.0f, 1.0f, 1.0f), "O"); ImGui::SameLine(); ImGui::TextWrapped("Rotate by negative 5 degrees, z axis.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.5f, 0.0f, 1.0f, 1.0f), "E"); ImGui::SameLine(); ImGui::TextWrapped("Rotate by positive 5 degrees, x axis.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.5f, 0.0f, 1.0f, 1.0f), "R"); ImGui::SameLine(); ImGui::TextWrapped("Rotate by negative 5 degrees, x axis.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.5f, 0.0f, 1.0f, 1.0f), "T"); ImGui::SameLine(); ImGui::TextWrapped("Rotate by positive 5 degrees, y axis.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.5f, 0.0f, 1.0f, 1.0f), "Y"); ImGui::SameLine(); ImGui::TextWrapped("Rotate by negative 5 degrees, y axis.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.5f, 0.0f, 1.0f, 1.0f), "I"); ImGui::SameLine(); ImGui::TextWrapped("Move by positive 0.1 in Y.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.5f, 0.0f, 1.0f, 1.0f), "K"); ImGui::SameLine(); ImGui::TextWrapped("Move by negative 0.1 in Y.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.5f, 0.0f, 1.0f, 1.0f), "L"); ImGui::SameLine(); ImGui::TextWrapped("Move by positive 0.1 in X.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.5f, 0.0f, 1.0f, 1.0f), "J"); ImGui::SameLine(); ImGui::TextWrapped("Move by negative 0.1 in X.");
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::TextColored(ImVec4(2.0f, 0.5f, 1.0f, 1.0f), "W"); ImGui::SameLine(); ImGui::TextWrapped("Move camera forward.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(2.0f, 0.5f, 1.0f, 1.0f), "S"); ImGui::SameLine(); ImGui::TextWrapped("Move camera back.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(2.0f, 0.5f, 1.0f, 1.0f), "D"); ImGui::SameLine(); ImGui::TextWrapped("Move camera right.");
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(2.0f, 0.5f, 1.0f, 1.0f), "A"); ImGui::SameLine(); ImGui::TextWrapped("Move camera left.");
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::TextColored(ImVec4(0.0f, 0.5f, 1.0f, 1.0f), "C"); ImGui::SameLine(); ImGui::TextWrapped("Turn point light on/off.");
			ImGui::Spacing();
			//ImGui::TextColored(ImVec4(0.0f, 0.5f, 1.0f, 1.0f), "0 or 1"); ImGui::SameLine(); ImGui::TextWrapped("Choose material.");
			//ImGui::Spacing();
			//ImGui::TextColored(ImVec4(0.0f, 0.5f, 1.0f, 1.0f), "V"); ImGui::SameLine(); ImGui::TextWrapped("Increase shine.");
			//ImGui::Spacing();
			//ImGui::TextColored(ImVec4(0.0f, 0.5f, 1.0f, 1.0f), "B"); ImGui::SameLine(); ImGui::TextWrapped("Decrease shine.");
			//ImGui::Spacing();
			ImGui::TextColored(ImVec4(0.0f, 0.5f, 1.0f, 1.0f), "N or M"); ImGui::SameLine(); ImGui::TextWrapped("Set object color to white or blue, respectively.");
			ImGui::Spacing();

			if (ImGui::Button("Close")) {
				show_controls = false;
			}
			ImGui::End();
		}

		// .txt Save a Scene File //////////////////////////////////////////////////////////////////////////////////
		if (show_file_save)
		{
			ImGui::Begin(".txt Scene Saver", &show_file_save);
			ImGui::Text("Save .txt Scene File");
			ImGui::Separator();
			ImGui::Text("Enter file name without extension...");
			ImGui::InputText("no blank", aFileN, 64, ImGuiInputTextFlags_CharsNoBlank);
			if (ImGui::Button("Save Scene")) {
				fileNameNew += aFileN;
				string newPath = "../" + fileNameNew + ".txt";
				std::ofstream fs;
				fs.open(newPath);

				if (!fs)
				{
					std::cerr << "Cannot open the output file." << std::endl;
					return 1;
				}
				if (fs.is_open()) {
					if (how_many_objs >= 1) {
						for (int i = 0; i < objFiles.size(); i++) {
							ModelGL* tempModelGLOut = objFiles[i]->returnModelGL();
							ModelData* tempModelDataOut = objFiles[i]->returnModelData();

							aFName = tempModelDataOut->getFileName();
							glm::vec3 coordsOut = tempModelGLOut->getCenterofModel();

							fs << aFName << " " << coordsOut.x << " " << coordsOut.y << " " << coordsOut.z << std::endl;
						}
					}
				}
				fs.close(); 
			}
			if (ImGui::Button("Close")) {
				show_file_save = false;
				fileNameNew = "";
			}
			ImGui::End();
		}

		// Load .obj File //////////////////////////////////////////////////////////////////////////////////
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
						string modelFilename = fileName;
						modelData = loadModel(modelFilename);

						//save name of model loaded
						std::string nameForAdding = getFileName(modelFilename);
						modelData->addFileName(nameForAdding);

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
								//rescaling
								pos += 1.0;
								pos /= 2.0;
								vertices->at(i).color = glm::vec4(pos.x, pos.y, pos.z, 1.0);
							}
							collide = new Collide(modelData);
							modelGL = new ModelGL(modelData);
							anActor = new Actor(modelData, modelGL, collide);
							//add Actor obj here
							objFiles.push_back(anActor);
							/*shader->activate();
							shader->setShininess(shiny);
							shader->setMaterialChoice(0);
							glClearColor(0.0f, 0.0f, 0.7f, 1.0f);
							glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
							//load object at 0, 0, 0 coordinates
							modelGL->translate(glm::vec3(0, 0, 0));
							modelData = NULL;
							modelGL = NULL;
					}
					fileDialog1.ClearSelected();
					load_an_obj_file = false;
				}
			if (ImGui::Button("Close")) {
				load_an_obj_file = false;
			}
			ImGui::End();
		}

		// .txt Scene Loader //////////////////////////////////////////////////////////////////////////////////
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
							string newNewObj = newObj; //save object name
							newObj = extenS + newObj;
							modelData = loadModel(newObj);
							modelData->addFileName(newNewObj); //save name of object loaded

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
							anActor = new Actor(modelData, modelGL, collide);
							//add Actor obj here
							objFiles.push_back(anActor);
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
		//cout << "Current Ray: " << glm::to_string(mousePick->getCurrentRay()) << endl;

		/*/////////////////////////////////////////////////////////////////////////////*/
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/*Draw the mesh using ModelGL's draw() method (passing in the shader)*/
		glLineWidth(10.0);
		if (how_many_objs >= 1) {
			for (int i = 0; i < objFiles.size(); i++) {
				temp = objFiles[i]->returnModelGL();
				shader->activate();
				temp->draw(shader);
				tempModelData = objFiles[i]->returnModelData();
				shaderBox->activate();
				shaderBox->draw_bounds(tempModelData, temp->getModel());
			}
		}
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	// Cleanup		
	delete shader;

	// Delete/Clear
	delete temp;
	objFiles.clear();

	// Teardown dearimgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
		
	// Destroy window and stop GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
