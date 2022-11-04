#include <catch2/catch_all.hpp>
#include <cmath>
#include "threedee/ThreeDeePlugin.hpp"
#include "plugin_loader.hpp"

#include <iostream>
using namespace std;

#define SAMPLING_RATE 48000
#define NSAMPLES 500

TEST_CASE("threedee run", "[threedee]")
{
    /*
    auto plugin = loadPlugin("./threedee.lv2/libthreedee.so");

    float input_buffer[NSAMPLES];

    float out_left_buffer[NSAMPLES] = {.0};
    float out_right_buffer[NSAMPLES] = {.0};
    float angle = 0;
    float radius = 1.0;

    for (size_t i = 0; i < NSAMPLES; i += 1)
    {
        input_buffer[i] = sin(440.0 * 2 * M_PI * i / SAMPLING_RATE);
    }

    plugin->set_sample_rate(SAMPLING_RATE);
    plugin->_add_port_(THREEDEE_CHANNEL_IN, input_buffer);
    plugin->_add_port_(THREEDEE_CHANNEL_OUT_LEFT, out_left_buffer);
    plugin->_add_port_(THREEDEE_CHANNEL_OUT_RIGHT, out_right_buffer);
    plugin->_add_port_(THREEDEE_CONTROL_ANGLE, &angle);
    plugin->_add_port_(THREEDEE_CONTROL_RADIUS, &radius);

    plugin->activate();
    plugin->_run_(NSAMPLES);
    plugin->deactivate();
    */
}