#ifndef MODEL_DATA_H
#define MODEL_DATA_H

#include "MeshData.hpp"
#include "TextureData.hpp"
#include "MaterialData.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class ModelData {
private:
	vector<MeshData*> meshes;
	vector<MaterialData*> materials;
	vector<TextureData*> textures;
	vector<int> matIndices;
	string aName;

	glm::mat4 transform; //bounding box
	
public:
	ModelData();
	~ModelData();

	void reset();

	int addTexture(TextureData *t);
	int addMaterial(MaterialData *m);
	int addMesh(MeshData *m, int matIndex);

	int getTextureCnt();
	int getMaterialCnt();
	int getMeshCnt();

	TextureData* getTexture(int index);
	MaterialData* getMaterial(int index);
	MeshData* getMesh(int index);
	int getMaterialIndex(int meshIndex);

	void ModelData::addFileName(string m);
	string ModelData::getFileName();

	glm::mat4 ModelData::sizeposBB(ModelData* temp, glm::mat4 mainModelMatrix);
	glm::mat4 ModelData::returnTransform();
};

#endif
