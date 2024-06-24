#pragma once

#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include <string>
#include <vector>

class SoundManager
{
public:
    SoundManager();
    ~SoundManager();

    void PlaySound(const std::string &wavSound);

private:
    ALCdevice *device;
    ALCcontext *context;
    std::vector<ALuint> buffers;

    ALuint LoadSound(const std::string &filename);
    void CleanUp();
};
