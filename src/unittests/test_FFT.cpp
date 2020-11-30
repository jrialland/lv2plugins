#include <catch2/catch_all.hpp>

#include "common-audio/FFT.hpp"
#include <cmath>
#include <iostream>
using namespace std;

TEST_CASE("", "[FFT]") {

  FFT fft;

  float *re = new float[1024];
  for (int i = 0; i < 1024; i++) {
    re[i] = sin(double(i) / 3);
  }

  fft.fft(1024, re);

  fft.ifft(re);
}