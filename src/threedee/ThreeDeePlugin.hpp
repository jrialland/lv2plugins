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
 *      {"portType":"AUDIO_OUT", "symbol":"out_right", "name":"Audio Out Right"}
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