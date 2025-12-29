#include "sound.h"
#include <stdexcept>

void miniaudioCallback(
        ma_device *,
        void *,
        const void *,
        ma_uint32
        );

bool Sound::initiated = false;

Sound::Sound() {
    initiated = true;
    ma_device_config config = ma_device_config_init(ma_device_type_capture);
    config.capture.format   = ma_format_f32;
    config.capture.channels = CHANNEL;
    config.sampleRate       = SAMPLE_RATE;
    config.dataCallback = miniaudioCallback;

    ma_result r = ma_device_init(NULL, &config, &device);
    if (r != MA_SUCCESS) {
        throw std::runtime_error("initiate device unsuccessful");
    }
}

Sound& Sound::instance() {
    static Sound inst;
    return inst;
}

void miniaudioCallback(
    ma_device * device,
    void * output,
    const void * input,
    ma_uint32 frameCount
) {
    // don't actually want to spend time initiating things,
    // shouldn't happen anyways
    if (!Sound::initiated)
        return;

    Sound& sound = Sound::instance();
    for (int i = 0; i < frameCount; i++) {
        
    }
}


