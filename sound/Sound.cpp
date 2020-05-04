#include "Sound.hpp"

Sound::Sound() {
	
}

Sound::Sound(std::string file) {
	newSound = file;
	char* c = &*newSound.begin();

	engine->play2D(c, true, false, true);

	engine->setSoundVolume(0.25f);
}

void Sound::pausesound() {
	engine->setAllSoundsPaused(true);
}

void Sound::unpausesound() {
	engine->setAllSoundsPaused(false);
}

void Sound::stopsound() {
	engine->stopAllSounds();
}

void Sound::turnUpVolume() {
	float currentVolume = engine->getSoundVolume();
	if (currentVolume <= 0.98f) {
		currentVolume = currentVolume + 0.02;
	}
	engine->setSoundVolume(currentVolume);
}

void Sound::turnDownVolume() {
	float currentVolume = engine->getSoundVolume();
	if (currentVolume >= 0.02f) {
		currentVolume = currentVolume - 0.02;
	}
	engine->setSoundVolume(currentVolume);
}
