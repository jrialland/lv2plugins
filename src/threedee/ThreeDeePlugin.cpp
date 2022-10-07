#include <cstring>
#include <stdexcept>
#include <iostream>
#include <sstream>

#include "ThreeDeePlugin.hpp"

EXPORT_AUDIO_PLUGIN("https://github.com/jrialland/lv2plugins/threedee",
                    ThreeDeePlugin);

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
    float azimuth = get_port_value<THREEDEE_CONTROL_AZIMUTH>();
    float elevation = get_port_value<THREEDEE_CONTROL_AZIMUTH>();
    float distance = get_port_value<THREEDEE_CONTROL_AZIMUTH>();

    // detect if there is a position change
    if(azimuth != position.fAzimuth || elevation != position.fElevation || distance != position.fDistance) {
        setPosition(azimuth, elevation, distance);
    }

    float *input_buffer = get_buffer<THREEDEE_CHANNEL_IN>();
    // Encode signal into BFormat buffer
    encoder.Process(input_buffer, nsamples, &fmt);

    float *output_buffers[] = {
        get_buffer<THREEDEE_CHANNEL_OUT_LEFT>(),
        get_buffer<THREEDEE_CHANNEL_OUT_RIGHT>()
    };
    
    // Decode to get the speaker feeds
    decoder.Process(&fmt, nsamples, output_buffers);
}

ThreeDeePlugin::~ThreeDeePlugin()
{
}