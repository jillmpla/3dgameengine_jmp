#include "MeshShaderGL.hpp"

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

//draw bounding box around obj/////////////////////////////////////////////////////////////////////
void MeshShaderGL::draw_bounds(ModelData *tempMD) {
    GLfloat
        min_x, max_x,
        min_y, max_y,
        min_z, max_z;

    vector<Vertex>* vertices = tempMD->getMesh(0)->getVertices();
    glm::vec3 pos = vertices->at(0).pos;
    min_x = max_x = pos.x;
    min_y = max_y = pos.y;
    min_z = max_z = pos.z;
    for (int i = 0; i < vertices->size(); i++) {
        pos = vertices->at(i).pos;
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
    GLuint vbo_vertices;
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /////////////////////////////////////////

    GLushort elements[] = {
      0, 1, 2, 3,
      4, 5, 6, 7,
      0, 4, 1, 5, 2, 6, 3, 7
    };
    GLuint ibo_elements;
    glGenBuffers(1, &ibo_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //apply object transformation
    transformLoc = glGetUniformLocation(programID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    GLuint posLoc = glGetAttribLocation(programID, "position");
    glEnableVertexAttribArray(posLoc);
    glVertexAttribPointer(
        posLoc,  
        4,                  
        GL_FLOAT,
        GL_FALSE,
        0,
        0
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(posLoc);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &ibo_elements);
}