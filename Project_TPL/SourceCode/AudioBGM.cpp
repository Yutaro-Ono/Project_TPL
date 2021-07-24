#include "AudioBGM.h"

AudioBGM::AudioBGM()
	:m_music(nullptr)
	,m_volume(6)
{
	Mix_VolumeMusic(m_volume);
}

AudioBGM::~AudioBGM()
{
	Mix_FreeMusic(m_music);
}

void AudioBGM::Play()
{
	if (m_music)
	{
		Mix_PlayMusic(m_music, -1);
	}
}

void AudioBGM::FadeInMusic(int in_fadeTime)
{
	if (m_music)
	{
		Mix_FadeInMusic(m_music, -1, in_fadeTime);
	}
}

bool AudioBGM::LoadMusic(const std::string & in_fileName)
{
	m_music = Mix_LoadMUS(in_fileName.c_str());

	return m_music != nullptr;
}

/// <summary>
/// âπäyÇÃâπó í≤êÆ (1 Å` 128)
/// </summary>
/// <param name="in_vol">îCà”ÇÃâπó (1 Å` 128)</param>
void AudioBGM::SetMusicVolume(int in_vol)
{
	m_volume = in_vol;
	Mix_VolumeMusic(m_volume);
}

void AudioBGM::Stop()
{
	Mix_HaltMusic();
}

void AudioBGM::FadeOutMusic(int in_fadeTime)
{
	Mix_FadeOutMusic(in_fadeTime);
}

bool AudioBGM::IsPlaying()
{
	return Mix_PlayingMusic();
}
