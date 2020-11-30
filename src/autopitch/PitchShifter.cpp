#include "PitchShifter.hpp"

#include <algorithm>
#include <cmath>

#include <iostream>
using namespace std;

PitchShifter::PitchShifter() {
  float vec0[131072];
  for (int i = 0; i < sizeof(vec0) / sizeof(vec0[0]); i++) {
    vec0[i] = 0.0f;
  }
  for (int i = 0; i < sizeof(rec0) / sizeof(rec0[0]); i++) {
    rec0[i] = 0.0f;
  }
}

void PitchShifter::shift(uint32_t nsamples, float *input, float *output,
                         float ratio) {

  // algorithm extracted from source code generated by FAUST
  // (https://faust.grame.fr)

  float window = nsamples * 4;
  float fSlow2 = .1f; // crossfade
  for (int i = 0; i < nsamples; i++) {
    float fTemp0 = float(input[i]);
    vec0[(iota & 131071)] = fTemp0;
    rec0[0] = std::fmod((window + ((rec0[1] + 1.0f) - ratio)), window);
    int iTemp1 = int(rec0[0]);
    float fTemp2 = std::floor(rec0[0]);
    float fTemp3 = (1.0f - rec0[0]);
    float fTemp4 = std::min<float>((fSlow2 * rec0[0]), 1.0f);
    float fTemp5 = (window + rec0[0]);
    int iTemp6 = int(fTemp5);
    float fTemp7 = std::floor(fTemp5);
    output[i] =
        ((((vec0[((iota - std::min<int>(65537, std::max<int>(0, iTemp1))) &
                  131071)] *
            (fTemp2 + fTemp3)) +
           ((rec0[0] - fTemp2) *
            vec0[(
                (iota - std::min<int>(65537, std::max<int>(0, (iTemp1 + 1)))) &
                131071)])) *
          fTemp4) +
         (((vec0[((iota - std::min<int>(65537, std::max<int>(0, iTemp6))) &
                  131071)] *
            ((fTemp7 + fTemp3) - window)) +
           ((window + (rec0[0] - fTemp7)) *
            vec0[(
                (iota - std::min<int>(65537, std::max<int>(0, (iTemp6 + 1)))) &
                131071)])) *
          (1.0f - fTemp4)));
    iota++;
    rec0[1] = rec0[0];
  }
}