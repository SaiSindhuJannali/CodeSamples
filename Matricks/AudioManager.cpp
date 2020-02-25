/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sai Sindhu Jannali
Modified by: Sairaj Padghe
- End Header --------------------------------------------------------*/
#include "stdafx.h"

AudioManager* AudioManager::pObject = nullptr;


AudioManager* AudioManager::CreateInstance()
{
	pObject = new AudioManager();
	return pObject;	
}

AudioManager::AudioManager():mSystem(nullptr),currentSong(nullptr),fade(FADE_NONE)
{
	SetSongsVolume(Audio_Volume);
	SetSFXsVolume(SFX_Volume);
	groups[CATEGORY_PROMPT]->setVolume(1.0f);
}

void AudioManager::Init()
{
	FMOD::System_Create(&mSystem);
	mSystem->init(100, FMOD_INIT_NORMAL, nullptr);
	// Create channels groups for each category
	mSystem->getMasterChannelGroup(&master);
	for (int i = 0; i < CATEGORY_COUNT; ++i) 
	{
		mSystem->createChannelGroup(0, &groups[i]);
		master->addGroup(groups[i]);
	}
	// Set up modes for each category
	modes[CATEGORY_SFX] = FMOD_DEFAULT;
	modes[CATEGORY_SONG] = FMOD_DEFAULT | FMOD_CREATESTREAM | FMOD_LOOP_NORMAL;
	// Seed random number generator for SFXs
	srand(time(0));
}

void AudioManager::Close()
{
	// Release sounds in each category
	SoundMap::iterator iter;
	for (int i = 0; i < CATEGORY_COUNT; ++i)
	{
		for (iter = sounds[i].begin(); iter != sounds[i].end(); ++iter)
			iter->second->release();
		sounds[i].clear();
	}
	// Release system
	mSystem->release();
	delete pObject;
}

void AudioManager::Update(float elapsed)
{
	groups[CATEGORY_PROMPT]->setVolume(40.0f);
 
   	const float fadeTime = 1.0f; // in seconds
   	if (currentSong != 0 && fade == FADE_IN)
   	{

   		float volume;
   		currentSong->getVolume(&volume);
   		float nextVolume = volume + elapsed / fadeTime;
   		if (nextVolume >= 1.0f) {
   			currentSong->setVolume(0.02f);
   			fade = FADE_NONE;
   		}
   		else
   		{
   			currentSong->setVolume(nextVolume);
   		}
   	}
   	else if (currentSong != 0 && fade == FADE_OUT) {
   		float volume;
   		currentSong->getVolume(&volume);
   		float nextVolume = volume - elapsed / fadeTime;
   		if (nextVolume <= 0.0f)
   		{
   			currentSong->stop();
   			currentSong = 0;
   			currentSongPath.clear();
   			fade = FADE_NONE;
   		}
   		else
   		{
   			currentSong->setVolume(nextVolume);
   		}
   	}
   	else if (currentSong == 0 && !nextSongPath.empty())
   	{
   		PlaySong(nextSongPath,musicVol);
   		nextSongPath.clear();
   	}
	mSystem->update();
	
	if (muteBGM)
		groups[CATEGORY_SONG]->setVolume(0.0f);
	else
		groups[CATEGORY_SONG]->setVolume(Audio_Volume);

	if (muteSFX) {
		groups[CATEGORY_SFX]->setVolume(0.0f);
		groups[CATEGORY_PROMPT]->setVolume(0.0f);
	}
	else {
		groups[CATEGORY_SFX]->setVolume(SFX_Volume);
		groups[CATEGORY_PROMPT]->setVolume(SFX_Volume);
	}

}

void AudioManager::ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
	//	std::cout << FMOD_ErrorString(result) << std::endl;
	}
	return;
}

void AudioManager::Load(Category type, const std::string& path)
{
	if (sounds[type].find(path) != sounds[type].end()) return;
	FMOD::Sound* sound;
	ErrorCheck(mSystem->createSound(path.c_str(), modes[type], 0, &sound));
	sounds[type].insert(std::make_pair(path, sound));
}
//
//float AudioManager::RandomBetween(float min, float max) 
//{
//	if (min == max) return min;
//	float n = (float)rand() / (float)RAND_MAX;
//	return min + n * (max - min);
//}
//
//float AudioManager::ChangeOctave(float frequency, float variation) 
//{
//	static float octave_ratio = 2.0f;
//	return frequency * pow(octave_ratio, variation);
//}
//
//float AudioManager::ChangeSemitone(float frequency, float variation) 
//{
//	static float semitone_ratio = pow(2.0f, 1.0f / 12.0f);
//	return frequency * pow(semitone_ratio, variation);
//}

void AudioManager::PlaySFX(const std::string & path, float Volume, bool loopable)
{
	//std::cout << groups[CATEGORY_SONG]->getVolume(&Audio_Volume) << std::endl;

	if (isMute) 
		musicVol = 0;
	else 
		musicVol = Volume;
	// Ignore if this song is already playing
	if (currentSFXPath == path)
	{
		//StopSFX(currentSFXPath);
		return;
	}

	SoundMap::iterator sound = sounds[CATEGORY_SFX].find(path);

	if (sound == sounds[CATEGORY_SFX].end()) return;
	currentSFXPath = path;
	mSystem->playSound(sound->second, NULL, true, &currentSFX);
	currentSFX->setChannelGroup(groups[CATEGORY_SFX]);

	if (SFX_Volume > 0.0f && SFX_Volume <= 100.0f)
	{
		sfx_volume = SFX_Volume / 1000.0f;
		groups[CATEGORY_SFX]->setVolume(sfx_volume);
	}
	currentSFX->setVolume(sfx_volume);
	currentSFX->setPaused(false);

	if (loopable)
		currentSFX->setMode(FMOD_LOOP_NORMAL);
}

