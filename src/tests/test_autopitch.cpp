#include <catch2/catch_all.hpp>
#include "plugin_loader.hpp"
#include <cmath>

#include <iostream>
using namespace std;

#define SAMPLING_RATE 48000
#define NSAMPLES 500

TEST_CASE("autopitch run", "[autopitch]")
{
    auto plugin = loadPlugin("./autopitch.lv2/libautopitch.so");
    float input_buffer[NSAMPLES];
    float output_buffer[NSAMPLES] = {.0};

    for (size_t i = 0; i < NSAMPLES; i += 1)
    {
        input_buffer[i] = sin(440.0 * 2 * M_PI * i / SAMPLING_RATE);
    }

    plugin->set_sample_rate(SAMPLING_RATE);
    plugin->_add_port_(0, input_buffer);
    plugin->_add_port_(1, output_buffer);
    plugin->activate();
    plugin->_run_(NSAMPLES);
    plugin->deactivate();
}