#ifndef SOUND_H
#define SOUND_H

#include "miniaudio.h"
#include <thread>
#include <atomic>

#define CHANNEL             1
#define SAMPLE_RATE         48000 // 48kHz, 48k samples per second
#define TIME_PER_PROCESS    500   // ms, expected process every 500ms at worst
#define RING_BUFFER_SIZE    (TIME_PER_PROCESS * SAMPLE_RATE * CHANNEL)

class ringBuffer;

class Sound {
private:
    ma_device device;
    std::atomic<float> envelope;

public:
    static bool initiated;
    static Sound& instance();
    
private:
    Sound();

public:
    Sound(const Sound&) = delete;
    Sound& operator=(const Sound&) = delete;
};

class ringBuffer {
    float buffer[RING_BUFFER_SIZE];
    size_t write = 0;
    size_t read = 0;
};

#endif
