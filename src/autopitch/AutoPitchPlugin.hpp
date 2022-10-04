#pragma once

#include "PitchDetector.hpp"
#include "PitchShifter.hpp"
#include "common-audio/Lv2AudioPlugin.hpp"

/**
 * This audio plugin detects the pitch of a voice and tries to
 * modify it in order to correspond to a note in a scale
 *
 * @lv2cpp.meta {
 *    "uri" : "https://github.com/jrialland/lv2plugins/autopitch",
 *    "name" : "AutoPitch",
 *    "pluginType" : "AUDIO",
 *    "description" : "Automatic pitch correction",
 *    "ports" : [
 *      {"portType":"AUDIO_IN", "symbol":"in", "name":"Audio In"},
 *      {"portType":"AUDIO_OUT", "symbol":"out", "name":"Audio Out"}
 *    ],
 *    "ui" : "https://github.com/jrialland/lv2plugins/autopitch#UI"
 * }
 */
class AutoPitchPlugin : public lv2cpp::Lv2AudioPlugin {

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
