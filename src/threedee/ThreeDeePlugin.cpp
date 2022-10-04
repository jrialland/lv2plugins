#include <cstring>
#include <stdexcept>
#include <iostream>
#include <sstream>

#include "ThreeDeePlugin.hpp"

ThreeDeePlugin::ThreeDeePlugin()
{
    encoder.Configure(1, true, 0);
    decoder.Configure(1, true, kAmblib_Stereo, 2);
    setPosition(0, 0, 0);
}

void ThreeDeePlugin::setPosition(float azimuth, float elevation, float distance)
{
    position.fAzimuth = 0;
    position.fElevation = 0;
    position.fDistance = 5;
    encoder.SetPosition(position);
    encoder.Refresh();
}

void ThreeDeePlugin::run(uint32_t nsamples)
{

    float *input_buffer = get_buffer<float>(0);
    // Encode signal into BFormat buffer
    encoder.Process(input_buffer, nsamples, &fmt);

    float *output_buffers[2];
    output_buffers[0] = get_buffer<float>(1); // left channel
    output_buffers[1] = get_buffer<float>(2); // right channel

    // Decode to get the speaker feeds
    decoder.Process(&fmt, 512, output_buffers);
}

ThreeDeePlugin::~ThreeDeePlugin()
{
}