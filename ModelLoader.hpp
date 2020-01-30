#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

/*
//
// NOTE: The code that follows is heavily inspired by code from https://learnopengl.com/.
//
*/

#include "ModelData.hpp"
#include <string>
using namespace std;

void addDefaultTextures(ModelData *model, int &diffuseTex, int &specTex, int &normalTex);
int addDefaultMaterial(ModelData *model, int diffuseTex, int specTex, int normalTex);
ModelData* loadModel(string path);

#endif
