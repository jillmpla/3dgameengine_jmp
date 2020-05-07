#include "ModelData.hpp"
#include "MeshShaderGL.hpp"

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

void ModelData::addFileName(string m) {
	aName = m;
}

string ModelData::getFileName() {
	return aName;
}

glm::mat4 ModelData::sizeposBB(ModelData* temp, glm::mat4 mainModelMatrix) {
	// size and position of bounding box
	GLfloat
		min_x, max_x,
		min_y, max_y,
		min_z, max_z;

	vector<Vertex>* vertices = temp->getMesh(0)->getVertices();
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
	transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

	return transform;
}

glm::mat4 ModelData::returnTransform() {
	return transform;
}
