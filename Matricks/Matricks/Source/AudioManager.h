/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sai Sindhu Jannali
Modified by: Sairaj Padghe
- End Header --------------------------------------------------------*/
#pragma once

enum FadeState
{
	FADE_NONE,
	FADE_IN,
	FADE_OUT
};

enum Category
{
	CATEGORY_SFX,
	CATEGORY_SONG,
	CATEGORY_PROMPT,
	CATEGORY_COUNT
};

typedef std::map<std::string, FMOD::Sound*> SoundMap;

class AudioManager
{
	static AudioManager *pObject;
	AudioManager();
public:

	AudioManager(const AudioManager &) = delete;
	void operator=(const AudioManager &) = delete;

	static AudioManager* CreateInstance();
 	inline static AudioManager* GetInstance() { return pObject; }

	void Init();
	void Close();
	void Update(float elapsed);
	void Load(Category type, const std::string& path);
	void ErrorCheck(FMOD_RESULT result);

	void LoadSFX(const std::string& path);
	void LoadSong(const std::string& path);
	void LoadPromptAudio(const std::string& path);

	void PlaySFX(const std::string& path, float Volume, bool loopable);
	void PlaySong(const std::string& path, float Volume);
	void PlayPromptAudio(const std::string& path, float Volume, bool loopable);

	void SetMasterVolume(float volume);
	void SetSFXsVolume (float volume);
	void SetSongsVolume(float volume);

	void StopSFX(const std::string& path);
	void StopSFXs();
	void StopPromptAudio(const std::string& path);
	void StopSongs();
	void StopPrompt();

	void PauseMusic() { currentSong->setPaused(true); }
	void ResumeMusic() { currentSong->setPaused(false); }

	void MuteMusic() { isMute = true;  muteBGM = muteSFX = true;  currentSong->setMute(true); currentSFX->setMute(true); PromptAudio->setMute(true); }
	void UnMuteMusic() { isMute = false; muteBGM = muteSFX = false; currentSong->setMute(false); currentSFX->setMute(false); PromptAudio->setMute(false); }
	void ToggleMuteMusic();
	void SetFadeTime(float time) { fadeTime = time; }

	float Audio_Volume = 50, bgm_volume;
	float SFX_Volume = 50, sfx_volume;
	bool muteBGM = false;
	bool muteSFX = false;

	void setSFXvolume();
	void ResetSFXvolume();

	void PausePrompt();
	void UnpausePrompt();

	void pauseAudio();
	void unpauseAudio();

	std::string currentSongPath;
	std::string currentSFXPath;
	std::string currentPromptPath; 


private:
	FMOD::System* mSystem;
	FMOD::ChannelGroup* master;
	FMOD::ChannelGroup* groups[CATEGORY_COUNT];
	FMOD_MODE modes[CATEGORY_COUNT];

	FadeState fade;
	float fadeTime;
	SoundMap sounds[CATEGORY_COUNT];

	FMOD::Channel* currentSong;
	FMOD::Channel* currentSFX;
	FMOD::Channel* PromptAudio;
	bool currentPriority = false;
	std::string nextSongPath;

	float musicVol;
	bool isMute;
	bool isPromptPlaying=false;

	float SFXvolume = 0.5;
};