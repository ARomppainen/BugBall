#include "SoundPlayer.h"

SoundPlayer::SoundPlayer(){
	
}

SoundPlayer::~SoundPlayer(){

}

void SoundPlayer::setCurrentGroupFile(const char* groupFile){
	CIwResGroup* TestLevelGroup = IwGetResManager()->GetGroupNamed(groupFile);
	IwGetResManager()->SetCurrentGroup(TestLevelGroup);

	prevGroup = groupFile;
}

void SoundPlayer::playSound(const char* wavefile){
	CIwResGroup* TestLevelGroup = IwGetResManager()->GetGroupNamed(prevGroup);
	IwGetResManager()->SetCurrentGroup(TestLevelGroup);

	CIwSoundSpec* punchSound = (CIwSoundSpec*)TestLevelGroup->GetResNamed(wavefile, IW_SOUND_RESTYPE_SPEC);
	//CIwSoundInst* SoundInstance = punchSound->Play();
    punchSound->Play();
}

void SoundPlayer::playSound(const char* wavefile, const char* groupFile){
	CIwResGroup* TestLevelGroup = IwGetResManager()->GetGroupNamed(groupFile);
	IwGetResManager()->SetCurrentGroup(TestLevelGroup);

	CIwSoundSpec* punchSound = (CIwSoundSpec*)TestLevelGroup->GetResNamed(wavefile, IW_SOUND_RESTYPE_SPEC);
	//CIwSoundInst* SoundInstance = punchSound->Play();
    punchSound->Play();

	prevGroup = groupFile;
}

void SoundPlayer::playMusic(const char* bgm){
	if(s3eAudioIsCodecSupported(S3E_AUDIO_CODEC_MP3))
		s3eAudioPlay(bgm, 0);
}

void SoundPlayer::stopMusic(){
	s3eAudioStop();
}

void SoundPlayer::pauseMusic(){
	s3eAudioPause();
}