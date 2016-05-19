#pragma once

#include "SoundPlayer.h"
#include "IwResManager.h"
#include "IwSound.h"
#include "s3eAudio.h"
#include <iostream>

class SoundPlayer
{
public:
	SoundPlayer();
	~SoundPlayer();

	void setCurrentGroupFile(const char* groupFile);
	void playSound(const char* waveFile);
	void playSound(const char* wavefile, const char* groupFile);
	void playMusic(const char* bgm);
	void stopMusic();
	void pauseMusic();
private:
	const char* prevGroup;
};