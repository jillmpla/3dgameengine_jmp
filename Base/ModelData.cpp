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