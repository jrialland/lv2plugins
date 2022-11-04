#pragma once
#include "common-audio/Lv2AudioPlugin.hpp"
#include <AL/al.h>
#include <AL/alc.h>

/**
 * Sound spatializer
 *
 * @lv2cpp.meta {
 *    "uri" : "https://github.com/jrialland/lv2plugins/threedee",
 *    "name" : "ThreeDee",
 *    "pluginType" : "AUDIO",
 *    "description" : "Sound spatializer",
 *    "ports" : [
 *      {"portType":"AUDIO_IN", "symbol":"in", "name":"Audio In"},
 *      {"portType":"AUDIO_OUT", "symbol":"out_left", "name":"Audio Out Left"},
 *      {"portType":"AUDIO_OUT", "symbol":"out_right", "name":"Audio Out Right"},
 *      {"portType":"CONTROL_IN", "symbol":"radius", "name":"Radius", "minimum":0, "default":1, "maximum":5},
 *      {"portType":"CONTROL_IN", "symbol":"angle", "name":"Angle", "minimum":-6.28318, "default":0, "maximum":6.28318}
 *    ],
 *    "ui" : "https://github.com/jrialland/lv2plugins/autopitch#UI"
 * }
 */
class ThreeDeePlugin : public lv2cpp::Lv2AudioPlugin
{

    float fConst0, fConst1, fRec0[2], fRec1[2];

    float radius = 1.0;

    float angle = 0;

public:

    void set_sample_rate(double rate) override;

    void run(uint32_t nsamples) override;

    void setPosition(float radius, float angle);
};

#define THREEDEE_CHANNEL_IN 0
#define THREEDEE_CHANNEL_OUT_LEFT 1
#define THREEDEE_CHANNEL_OUT_RIGHT 2
#define THREEDEE_CONTROL_RADIUS 3
#define THREEDEE_CONTROL_ANGLE 4