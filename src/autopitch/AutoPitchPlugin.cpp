#include "AutoPitchPlugin.hpp"
#include "common-audio/Musical.hpp"

AutoPitchPlugin::AutoPitchPlugin() : lv2cpp::Lv2AudioPlugin()
{
  pitchDetector = nullptr; // is initialized when set_sample_rate is called
  pitchShifter = new PitchShifter();
}

void AutoPitchPlugin::set_sample_rate(double rate)
{
  lv2cpp::Lv2AudioPlugin::set_sample_rate(rate);
  if (pitchDetector)
  {
    delete pitchDetector;
  }
  pitchDetector = new PitchDetector((int)rate);
}

AutoPitchPlugin::~AutoPitchPlugin()
{
  if (pitchDetector)
  {
    delete pitchDetector;
  }
  delete pitchShifter;
}

void AutoPitchPlugin::activate() { correction = 1.0; }

void AutoPitchPlugin::run(uint32_t nsamples)
{

  float *input = get_buffer<AUTOPITCH_CHANNEL_IN>();
  float *output = get_buffer<AUTOPITCH_CHANNEL_OUT>();
/*
  float pitch = pitchDetector->detect_pitch(nsamples, input);

  if (pitch > 0)
  {
    // find the nearest note that is in the current scale
    Musical::Note target = Musical::getNearestNote("octave", "A", pitch);
    correction = target.freq / pitch;
  }
*/
  correction = 2;
  pitchShifter->shift(nsamples, input, output, correction);
}

EXPORT_AUDIO_PLUGIN("https://github.com/jrialland/lv2plugins/autopitch",
                    AutoPitchPlugin);