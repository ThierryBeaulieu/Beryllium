#pragma once
#include <AL/alc.h>

class SoundDevice
{
public:
	static SoundDevice *get();

private:
	SoundDevice();
	~SoundDevice();

	ALCdevice *m_ALCDevice;
	ALCcontext *m_ALCContext;
};
