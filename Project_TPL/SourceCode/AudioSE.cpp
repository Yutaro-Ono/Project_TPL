#include "AudioSE.h"
#include <map>



AudioSE::AudioSE()
	:m_chunk(nullptr)
	,m_channel(-1)
	,m_volume(10)
{

}

AudioSE::~AudioSE()
{
	Mix_FreeChunk(m_chunk);
}

void AudioSE::Play()
{
	m_channel = Mix_PlayChannel(-1, m_chunk, 0);
}

void AudioSE::Stop()
{
	if (m_channel == -1)
	{
		return;
	}

	if (Mix_Playing(m_channel))
	{
		Mix_HaltChannel(m_channel);
	}

	m_channel = -1;
}

/// <summary>
/// サウンドチャンクの音量をセット
/// </summary>
/// <param name="in_vol"> 音量 (1 ～ 128) </param>
void AudioSE::SetChunkVolume(int in_vol)
{
	m_volume = in_vol;
	Mix_VolumeChunk(m_chunk, m_volume);
}

bool AudioSE::LoadSound(const std::string & in_fileName)
{
	// チャンクにロード
	m_chunk = Mix_LoadWAV(in_fileName.c_str());
	// 音量をセット
	Mix_VolumeChunk(m_chunk, m_volume);

	return m_chunk != nullptr;
}

bool AudioSE::IsPlaying()
{
	return m_channel >= 0 && Mix_Playing(m_channel);
}
