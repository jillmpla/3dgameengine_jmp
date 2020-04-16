#include "ModelData.hpp"

ModelData::ModelData() {}

ModelData::~ModelData() {
	reset();
}

void ModelData::reset() {
	for (int i = 0; i < meshes.size(); i++) {
		delete meshes.at(i);
	}
	meshes.clear();

	for (int i = 0; i < materials.size(); i++) {
		delete materials.at(i);
	}
	materials.clear();

	for (int i = 0; i < textures.size(); i++) {
		delete textures.at(i);
	}
	textures.clear();

	matIndices.clear();	
}

int ModelData::addTexture(TextureData *t) {
	textures.push_back(t);
	return ((int)textures.size() - 1);
}

int ModelData::addMaterial(MaterialData *m) {
	materials.push_back(m);
	return ((int)materials.size() - 1);
}

int ModelData::addMesh(MeshData *m, int matIndex) {
	meshes.push_back(m);
	matIndices.push_back(matIndex);
	return ((int)meshes.size() - 1);
}

int ModelData::getTextureCnt() {
	return (int)textures.size();
}

int ModelData::getMaterialCnt() {
	return (int)materials.size();
}

int ModelData::getMeshCnt() {
	return (int)meshes.size();
}

TextureData* ModelData::getTexture(int index) {
	if (index >= 0 && index < textures.size()) {
		return textures.at(index);
	}
	else {
		return NULL;
	}
}

MaterialData* ModelData::getMaterial(int index) {
	if (index >= 0 && index < materials.size()) {
		return materials.at(index);
	}
	else {
		return 0;
	}
}

MeshData* ModelData::getMesh(int index) {
	if (index >= 0 && index < meshes.size()) {
		return meshes.at(index);
	}
	else {
		return 0;
	}
}

int ModelData::getMaterialIndex(int meshIndex) {
	if (meshIndex >= 0 && meshIndex < matIndices.size()) {
		return matIndices.at(meshIndex);
	}
	else {
		return 0;
	}
}

//draw bounding box around obj
void ModelData::draw_bounds(ModelData* tempMD) {
    GLfloat
        min_x, max_x,
        min_y, max_y,
        min_z, max_z;

    vector<Vertex>* vertices = tempMD->getMesh(0)->getVertices();

    for (int i = 0; i < vertices->size(); i++) {
        glm::vec3 pos = vertices->at(i).pos;
        min_x = max_x = pos.x;
        min_y = max_y = pos.y;
        min_z = max_z = pos.z;
        if (pos.x < min_x) min_x = pos.x;
        if (pos.x > max_x) max_x = pos.x;
        if (pos.y < min_y) min_y = pos.y;
        if (pos.y > max_y) max_y = pos.y;
        if (pos.z < min_z) min_z = pos.z;
        if (pos.z > max_z) max_z = pos.z;
        //glm::vec4(pos.x, pos.y, pos.z, 1.0);
    }
    glm::vec3 size = glm::vec3(max_x - min_x, max_y - min_y, max_z - min_z);
    glm::vec3 center = glm::vec3((min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2);
    glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

    // Cube 1x1x1, centered on origin
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

    /* Apply object's transformation matrix */
    /*glm::mat4 m = setModelTransform * transform;
    glUniformMatrix4fv(uniform_m, 1, GL_FALSE, glm::value_ptr(m));

    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glEnableVertexAttribArray(attribute_v_coord);
    glVertexAttribPointer(
        attribute_v_coord,  // attribute
        4,                  // number of elements per vertex, here (x,y,z,w)
        GL_FLOAT,           
        GL_FALSE,           
        0,                  
        0                   
    );*/

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
    glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //glDisableVertexAttribArray(attribute_v_coord);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &ibo_elements);
}