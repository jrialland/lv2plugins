#include "AutoPitchPlugin.hpp"
#include "common/music.hpp"
using namespace music;

#include<iostream>
using namespace std;

AutoPitchPlugin::AutoPitchPlugin() : Lv2Plugin() {
    pitchDetector = nullptr; //is initialized when set_sample_rate is called
    pitchShifter = new PitchShifter();
}

void AutoPitchPlugin::set_sample_rate(double rate) {
    Lv2Plugin::set_sample_rate(rate);
    if(pitchDetector) {
        delete pitchDetector;
    }
    pitchDetector = new PitchDetector((int)rate);
}

AutoPitchPlugin::~AutoPitchPlugin() {
    if(pitchDetector) {
        delete pitchDetector;
    }
    delete pitchShifter;
}

void AutoPitchPlugin::activate() {
    correction = 1.0;
}

void AutoPitchPlugin::run(uint32_t nsamples) {
    float* input = get_buffer<float>(0);
    float* output = get_buffer<float>(1);
    cout << "input " << input << endl;
    cout << "output " << output << endl;
    
    ScaleInKey sk(Temperament::Equal, Scale::Ionian, Key::F);
    
    float pitch = pitchDetector->detect_pitch(nsamples, input);

    if(pitch > 0) {
        // find the nearest note that is in the current scale
        Note target = sk.nearest_note(pitch);
        correction = target.freq() / pitch;
    }
    pitchShifter->shift(nsamples, input, output, correction);
}