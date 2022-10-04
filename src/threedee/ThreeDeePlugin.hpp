#include "common-audio/Lv2AudioPlugin.hpp"
#include <AmbisonicEncoder.h>
#include <AmbisonicDecoder.h>

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
 *      {"portType":"CONTROL_IN", "symbol":"azimuth", "name":"Azimuth", "minimum":-180, "default":0, "maximum":180},
 *      {"portType":"CONTROL_IN", "symbol":"elevation", "name":"Elevation", "minimum":-100, "default":0, "maximum":100},
 *      {"portType":"CONTROL_IN", "symbol":"distance", "name":"Distance", "minimum":0, "default":0, "maximum":100}
 *    ],
 *    "ui" : "https://github.com/jrialland/lv2plugins/autopitch#UI"
 * }
 */
class ThreeDeePlugin : public lv2cpp::Lv2AudioPlugin {

    private:

    CBFormat fmt;
    CAmbisonicEncoder encoder;
    CAmbisonicDecoder decoder;
    PolarPoint position;

    public:

        ThreeDeePlugin();

        ~ThreeDeePlugin();

        void run(uint32_t nsamples) override;
        
        void setPosition(float azimuth, float elevation, float distance);
};

#define THREEDEE_CHANNEL_IN 0
#define THREEDEE_CHANNEL_OUT_LEFT 1
#define THREEDEE_CHANNEL_OUT_RIGHT 2
#define THREEDEE_CONTROL_AZIMUTH 3
#define THREEDEE_CONTROL_ELEVATION 4
#define THREEDEE_CONTROL_DISTANCE 5
