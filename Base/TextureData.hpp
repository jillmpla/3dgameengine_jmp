#ifndef TEXTURE_DATA_H
#define TEXTURE_DATA_H


#include <string>
#include "glm/glm.hpp"
using namespace std;

enum TextureType { TEXTURE_FLOAT, TEXTURE_UCHAR };

class TextureData {
private:	
	unsigned char *data = 0;
	TextureType type = TEXTURE_UCHAR;
	int width = 0;
	int height = 0;
	int channels = 0;
	string path = "";

public:
	TextureData(std::string texturePath);
	TextureData(int width, int height, glm::vec4 color);
	TextureData(int width, int height, unsigned char color);
	virtual ~TextureData();
	
	int getWidth();
	int getHeight();
	int getChannels();
	string getPath();
	TextureType getType();
	void* getData();
};


#endif

