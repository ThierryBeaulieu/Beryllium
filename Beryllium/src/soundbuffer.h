#pragma once
#include <AL/al.h>
#include <vector>
class SoundBuffer
{
public:
	static SoundBuffer *get();

	ALuint AddSoundEffect(const char *filename);
	bool RemoveSoundEffect(const ALuint &buffer);

private:
	SoundBuffer();
	~SoundBuffer();

	std::vector<ALuint> m_SoundEffectBuffers;
};