void AudioManager::LoadSFX(const std::string & path)
{
	Load(CATEGORY_SFX, path);
}

void AudioManager::LoadSong(const std::string & path)
{
	Load(CATEGORY_SONG, path);
}

void AudioManager::LoadPromptAudio(const std::string & path)
{
	Load(CATEGORY_PROMPT, path);
}

void AudioManager::PlaySong(const std::string& path, float Volume)
{
	//std::cout << groups[CATEGORY_SFX]->getVolume(&SFX_Volume) << std::endl;

	musicVol = Volume;
	// Ignore if this song is already playing
	if (currentSongPath == path) return;
	// If a song is playing stop them and set this as the next song
	if (currentSong != 0) 
	{
		StopSongs();
		nextSongPath = path;
		return;
	}
	// Find the song in the corresponding sound map
	SoundMap::iterator sound = sounds[CATEGORY_SONG].find(path);
	if (sound == sounds[CATEGORY_SONG].end()) return;
	// Start playing song with volume set to 0 and fade in
	currentSongPath = path;
	ErrorCheck(mSystem->playSound(sound->second, NULL, true, &currentSong));
	ErrorCheck(currentSong->setChannelGroup(groups[CATEGORY_SONG]));
	if (Audio_Volume > 0.0f && Audio_Volume <= 100.0f)
	{
		bgm_volume = Audio_Volume / 1000.0f;
		//groups[CATEGORY_SONG]->setVolume(bgm_volume);
	}
	ErrorCheck(currentSong->setVolume(bgm_volume));
	ErrorCheck(currentSong->setPaused(false));
	fade = FADE_IN;
}

void AudioManager::PlayPromptAudio(const std::string & path, float Volume, bool loopable)
{
	if (isMute)
		musicVol = 0;
	else
		musicVol = Volume;
	// Ignore if this song is already playing
	if (currentPromptPath == path)
	{
		//StopSFX(currentSFXPath);
		return;
	}
	SoundMap::iterator sound = sounds[CATEGORY_PROMPT].find(path);

	if (sound == sounds[CATEGORY_PROMPT].end()) return;
	currentPromptPath = path;
	//isPromptPlaying = true;
	mSystem->playSound(sound->second, NULL, true, &PromptAudio);
	PromptAudio->setChannelGroup(groups[CATEGORY_PROMPT]);
	PromptAudio->setVolume(musicVol);
	PromptAudio->setPaused(false);

	if (loopable)
		PromptAudio->setMode(FMOD_LOOP_NORMAL);

}

void AudioManager::StopSFXs() 
{
	groups[CATEGORY_SFX]->stop();
	currentSFXPath.clear();
}

void AudioManager::StopPromptAudio(const std::string & path)
{
	groups[CATEGORY_PROMPT]->stop();
	currentPromptPath.clear();

}

void  AudioManager::StopSFX(const std::string& path)
{
	FMOD::Sound* sound = sounds[CATEGORY_SFX][path];
	if (sound)
	{
		currentSFXPath.clear();
		FMOD::ChannelGroup* ch = groups[CATEGORY_SFX];
		ch->stop();
	}
}

void AudioManager::StopSongs() 
{
	if (currentSong != 0)
		fade = FADE_OUT;
	nextSongPath.clear();
	currentSongPath.clear();
	PromptAudio->setPaused(true);
	groups[CATEGORY_SONG]->stop();

}

void AudioManager::StopPrompt()
{
	groups[CATEGORY_PROMPT]->stop();
	currentPromptPath.clear();
}

void AudioManager::SetMasterVolume(float volume)
{
// 	if (volume > 1.0f && volume <= 10.0f)
// 		volume *= 50.0f;
	if (volume > 0.0f && volume <= 100.0f)
		volume /= 10.0f;
	master->setVolume(volume);
}

void AudioManager::SetSFXsVolume(float volume) 
{ 
	if (volume > 0.0f && volume <= 100.0f)
	{
		volume /= 100.0f;
		sfx_volume = volume;
		groups[CATEGORY_SFX]->setVolume(volume);
	}
// 	else
// 		groups[CATEGORY_SFX]->setVolume(0.0f);
 }

void AudioManager::SetSongsVolume(float volume) 
{
	if (volume > 0.0f && volume <= 100.0f)
	{
		volume /= 100.0f;
		bgm_volume = volume;
		groups[CATEGORY_SONG]->setVolume(volume);
	}
// 	else
// 		groups[CATEGORY_SONG]->setVolume(0.0f);
}

void AudioManager::ToggleMuteMusic() 
{ 
	isMute ? UnMuteMusic() : MuteMusic(); 
}

void AudioManager::setSFXvolume()
{
	SFXvolume = 0.1;
}

void AudioManager::ResetSFXvolume()
{
	SFXvolume = 0.5;
}

void AudioManager::PausePrompt()
{
	PromptAudio->setPaused(true);
}

void AudioManager::UnpausePrompt()
{
	PromptAudio->setPaused(false);
}

void AudioManager::pauseAudio()
{
	currentSong->setPaused(true);
	currentSFX->setPaused(true);
	PromptAudio->setPaused(true);
}

void AudioManager::unpauseAudio()
{
	currentSong->setPaused(false);
	currentSFX->setPaused(false);
	PromptAudio->setPaused(false);
}
