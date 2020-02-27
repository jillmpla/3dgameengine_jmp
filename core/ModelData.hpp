#ifndef MODEL_DATA_H
#define MODEL_DATA_H

#include "MeshData.hpp"
#include "TextureData.hpp"
#include "MaterialData.hpp"
using namespace std;

class ModelData {
private:
	vector<MeshData*> meshes;
	vector<MaterialData*> materials;
	vector<TextureData*> textures;
	vector<int> matIndices;			// Index of material for given Mesh (# of elements = # of meshes)
	
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
};

#endif
