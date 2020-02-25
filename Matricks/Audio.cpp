/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sai Sindhu Jannali
Modified By: Sairaj Padghe
- End Header --------------------------------------------------------*/
#include "stdafx.h"

Audio::Audio():Component(AUDIO), mCurrentIndex(0)
{
}

Component* Audio::Create()
{
	return new Audio();
}

void Audio::Clear()
{
	mIsmute = false;
	mIsLoop = false;
	mIsBGM = false;
	isPromptAuido = false;
	isMute = false;
	isSFX = false;
	m_audioClip.clear();
}

void Audio::Initialize()
{
}

void Audio::Update()
{
	if (mpOwner->isPaused)
	{
		Stop();
		return;
	}
	if (mIsBGM)
	{
		AudioManager::GetInstance()->PlaySong(std::get<0>(m_audioClip[mCurrentIndex]), std::get<1>(m_audioClip[mCurrentIndex]));
	}
	
	if (mIsmute) 
	{
		AudioManager::GetInstance()->MuteMusic();
	}
}

void Audio::Serialize(rapidjson::Value::Object obj)
{
	std::string fullPath = "Resources/Audio/";
	if (obj.HasMember("AudioClip"))
	{
		rapidjson::Value::Array audio = obj["AudioClip"].GetArray();
		for (unsigned int i = 0; i < audio.Size(); ++i)
		{
			rapidjson::Value::Object clip = audio[i].GetObject();
			std::string name = fullPath + clip["Clip"].GetString();
			float volume = clip["Volume"].GetFloat();
			bool loopable = clip["IsLoop"].GetBool();
			bool priority = false;
			if (clip.HasMember("IsPrior"))  
				priority = clip["IsPrior"].GetBool();
			m_audioClip[i] = std::make_tuple(name, volume, loopable,priority);
		}
	}
	if (obj.HasMember("IsMute")) mIsmute = obj["IsMute"].GetBool();
	if (obj.HasMember("BGM")) mIsBGM = obj["BGM"].GetBool();
	if (obj.HasMember("isPromptAudio")) isPromptAuido = obj["isPromptAudio"].GetBool();
	if (obj.HasMember("isSFX")) isSFX = obj["isSFX"].GetBool();
	if (obj.HasMember("FadeTime")) mFadeTime = obj["FadeTime"].GetFloat();

	if (mIsBGM)
	{
		for (unsigned int i = 0; i < m_audioClip.size(); ++i) {
			AudioManager::GetInstance()->LoadSong(std::get<0>(m_audioClip[i]));
		}
	}
	if(isSFX) 
	{
		for (unsigned int i = 0; i < m_audioClip.size(); ++i) {
			AudioManager::GetInstance()->LoadSFX(std::get<0>(m_audioClip[i]));
		}
	}
	if (isPromptAuido)
	{
		for (unsigned int i = 0; i < m_audioClip.size(); ++i) {
			AudioManager::GetInstance()->LoadPromptAudio(std::get<0>(m_audioClip[i]));
		}
	}
}

void Audio::DeSerialize(std::ofstream* file)
{
	*file << "{\n\"Type\":\"Audio\",\n";
	*file << "\"AudioClip\":[\n";
	for (auto a : m_audioClip)
	{
		//*file << "{\n\"Type\":\"" << a.first << "\",\n\"Sheet\":\"" << a.second << "\"\n},\n";
	}
	long pos = (long)file->tellp();
	file->seekp(pos - 2);
	*file << "\n]\n},\n";
}

void Audio::Play(int index)
{
	if (index >= (int)m_audioClip.size())
		return;
	else {
		mCurrentIndex = index;
	}
	isMute = mIsmute;

	if (mIsBGM) {
		if (mIsmute)
			AudioManager::GetInstance()->PlaySong(std::get<0>(m_audioClip[mCurrentIndex]), 0.0);
		else
			AudioManager::GetInstance()->PlaySong(std::get<0>(m_audioClip[mCurrentIndex]), std::get<1>(m_audioClip[mCurrentIndex]));
	}
	if(isSFX)
	{
		if (mIsmute)
			AudioManager::GetInstance()->PlaySFX(std::get<0>(m_audioClip[mCurrentIndex]), 0, mIsLoop);
		else
		{
			AudioManager::GetInstance()->PlaySFX(std::get<0>(m_audioClip[mCurrentIndex]), std::get<1>(m_audioClip[mCurrentIndex]), std::get<2>(m_audioClip[mCurrentIndex]));
		}
	}
	if (isPromptAuido)
	{
		//AudioManager::GetInstance()->setSFXvolume();
		if (mIsmute)
			AudioManager::GetInstance()->PlayPromptAudio(std::get<0>(m_audioClip[mCurrentIndex]), 0, mIsLoop);
		else
		{
			AudioManager::GetInstance()->PlayPromptAudio(std::get<0>(m_audioClip[mCurrentIndex]), std::get<1>(m_audioClip[mCurrentIndex]), std::get<2>(m_audioClip[mCurrentIndex]));
		}
	}
	//else
		//AudioManager::GetInstance()->ResetSFXvolume();
}

void Audio::Stop()
{
	if (mIsBGM)
		AudioManager::GetInstance()->StopSongs();
	if(isSFX)
		AudioManager::GetInstance()->StopSFX(std::get<0>(m_audioClip[mCurrentIndex]));
// 	if(isPromptAuido)
// 		AudioManager::GetInstance()->StopPromptAudio(std::get<0>(m_audioClip[mCurrentIndex]));

}

void Audio::StopSFX(int index)
{
	if (std::get<0>(m_audioClip[mCurrentIndex]).compare("Pickup_MagicBells02.wav") == 0)
	{
	}//std::cout << "Yes" << std::endl;
	else
		AudioManager::GetInstance()->StopSFX(std::get<0>(m_audioClip[index]));
}

void Audio::Pause()
{
	AudioManager::GetInstance()->PauseMusic();
}

void Audio::Resume()
{
	AudioManager::GetInstance()->ResumeMusic();
}

void Audio::Mute()
{
	AudioManager::GetInstance()->MuteMusic();
	isMute = true;
}

void Audio::UnMute()
{
	isMute = false;
	AudioManager::GetInstance()->UnMuteMusic();
}

void Audio::ToggleMute()
{
	(isMute) ? UnMute() : Mute();
}
