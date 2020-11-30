#pragma once

extern "C" {
#include <sbt_fft/sbt_fft.h>
}

class FFT {

  int n;

  cmplx *im = nullptr;

public:
  virtual ~FFT();

  void fft(int nsamples, float *input);

  void ifft(float *output);
};