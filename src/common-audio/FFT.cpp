#include "FFT.hpp"

extern "C" {
#define STB_FFT_IMPLEMENTAION
#include <sbt_fft/sbt_fft.h>
}

FFT::~FFT() {
  if (im != nullptr) {
    delete[] im;
  }
}

void FFT::fft(int nsamples, float *input) {
  if (im == nullptr || nsamples != n) {
    im = new cmplx[nsamples / 2];
    n = nsamples;
  }
  STB_FFT_R2C(input, im, nsamples);
}

void FFT::ifft(float *output) { STB_IFFT_C2R(im, output, n); }
