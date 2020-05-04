#pragma once
#include <string>
#include <iostream>
#include "../libs/irrKlang/irrKlang.h"
using namespace irrklang;
#pragma comment(lib, "../libs/irrKlang/irrKlang.lib")

class Sound
{
private:
	std::string newSound;
	irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

public:
	Sound();
	Sound(std::string file);
	void stopsound();
	void Sound::turnUpVolume();
	void Sound::turnDownVolume();
	void Sound::pausesound();
	void Sound::unpausesound();
};