#pragma once
#include <SDL_mixer.h>
#include <iostream>

class AudioBGM
{


public:

	AudioBGM();
	~AudioBGM();

	void Play();
	void FadeInMusic(int in_fadeTime);
	bool LoadMusic(const std::string& in_fileName);

	void SetMusicVolume(int in_vol);

	static void Stop();
	static void FadeOutMusic(int in_fadeTime);
	static bool IsPlaying();

private:

	Mix_Music* m_music;

	int m_volume;            // 音楽のボリューム (1〜128)

};