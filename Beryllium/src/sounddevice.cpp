#include "sounddevice.h"
#include <AL/al.h>
#include <stdio.h>

SoundDevice *SoundDevice::get()
{
	static SoundDevice *snd_device = new SoundDevice();
	return snd_device;
}

SoundDevice::SoundDevice()
{
	m_ALCDevice = alcOpenDevice(nullptr); // nullptr = get default device
	if (!m_ALCDevice)
		throw("failed to get sound device");

	m_ALCContext = alcCreateContext(m_ALCDevice, nullptr); // create context
	if (!m_ALCContext)
		throw("Failed to set sound context");

	if (!alcMakeContextCurrent(m_ALCContext)) // make context current
		throw("failed to make context current");

	const ALCchar *name = nullptr;
	if (alcIsExtensionPresent(m_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(m_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(m_ALCDevice) != AL_NO_ERROR)
		name = alcGetString(m_ALCDevice, ALC_DEVICE_SPECIFIER);
	printf("Opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice()
{
	if (!alcMakeContextCurrent(nullptr))
	{
		// todo : log error
	}

	alcDestroyContext(m_ALCContext);
	if (m_ALCContext)
	{
		// todo : log error
	}

	if (!alcCloseDevice(m_ALCDevice))
	{
		// todo : log error
	}
}
