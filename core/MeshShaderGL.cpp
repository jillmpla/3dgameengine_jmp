#include "MeshShaderGL.hpp"
#include <iostream>
using namespace std;
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/transform.hpp"

MeshShaderGL::MeshShaderGL(std::string vertex, std::string fragment, bool isFilepath) : ShaderGL(vertex, fragment, isFilepath) 
{
	modelMatLoc = glGetUniformLocation(programID, "modelMat");
	viewMat = glGetUniformLocation(programID, "viewMat");
	projMat = glGetUniformLocation(programID, "projMat");
	normal = glGetUniformLocation(programID, "normMat");
	lightPosition = glGetUniformLocation(programID, "light.pos");
	lightColor = glGetUniformLocation(programID, "light.color");
	shininess = glGetUniformLocation(programID, "shiny");
	material = glGetUniformLocation(programID, "matChoice");

    createBoundingBox();
}

MeshShaderGL::~MeshShaderGL() {
    cleanupBoundingBox();
}

void MeshShaderGL::setModelTransform(glm::mat4 &modelMat) {
	this->modelMat = modelMat;
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(this->modelMat));

	//compute normal matrix and pass to shader
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(viewMatrix*modelMat)));
	glUniformMatrix3fv(normal, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

void MeshShaderGL::setViewAndProjection(Camera *camera) {
	viewMatrix = camera->getViewMatrix();
	glUniformMatrix4fv(viewMat, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projMat, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));

	//compute normal matrix and pass to shader
	glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(viewMatrix*modelMat)));
	glUniformMatrix3fv(normal, 1, GL_FALSE, glm::value_ptr(normalMatrix));
}

void MeshShaderGL::setLight(Light *light) {
	glm::vec4 viewLit = viewMatrix*light->getPos();
	glUniform4fv(lightPosition, 1, glm::value_ptr(viewLit));
	glUniform4fv(lightColor, 1, glm::value_ptr(light->getColor()));
}

void MeshShaderGL::setShininess(float s) {
	glUniform1f(shininess, s);                      //for floats
}

void MeshShaderGL::setMaterialChoice(int choice) {
	glUniform1i(material, choice);                 //for ints
}

void MeshShaderGL::createBoundingBox() {

    // Cube 1x1x1 - center on origin

    GLfloat vertices1[] = {
      -0.5, -0.5, -0.5, 1.0,
       0.5, -0.5, -0.5, 1.0,
       0.5,  0.5, -0.5, 1.0,
      -0.5,  0.5, -0.5, 1.0,
      -0.5, -0.5,  0.5, 1.0,
       0.5, -0.5,  0.5, 1.0,
       0.5,  0.5,  0.5, 1.0,
      -0.5,  0.5,  0.5, 1.0,
    };
    
    //Create and bind the VAO
    glGenVertexArrays(1, &bound_VAO);
    glBindVertexArray(bound_VAO);

    glGenBuffers(1, &bound_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, bound_VBO);
<<<<<<< HEAD
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*48, vertices1, GL_STATIC_DRAW); //32
=======
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*32, vertices1, GL_STATIC_DRAW);
>>>>>>> master
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, 0);

    /////////////////////////////////////////

    GLuint elements[] = {
      0, 1, 2, 3,
      4, 5, 6, 7,
      0, 4, 1, 5, 2, 6, 3, 7
    };
    
    glGenBuffers(1, &bound_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bound_EBO);
<<<<<<< HEAD
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*16, elements, GL_STATIC_DRAW); //16
=======
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*16, elements, GL_STATIC_DRAW); 
>>>>>>> master
}

void MeshShaderGL::cleanupBoundingBox() {

    glDeleteBuffers(1, &bound_VBO);
    glDeleteBuffers(1, &bound_EBO);
    glBindVertexArray(0);
	glDeleteVertexArrays(1, &bound_VAO);
}

<<<<<<< HEAD
void MeshShaderGL::draw_bounds(ModelData *tempMD, glm::mat4 mainModelMatrix) {
    // size and position of bounding box
=======
//draw bounding box around obj/////////////////////////////////////////////////////////////////////
void MeshShaderGL::draw_bounds(ModelData *tempMD, glm::mat4 mainModelMatrix) {
    // THIS IS THE PART THAT FIGURES OUT WHAT THE SIZE AND POSITION OF THE BOUNDING BOX IS
>>>>>>> master
    GLfloat
        min_x, max_x,
        min_y, max_y,
        min_z, max_z;

    vector<Vertex>* vertices = tempMD->getMesh(0)->getVertices();
    glm::vec3 pos = vertices->at(0).pos;
    pos = glm::vec3(mainModelMatrix * glm::vec4(pos, 1.0));
    min_x = max_x = pos.x;
    min_y = max_y = pos.y;
    min_z = max_z = pos.z;
    for (int i = 0; i < vertices->size(); i++) {
        pos = vertices->at(i).pos;
        pos = glm::vec3(mainModelMatrix * glm::vec4(pos, 1.0));
        if (pos.x < min_x) min_x = pos.x;
        if (pos.x > max_x) max_x = pos.x;
        if (pos.y < min_y) min_y = pos.y;
        if (pos.y > max_y) max_y = pos.y;
        if (pos.z < min_z) min_z = pos.z;
        if (pos.z > max_z) max_z = pos.z;
    }
    glm::vec3 size = glm::vec3(max_x - min_x, max_y - min_y, max_z - min_z);
    glm::vec3 center = glm::vec3((min_x + max_x) / 2.0, (min_y + max_y) / 2.0, (min_z + max_z) / 2.0);
    glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

    //apply object transformation    
    glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(bound_VAO);
<<<<<<< HEAD
	glDrawElements(GL_LINE_LOOP, 16, GL_UNSIGNED_INT, (void*)0); //16
=======
	glDrawElements(GL_LINES, 16, GL_UNSIGNED_INT, (void*)0);
>>>>>>> master
	glBindVertexArray(0);
}