#ifndef SOUND_H
#define SOUND_H

#include "miniaudio.h"
#include <atomic>

#define CHANNEL_COUNT       1
#define SAMPLE_RATE         48000 // 48kHz, 48k samples per second
#define TIME_PER_PROCESS    1 / 2   // s, expected process every 500ms at worst
#define RING_BUFFER_SIZE    (4 * TIME_PER_PROCESS * SAMPLE_RATE * CHANNEL_COUNT)
#define MAX_READ            (SAMPLE_RATE * TIME_PER_PROCESS * CHANNEL_COUNT)

class RingBuffer {
public:
    float buffer[RING_BUFFER_SIZE];
    std::atomic<size_t> head = 0;
    
    RingBuffer() {}
};

class Sound {
private:
    ma_device device;

    // attack smoothing T value
    const float T_ATTACK = 0.05; 

    // release smoothing T value
    const float T_RELEASE = 0.2;

public:
    RingBuffer buffer;
    std::atomic<float> env;
    std::atomic<bool> close = false;

public:
    static std::atomic<Sound*> globalSound;
    static Sound& instance();
    
private:
    Sound();
    void processSound();
    float sampleToMagnitude(float);
    float calculateEnv(float, float);

public:
    Sound(const Sound&) = delete;
    Sound& operator=(const Sound&) = delete;
};



#endif
