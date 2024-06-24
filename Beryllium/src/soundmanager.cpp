#include "soundmanager.h"

SoundManager::SoundManager()
{
    device = alcOpenDevice(nullptr);
    if (!device)
    {
        std::cerr << "Failed to open sound device" << std::endl;
        return;
    }

    context = alcCreateContext(device, nullptr);
    if (!context)
    {
        std::cerr << "Failed to create OpenAL context" << std::endl;
        alcCloseDevice(device);
        return;
    }

    alcMakeContextCurrent(context);
}

SoundManager::~SoundManager()
{
    CleanUp();
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void SoundManager::PlaySound(const std::string &wavSound)
{
    ALuint buffer = LoadSound(wavSound);
    if (buffer == 0)
    {
        std::cerr << "Failed to load sound: " << wavSound << std::endl;
        return;
    }

    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);

    ALint source_state;
    do
    {
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
    } while (source_state == AL_PLAYING);

    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
}

ALuint SoundManager::LoadSound(const std::string &filename)
{
    ALuint buffer;
    alGenBuffers(1, &buffer);

    ALshort dummyData[] = {0};

    alBufferData(buffer, AL_FORMAT_MONO16, dummyData, sizeof(dummyData), 44100);

    buffers.push_back(buffer);

    return buffer;
}

void SoundManager::CleanUp()
{
    for (ALuint buffer : buffers)
    {
        alDeleteBuffers(1, &buffer);
    }
    buffers.clear();
}
