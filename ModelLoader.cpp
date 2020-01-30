/*
//
// NOTE: The code that follows is heavily inspired by code from https://learnopengl.com/.
//
*/

#define _USE_MATH_DEFINES
#include "ModelLoader.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;

void getAllTextureFilename(
	string directory,
	const aiMaterial *mat,
	aiTextureType type,
	vector<int> &textureList,
	unordered_map<string, int> allTextureIndices,
	ModelData *model) {

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		// Get filename
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string filename = std::string(str.C_Str());

		// Check if we have this texture
		auto got = allTextureIndices.find(filename);

		if (got == allTextureIndices.end()) {
			// We don't have it, so load it up!
			TextureData *texture = new TextureData(directory + "/" + filename);
			int texIndex = model->addTexture(texture);
			textureList.push_back(texIndex);
			allTextureIndices[filename] = texIndex;
		}
		else {
			// Already have it
			textureList.push_back(got->second);
		}
	}
}

void addDefaultTextures(ModelData *model, int &diffuseTex, int &specTex, int &normalTex) {
	diffuseTex = model->addTexture(new TextureData(1, 1, glm::vec4(1, 1, 1, 1)));
	specTex = model->addTexture(new TextureData(1, 1, glm::vec4(0.5, 0.5, 0.5, 1)));
	normalTex = model->addTexture(new TextureData(1, 1, glm::vec4(0.5, 0.5, 1, 0)));
}

int addDefaultMaterial(ModelData *model, int diffuseTex, int specTex, int normalTex) {
	MaterialData *defaultMat = new MaterialData();
	defaultMat->setName("DEFAULT_MAT");
	defaultMat->setKd(glm::vec3(1, 1, 1));
	defaultMat->setKs(glm::vec3(1, 1, 1));
	defaultMat->setShininess(32.0);
	defaultMat->addDiffuseTex(diffuseTex);
	defaultMat->addSpecularTex(specTex);
	defaultMat->addNormalsTex(normalTex);
	return model->addMaterial(defaultMat);
}

void loadAllMaterials(string directory, const aiScene *scene, ModelData *model) {

	// Create a mapping of name to texture index
	unordered_map<string, int> textureIndices;

	// Add default textures
	const string DEFAULT_DIFFUSE = "DEFAULT_DIFFUSE";
	const string DEFAULT_SPECULAR = "DEFAULT_SPECULAR";
	const string DEFAULT_NORMAL = "DEFAULT_NORMAL";

	int diffuseTex = 0;
	int specTex = 0;
	int normalTex = 0;
	addDefaultTextures(model, diffuseTex, specTex, normalTex);

	textureIndices[DEFAULT_DIFFUSE] = diffuseTex;
	textureIndices[DEFAULT_SPECULAR] = specTex;
	textureIndices[DEFAULT_NORMAL] = normalTex;

	// Loop through and load all materials
	for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
		// Get Assimp material
		aiMaterial *mat = scene->mMaterials[i];

		// Create our material
		MaterialData *ourMat = new MaterialData();

		// Get name
		aiString name;
		mat->Get(AI_MATKEY_NAME, name);
		ourMat->setName(name.C_Str());

		// Get diffuse, specular, and shininess
		aiColor3D kd(1.f, 1.f, 1.f);
		mat->Get(AI_MATKEY_COLOR_DIFFUSE, kd);
		ourMat->setKd(glm::vec3(kd.r, kd.g, kd.b));

		aiColor3D ks(1.f, 1.f, 1.f);
		mat->Get(AI_MATKEY_COLOR_SPECULAR, ks);
		ourMat->setKs(glm::vec3(ks.r, ks.g, ks.b));

		float shiny = 32.0;
		mat->Get(AI_MATKEY_SHININESS, shiny);
		ourMat->setShininess(shiny);

		// Get all texture names
		vector<int> diffuse;
		getAllTextureFilename(directory, mat, aiTextureType_DIFFUSE, diffuse,
			textureIndices, model);
		if (diffuse.size() > 0) {
			for (auto t : diffuse) {
				ourMat->addDiffuseTex(t);
			}
		}
		else {
			ourMat->addDiffuseTex(textureIndices[DEFAULT_DIFFUSE]);
		}

		vector<int> specular;
		getAllTextureFilename(directory, mat, aiTextureType_SPECULAR, specular,
			textureIndices, model);
		if (specular.size() > 0) {
			for (auto t : specular) {
				ourMat->addSpecularTex(t);
			}
		}
		else {
			ourMat->addSpecularTex(textureIndices[DEFAULT_SPECULAR]);
		}

		vector<int> normals;
		// NOTE: Using aiTextureType_HEIGHT because of weird situation 
		// with the Assimp library and OBJ files.
		// Really should be using aiTextureType_NORMALS, but that doesn't play nice
		// with OBJ's map_Bump
		getAllTextureFilename(directory, mat, aiTextureType_HEIGHT, normals,
			textureIndices, model);
		if (normals.size() > 0) {
			for (auto t : normals) {
				ourMat->addNormalsTex(t);
			}
		}
		else {
			ourMat->addNormalsTex(textureIndices[DEFAULT_NORMAL]);
		}

		// Add material
		model->addMaterial(ourMat);
	}

	// If no materials, add a default one:
	if (model->getMaterialCnt() == 0) {
		addDefaultMaterial(model,
			textureIndices[DEFAULT_DIFFUSE],
			textureIndices[DEFAULT_SPECULAR],
			textureIndices[DEFAULT_NORMAL]);
	}
}

void processMesh(aiMesh *mesh, const aiScene *scene, ModelData *model) {
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	// Load vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		vertex.pos = position;

		vertex.color = glm::vec4(1.0, 1.0, 1.0, 1.0);

		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		vertex.normal = normal;

		if (mesh->mTextureCoords[0]) {
			glm::vec2 uv;
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			vertex.texcoords = uv;
		}
		else {
			// Just use a planar projection
			float u = position.x;
			float v = position.y;
			vertex.texcoords = glm::vec2(u, v);
		}

		vertices.push_back(vertex);
	}

	// Load indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// Get material index
	int matIndex = mesh->mMaterialIndex;

	// Create the MeshData and add it
	MeshData *meshData = new MeshData(vertices, indices);
	model->addMesh(meshData, matIndex);
}

void processNode(aiNode *node, const aiScene *scene, ModelData *model) {
	// Go through all meshes...
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		// Grab the aiMesh
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		// Process the aiMesh and add it to our list of meshes
		processMesh(mesh, scene, model);
	}

	// Recursively do this to children...
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene, model);
	}
}

ModelData* loadModel(string path) {

	// Load model data
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices); // | aiProcess_FixInfacingNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		cout << "Error: " << importer.GetErrorString() << endl;
		return NULL;
	}

	// Get directory
	string directory = path.substr(0, path.find_last_of('/'));

	// Create the ModelData object
	ModelData *model = new ModelData();

	// Load all materials
	loadAllMaterials(directory, scene, model);

	// Process each node
	processNode(scene->mRootNode, scene, model);

	return model;
}

