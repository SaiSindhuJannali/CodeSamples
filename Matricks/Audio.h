/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sai Sindhu Jannali
Modified By: Sairaj Padghe
- End Header --------------------------------------------------------*/
#pragma once

class Audio :public Component
{
public:
	Audio();
	void Update();
	void Serialize(rapidjson::Value::Object obj);
	void DeSerialize(std::ofstream* file);
	void Initialize();
	Component* Create();
	void Clear();
	void HandleEvent(Event* pEvent) {}

	void Play(int index = 0);
	void Stop();
	void StopSFX(int index = 0);
	void Pause();
	void Resume();
	void SetVolume(float volume) { mVolume = volume; }
	void Loop(bool active) { mIsLoop = active; }
	void Mute();
	void UnMute();
	void ToggleMute();
	int getFadeState() { return fadeState; }
	void setFadeState(int fade) { fadeState = fade; }

	std::unordered_map< int , std::tuple<std::string , float , bool, bool> > m_audioClip;
	bool isPromptAuido;
	bool isMute;

	bool isSFX;
private:
	int mCurrentIndex;
	float mVolume;
	float mMinDist, mMaxDist;
	bool mIsmute;
	bool mIsLoop;
	bool mIsBGM;
	float mFadeTime;
	int fadeState;

};