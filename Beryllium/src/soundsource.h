#pragma once
#include <AL/al.h>

class SoundSource
{
public:
	SoundSource();
	~SoundSource();

	void Play(const ALuint buffer_to_play);
	void PlayLoop(const ALuint buffer_to_play);

private:
	ALuint m_Source;
	float m_Pitch = 1.f;
	float m_Gain = 1.f;
	float m_Position[3] = {0, 0, 0};
	float m_Velocity[3] = {0, 0, 0};
	bool m_LoopSound = false;
	ALuint m_Buffer = 0;
};
