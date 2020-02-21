#include "TextureData.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureData::TextureData(std::string texturePath) {
	// Save path information
	this->path = texturePath;

	// Set type
	this->type = TEXTURE_UCHAR;

	// Load texture from file	
	data = stbi_load(this->path.c_str(), &width, &height, &channels, 0);

	// If we couldn't load the texture, throw an exception
	if (!data) {
		throw new std::runtime_error(std::string("Error loading texture: " + this->path).c_str());
	}
}

TextureData::TextureData(int width, int height, glm::vec4 color) {
	// Reset path information
	this->path = "";

	// Set width, height, and channels
	this->width = width;
	this->height = height;
	this->channels = 4;

	// Set type
	this->type = TEXTURE_FLOAT;

	// Create data array
	float *fdata = new float[width*height * this->channels];
	int size = width * height;
	int index = 0;
	for (int i = 0; i < size; i++) {
		fdata[index++] = color.r;
		fdata[index++] = color.g;
		fdata[index++] = color.b;
		fdata[index++] = color.a;
	}
	data = (unsigned char*)fdata;
}

TextureData::TextureData(int width, int height, unsigned char color) {
	// Reset path information
	this->path = "";
	
	// Set type
	this->type = TEXTURE_UCHAR;

	// Set width, height, and channels
	this->width = width;
	this->height = height;
	this->channels = 1;

	// Create data array
	unsigned char* udata = new unsigned char[width*height];
	int size = width * height;
	for (int i = 0; i < size; i++) {
		udata[i] = color;
	}
	data = udata;
}

TextureData::~TextureData() {
	if (data) {
		delete[] data;
	}
}

int TextureData::getWidth() { return width; }
int TextureData::getHeight() { return height; }
int TextureData::getChannels() { return channels; }
string TextureData::getPath() { return path; }
TextureType TextureData::getType() { return type; }
void* TextureData::getData() { return data; }

