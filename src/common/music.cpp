
#include "music.hpp"

#include <cmath>
#include <limits>
#include <sstream>
#include <map>
#include <algorithm>
#include <iostream>
using namespace std;

namespace music {

////////////////////////////////////////////////////////////////////////////////
Key::Key(int _id, const string &_name) : id(_id), name(_name) {
}

string Key::str() const {
    return name;
}

int Key::ordinal() const {
    return id;
}

////////////////////////////////////////////////////////////////////////////////
const Key Key::A(0, "A");
const Key Key::ASharp(1, "A#");
const Key Key::B(2, "B");
const Key Key::C(3, "C");
const Key Key::CSharp(4, "C#");
const Key Key::D(5, "D");
const Key Key::DSharp(6, "D#");
const Key Key::E(7,"E");
const Key Key::F(8,"F");
const Key Key::FSharp(9,"F#");
const Key Key::G(10, "G");
const Key Key::GSharp(11, "G#");

////////////////////////////////////////////////////////////////////////////////
const vector<const Key*> Key::AllKeys = {
    &Key::A,
    &Key::ASharp,
    &Key::B,
    &Key::C,
    &Key::CSharp,
    &Key::D,
    &Key::DSharp,
    &Key::E,
    &Key::F,
    &Key::FSharp,
    &Key::G,
    &Key::GSharp
};

////////////////////////////////////////////////////////////////////////////////
Note::Note(int id, double freq) : id_(id), freq_(freq) {
}

////////////////////////////////////////////////////////////////////////////////
int Note::octave() const {
    //A-1 is the lowest note; octave 0 begins with C, so the lowest possible note is A-1
    if(id_ < 3) {
        return -1;
    } else {
        return (id_ - 3) / 12; 
    }
}

////////////////////////////////////////////////////////////////////////////////
const Key* Note::key() const {
    return Key::AllKeys[id_%12];
}

////////////////////////////////////////////////////////////////////////////////
std::string Note::str() const {
    ostringstream ss;
    ss << key()->str() << octave() << " (" << freq_ << " Hz)";
    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////
double Note::freq() const {
    return freq_;
}

int Note::id() const {
    return id_;
}

////////////////////////////////////////////////////////////////////////////////
ostream& operator<<(ostream& s, const Note& note) {
    s << note.str();
    return s;
}

////////////////////////////////////////////////////////////////////////////////
bool Note::operator<(const Note& other) const {
    return freq_ < other.freq_;
}

////////////////////////////////////////////////////////////////////////////////
Temperament::Temperament(function<double(const Key* key)> _ratioFn) : ratioFn(_ratioFn) {
}

////////////////////////////////////////////////////////////////////////////////
static const double eqRatio = pow(2, 1/12.0);

////////////////////////////////////////////////////////////////////////////////
const Temperament Temperament::Equal([](const Key* key) -> double {
    return pow(eqRatio, key->ordinal());
});

/*
 * hardcoded ratios from : Serge Cordier, "Piano bien tempéré et justesse orchestrale, éd. Buchet-Chastel, 1982"
 * (https://fr.wikipedia.org/wiki/Temp%C3%A9rament_%C3%A9gal_%C3%A0_quintes_justes#Application_au_piano)
 */
const Temperament Temperament::PerfectFifths([](const Key* key) -> double {
    switch(key->ordinal()) {
        case 0: return 1.0;
        case 1: return 1.059634;
        case 2: return 1.122824;
        case 3: return 1.189782;
        case 4: return 1.260734;
        case 5: return 1.335916;
        case 6: return 1.415582;
        case 7: return 1.5;
        case 8: return 1.589451;
        case 9: return 1.684236;
        case 10: return 1.784674;
        case 11: return 1.891101;
    }
    throw std::runtime_error("undefined key");
});

////////////////////////////////////////////////////////////////////////////////
const Temperament Temperament::JustIntonation([](const Key* key) -> double {
    switch (key->ordinal()) {
        case 0: return 1.0;
        case 1: return 16.0d / 15;
        case 2: return 9.0d / 8;
        case 3: return 6.0d / 5;
        case 4: return 5.0d / 4;
        case 5: return 4.0d / 3;
        case 6: return 7.0d / 5;
        case 7: return 3.0d / 2;
        case 8: return 5.0d / 3;
        case 9: return 8.0d / 5;
        case 10: return 9.0d / 5;
        case 11: return 15.0d / 8;
    }
    throw std::runtime_error("undefined key");
});


////////////////////////////////////////////////////////////////////////////////
Scale::Scale(const string &name_, vector<Interval> intervals_) : name(name_), intervals(intervals_) {
}


const Scale Scale::Chromatic("chromatic", vector<Interval>{Unison, MinorSecond, MajorSecond, MinorThird, MajorThird, Fourth, Fifth, MinorSixth, MajorSixth, MinorSeventh, MajorSeventh});
const Scale Scale::Diatonic("diatonic", vector<Interval>{Unison, MajorSecond, MinorThird, Fourth, Fifth, MinorSixth, MinorSeventh});
const Scale Scale::PentatonicMajor(" pentatonic major", vector<Interval>{Unison, MajorSecond, MajorThird, Fifth, MajorSixth});
const Scale Scale::PentatonicMinor("pentatonic minor", vector<Interval>{Unison, MinorThird, Fourth, Fifth, MinorSeventh});
const Scale Scale::Ionian("ionian mode", vector<Interval>{Unison, MajorSecond, MajorThird, Fourth, Fifth, MajorSixth, MajorSeventh});
const Scale Scale::Dorian("dorian mode", vector<Interval>{Unison, MajorSecond, MinorThird, Fourth, Fifth, MajorSixth, MinorSeventh});
const Scale Scale::Phrygian("phrygian mode", vector<Interval>{Unison, MinorSecond, MinorThird, Fourth, Fifth, MinorSixth, MinorSeventh});
const Scale Scale::Lydian("lydian mode", vector<Interval>{Unison, MajorSecond, MajorThird, Tritone, Fifth, MajorSixth ,MajorSeventh});
const Scale Scale::Mixolydian("mixolydian mode", vector<Interval>{Unison, MajorSecond, Fourth, Fifth ,MajorSixth, MinorSeventh});
const Scale Scale::Aeolian("aeolian mode", vector<Interval>{Unison, MajorSecond, MinorThird, Fourth, Fifth, MinorSixth, MinorSeventh});
const Scale Scale::Locrian("locrian mode", vector<Interval>{Unison, MinorSecond, MinorThird, Fourth, Tritone ,MinorSixth, MinorSeventh});
const Scale Scale::Zalzaian("zalzaian", vector<Interval>({Unison, MinorSecond, MinorThird, Fourth, Fifth, MinorSixth, MajorSixth}));
const Scale Scale::HarmonicMinor("harmonic minor", vector<Interval>({Unison,MajorSecond,MinorThird,Fourth,Fifth,MinorSixth}));

////////////////////////////////////////////////////////////////////////////////
double Temperament::frequency(int n, double pitch_a) const{
    int octave = n / 12;
    double f = (pitch_a/8) * pow(2, octave-1);
    return f * ratioFn(Key::AllKeys[n%12]);
}

Note Temperament::note(double freq, double pitch_a) const {
    double a = pitch_a/8;
    int octave = 0;
    if(freq < a) {
        return Note{0, a};
    }
    while(a < freq) {
        a *= 2;
        octave += 1;
    };
    a/=2;
    double bestdiff = std::numeric_limits<float>::infinity();
    double bestratio = 1.0;
    int best_i = 0;
    for(int i=0; i < 11; i++) {
        double ratio = ratioFn(Key::AllKeys[i]);
        double diff = abs(a * ratio - freq);
        if(diff < bestdiff) {
            bestdiff = diff;
            bestratio = ratio;
            best_i = i;
        }
        if(diff > bestdiff) {
            break;
        }
    }
    return Note{ octave * 12 + best_i, a * bestratio};

}

////////////////////////////////////////////////////////////////////////////////
ScaleInKey::ScaleInKey(const Temperament& temperament, const Scale& scale, const Key& key, double pitch_a)  : scale_(scale), key_(key) {
    // for each octave, starting with A0
    for(auto interval : scale.intervals) {
        int id = key.ordinal() + interval;
        double freq = temperament.frequency(id, pitch_a);
        if(id - 12 >= 0) {
            float f = temperament.frequency(id - 12, pitch_a);
            mnotes.emplace(f, Note{id - 12, f});
            freqs.push_back(f);
        }
        while(id < 88) { // highest note = C7
            mnotes.emplace(freq, Note{id, freq});
            freqs.push_back(freq);
            id += 12;
            freq *= 2;
        }
    }
    std::sort(freqs.begin(), freqs.end());
    highest = freqs.back();
}

////////////////////////////////////////////////////////////////////////////////
Note ScaleInKey::nearest_note(double freq) const {
    if(freq >= highest) {
        return mnotes.find(highest)->second;
    }
    auto it = std::lower_bound(freqs.begin(), freqs.end(), freq);
    return mnotes.find(*it)->second;
}

}

