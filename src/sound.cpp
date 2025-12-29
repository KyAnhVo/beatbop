#include "sound.h"
#include <atomic>
#include <stdexcept>
#include <thread>
#include <cmath>

// must do this once...
#define ever ;;

void miniaudioCallback(ma_device *, void *, const void *, ma_uint32);

std::atomic<Sound*> Sound::globalSound = nullptr;

Sound::Sound() {
    Sound::globalSound.store(this, std::memory_order_relaxed);
    env = 0;
    
    ma_device_config config = ma_device_config_init(ma_device_type_capture);
    config.capture.format   = ma_format_f32;
    config.capture.channels = CHANNEL_COUNT;
    config.sampleRate       = SAMPLE_RATE;
    config.dataCallback = miniaudioCallback;

    ma_result r = ma_device_init(NULL, &config, &device);
    if (r != MA_SUCCESS) {
        throw std::runtime_error("initiate device unsuccessful");
    }
    processSound();
}

Sound& Sound::instance() {
    static Sound inst;
    return inst;
}

void Sound::processSound() {
    size_t head, prevHead = 0, minStart;
    float env = this->env.load(std::memory_order_acquire);

    // sorry i wanna do this one time
    for (ever) {
        head = buffer.head.load(std::memory_order_relaxed);
        if (head == prevHead) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            continue;
        }
        minStart = RING_BUFFER_SIZE + head - MAX_READ;
        minStart = (minStart < RING_BUFFER_SIZE) ? minStart : minStart - RING_BUFFER_SIZE;

        /** CASES
         *  1A: 0...minStart...prevHead...head...RBS-1 -> start from prevHead
         *  1B: 0...prevHead...minStart...head...RBS-1 -> start from minStart
         *  1C: 0...prevHead...head...minStart...RBS-1 -> start from prevHead
         *  2A: 0...head...minStart...prevHead...RBS-1 -> start from prevHead
         *  2B: 0...head...prevHead...minStart...RBS-1 -> start from minStart
         *  2C: 0...minStart...head...prevHead...RBS-1 -> start from minStart
         *
         *  NOTE: RBS is RING_BUFFER_SIZE
         */

        if (prevHead < head) {                              
            // CASE 1
            if (prevHead <= minStart && minStart < head) {  
                // CASE 1B
                for (size_t i = minStart; i < head; i++) {
                    env = calculateEnv(env, buffer.buffer[i % RING_BUFFER_SIZE]);
                }
            } else {                                        
                // CASE 1A, 1C
                for (size_t i = prevHead; i < head; i++) {
                    env = calculateEnv(env, buffer.buffer[i % RING_BUFFER_SIZE]);
                }
            }
        } else {                                            
            // CASE 2
            if (head <= minStart && minStart < prevHead) {  
                // CASE 2A
                for (size_t i = prevHead; i < head; i++) {
                    env = calculateEnv(env, buffer.buffer[i % RING_BUFFER_SIZE]);
                }
            } else {                                        
                // CASE 2B, 2C
                for (size_t i = minStart; i < head; i++) {
                    env = calculateEnv(env, buffer.buffer[i % RING_BUFFER_SIZE]);
                }
            }
        }

        // alter head for next lookup, env for visualizer read
        buffer.head.store(head, std::memory_order_relaxed);
        this->env.store(env, std::memory_order_release);
    }
}

float Sound::sampleToMagnitude(float sample) {
    return sample * sample;
}

float Sound::calculateEnv(float env, float sample) {
    float m = sampleToMagnitude(sample);

    // attack-release smoothing
    float T, coef;
    T = (m > env) ? T_ATTACK : T_RELEASE;
    coef = 1 - std::expf(-1 / (SAMPLE_RATE * T));
    
    return env + coef * (m - env);
}

/****************************** AUDIO CALLBACK ******************************/

/**
 * Audio callback function for miniaudio, only happen once
 * thing is actually initiated
 */
void miniaudioCallback(
    ma_device * device,
    void * output,
    const void * input,
    ma_uint32 frameCount
) {
    // shouldnt happen, but we want stuff to be initiated
    if (!Sound::globalSound) return;

    Sound* sound = Sound::globalSound.load(std::memory_order_relaxed);
    size_t head = sound->buffer.head.load(std::memory_order_relaxed);
    float * in = (float*) input;
    for (int i = 0; i < frameCount; i++) {
        sound->buffer.buffer[head] = in[i];
        head = (head + 1) % RING_BUFFER_SIZE;
    }
    sound->buffer.head.store(head, std::memory_order_relaxed);
}


