#ifndef MATERIAL_DATA_H
#define MATERIAL_DATA_H

#include <string>
#include <vector>
#include "glm/glm.hpp"
using namespace std;

class MaterialData {
private:
	// NOTE: Indices refer to TextureData/TextureGL array elsewhere
	vector<int> allDiffuseTex;
	vector<int> allSpecularTex;
	vector<int> allNormalsTex;

	glm::vec3 kd = glm::vec3(0, 0, 0);
	glm::vec3 ks = glm::vec3(0, 0, 0);
	float shininess = 0;

	string name = "";

public:
	MaterialData();
	virtual ~MaterialData();

	void clearDiffuseTex();
	void clearSpecularTex();	
	void clearNormalsTex();

	void addDiffuseTex(int texIndex);
	void addSpecularTex(int texIndex);
	void addNormalsTex(int texIndex);

	int getDiffuseTex(int index);
	int getSpecularTex(int index);
	int getNormalsTex(int index);

	int getDiffuseTexCnt();
	int getSpecularTexCnt();	
	int getNormalsTexCnt();

	void setKd(glm::vec3 kd);
	void setKs(glm::vec3 ks);
	void setShininess(float shininess);
	void setName(string name);

	glm::vec3 getKd();
	glm::vec3 getKs();
	float getShininess();
	string getName();
};

#endif
