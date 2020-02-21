#include "MaterialData.hpp"

MaterialData::MaterialData() {}
MaterialData::~MaterialData() {
	allDiffuseTex.clear();
	allSpecularTex.clear();
	allNormalsTex.clear();
}

void MaterialData::clearDiffuseTex() {
	allDiffuseTex.clear();
}

void MaterialData::clearSpecularTex() {
	allSpecularTex.clear();
}

void MaterialData::clearNormalsTex() {
	allNormalsTex.clear();
}

void MaterialData::addDiffuseTex(int texIndex) {
	allDiffuseTex.push_back(texIndex);
}

void MaterialData::addSpecularTex(int texIndex) {
	allSpecularTex.push_back(texIndex);
}

void MaterialData::addNormalsTex(int texIndex) {
	allNormalsTex.push_back(texIndex);
}

int MaterialData::getDiffuseTex(int index) {
	if (index >= 0 && index < allDiffuseTex.size()) {
		return allDiffuseTex.at(index);
	}
	else {
		return -1;
	}
}

int MaterialData::getSpecularTex(int index) {
	if (index >= 0 && index < allSpecularTex.size()) {
		return allSpecularTex.at(index);
	}
	else {
		return -1;
	}
}

int MaterialData::getNormalsTex(int index) {
	if (index >= 0 && index < allNormalsTex.size()) {
		return allNormalsTex.at(index);
	}
	else {
		return -1;
	}
}

int MaterialData::getDiffuseTexCnt() {
	return (int)allDiffuseTex.size();
}

int MaterialData::getSpecularTexCnt() {
	return (int)allSpecularTex.size();
}

int MaterialData::getNormalsTexCnt() {
	return (int)allNormalsTex.size();
}

void MaterialData::setKd(glm::vec3 kd) {
	this->kd = kd;
}

void MaterialData::setKs(glm::vec3 ks) {
	this->ks = ks;
}

void MaterialData::setShininess(float shininess) {
	this->shininess = shininess;
}

void MaterialData::setName(string name) {
	this->name = name;
}

glm::vec3 MaterialData::getKd() { return kd; }
glm::vec3 MaterialData::getKs() { return ks; }
float MaterialData::getShininess() { return shininess; }
string MaterialData::getName() { return name; }
