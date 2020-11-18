#pragma once
#include "common/Lv2Plugin.hpp"
using namespace lv2cpp;

#include "PitchDetector.hpp"
#include "PitchShifter.hpp"

class AutoPitchPlugin : public Lv2Plugin {
    
    PitchDetector *pitchDetector;
    
    PitchShifter *pitchShifter;

    float correction = 1.0;

    public:

        AutoPitchPlugin();

        ~AutoPitchPlugin();

        void set_sample_rate(double rate) override;

        void activate() override;

        void run(uint32_t nsamples) override;
};

DECLARE_PLUGIN("https://github.com/jrialland/lv2plugins/autopitch", AutoPitchPlugin);
PLUGIN_DESCRIPTION("https://github.com/jrialland/lv2plugins/autopitch", "Automatic pitch correction");
PLUGIN_ADD_PORT("https://github.com/jrialland/lv2plugins/autopitch", 0, "AUDIO_IN", "In")
PLUGIN_ADD_PORT("https://github.com/jrialland/lv2plugins/autopitch", 1, "AUDIO_OUT", "Out")
