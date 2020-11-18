#pragma once
#include <cstdint>

class PitchDetector {

    uint32_t sample_rate;

    float* buffer;

    uint32_t buffer_size;

    float* get_buffer(uint32_t needed);

    public:

    PitchDetector(uint32_t sample_rate);

    ~PitchDetector();

    float detect_pitch(uint32_t nsamples, float* input);
};