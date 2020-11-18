#pragma once

#include <cstdint>

class PitchShifter {

	int iota = 0;

	float vec0[131072];
	
    float rec0[2];

    public:

        PitchShifter();

        void shift(uint32_t nsamples, float* input, float* output, float ratio);

};