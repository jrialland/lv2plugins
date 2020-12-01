#include "Musical.hpp"

#include <algorithm>
#include <cmath>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

struct ScaleDef {

  string name;

  vector<int> intervals;

  ScaleDef(string &&name_, const vector<int> &&intervals_)
      : name(name_), intervals(intervals_) {}
};

struct ScaleDefs {

  vector<ScaleDef> vec;

  template <class... Rest> ScaleDefs(Rest... rest) { add(rest...); }

  template <class... Rest> void add(ScaleDef &n, Rest... rest) {
    vec.emplace_back(n);
    add(rest...);
  }

  void add() const {}

  vector<string> names() const {
    vector<string> v;
    for (auto sd : vec) {
      v.emplace_back(sd.name);
    }
    return v;
  }

  map<string, vector<int>> intervals() const {
    map<string, vector<int>> m;
    for (auto &sd : vec) {
      m.emplace(sd.name, sd.intervals);
    }
    return m;
  }
};

static const ScaleDefs scaleDefs(
    ScaleDef("octave", {0}), ScaleDef("Major", {0, 4, 7}),
    ScaleDef("Majb5", {0, 4, 6}), ScaleDef("minor", {0, 3, 7}),
    ScaleDef("minb5", {0, 3, 6}), ScaleDef("sus2", {0, 2, 7}),
    ScaleDef("sus4", {0, 5, 7}), ScaleDef("aug", {0, 4, 8}),
    ScaleDef("augsus4", {0, 5, 8}), ScaleDef("tri", {0, 3, 6, 9}),
    ScaleDef("6", {0, 4, 7, 9}), ScaleDef("6sus4", {0, 5, 7, 9}),
    ScaleDef("6add9", {0, 4, 7, 9, 14}), ScaleDef("m6", {0, 3, 7, 9}),
    ScaleDef("m6add9", {0, 3, 7, 9, 14}), ScaleDef("7", {0, 4, 7, 10}),
    ScaleDef("7sus4", {0, 5, 7, 10}), ScaleDef("7#5", {0, 4, 8, 10}),
    ScaleDef("7b5", {0, 4, 6, 10}), ScaleDef("7#9", {0, 4, 7, 10, 15}),
    ScaleDef("7b9", {0, 4, 7, 10, 13}), ScaleDef("7#5#9", {0, 4, 8, 10, 15}),
    ScaleDef("7#5b9", {0, 4, 8, 10, 13}), ScaleDef("7b5b9", {0, 4, 6, 10, 13}),
    ScaleDef("7add11", {0, 4, 7, 10, 17}),
    ScaleDef("7add13", {0, 4, 7, 10, 21}), ScaleDef("7#11", {0, 4, 7, 10, 18}),
    ScaleDef("Maj7", {0, 4, 7, 11}), ScaleDef("Maj7b5", {0, 4, 6, 11}),
    ScaleDef("Maj7#5", {0, 4, 8, 11}), ScaleDef("Maj7#11", {0, 4, 7, 11, 18}),
    ScaleDef("Maj7add13", {0, 4, 7, 11, 21}), ScaleDef("m7", {0, 3, 7, 10}),
    ScaleDef("m7b5", {0, 3, 6, 10}), ScaleDef("m7b9", {0, 3, 7, 10, 13}),
    ScaleDef("m7add11", {0, 3, 7, 10, 17}),
    ScaleDef("m7add13", {0, 3, 7, 10, 21}), ScaleDef("m-Maj7", {0, 3, 7, 11}),
    ScaleDef("m-Maj7add11", {0, 3, 7, 11, 17}),
    ScaleDef("m-Maj7add13", {0, 3, 7, 11, 21}),
    ScaleDef("9", {0, 4, 7, 10, 14}), ScaleDef("9sus4", {0, 5, 7, 10, 14}),
    ScaleDef("add9", {0, 4, 7, 14}), ScaleDef("9#5", {0, 4, 8, 10, 14}),
    ScaleDef("9b5", {0, 4, 6, 10, 14}), ScaleDef("9#11", {0, 4, 7, 10, 14, 18}),
    ScaleDef("9b13", {0, 4, 7, 10, 14, 20}),
    ScaleDef("Maj9", {0, 4, 7, 11, 14}),
    ScaleDef("Maj9sus4", {0, 5, 7, 11, 15}),
    ScaleDef("Maj9#5", {0, 4, 8, 11, 14}),
    ScaleDef("Maj9#11", {0, 4, 7, 11, 14, 18}),
    ScaleDef("m9", {0, 3, 7, 10, 14}), ScaleDef("madd9", {0, 3, 7, 14}),
    ScaleDef("m9b5", {0, 3, 6, 10, 14}), ScaleDef("m9-Maj7", {0, 3, 7, 11, 14}),
    ScaleDef("11", {0, 4, 7, 10, 14, 17}),
    ScaleDef("11b9", {0, 4, 7, 10, 13, 17}),
    ScaleDef("Maj11", {0, 4, 7, 11, 14, 17}),
    ScaleDef("m11", {0, 3, 7, 10, 14, 17}),
    ScaleDef("m-Maj11", {0, 3, 7, 11, 14, 17}),
    ScaleDef("13", {0, 4, 7, 10, 14, 21}),
    ScaleDef("13#9", {0, 4, 7, 10, 15, 21}),
    ScaleDef("13b9", {0, 4, 7, 10, 13, 21}),
    ScaleDef("13b5b9", {0, 4, 6, 10, 13, 21}),
    ScaleDef("Maj13", {0, 4, 7, 11, 14, 21}),
    ScaleDef("m13", {0, 3, 7, 10, 14, 21}),
    ScaleDef("m-Maj13", {0, 3, 7, 11, 14, 21}),
    ScaleDef("Major", {0, 2, 4, 5, 7, 9, 11}),
    ScaleDef("Harmonic minor", {0, 2, 3, 5, 7, 8, 11}),
    ScaleDef("Melodic minor", {0, 2, 3, 5, 7, 9, 11}),
    ScaleDef("Whole tone", {0, 2, 4, 6, 8, 10}),
    ScaleDef("Diminished", {0, 2, 3, 5, 6, 8, 9, 11}),
    ScaleDef("Major pentatonic", {0, 2, 4, 7, 9}),
    ScaleDef("Minor pentatonic", {0, 3, 5, 7, 10}),
    ScaleDef("Jap in sen", {0, 1, 5, 7, 10}),
    ScaleDef("Major bebop", {0, 2, 4, 5, 7, 8, 9, 11}),
    ScaleDef("Dominant bebop", {0, 2, 4, 5, 7, 9, 10, 11}),
    ScaleDef("Blues", {0, 3, 5, 6, 7, 10}),
    ScaleDef("Arabic", {0, 1, 4, 5, 7, 8, 11}),
    ScaleDef("Enigmatic", {0, 1, 4, 6, 8, 10, 11}),
    ScaleDef("Neopolitan", {0, 1, 3, 5, 7, 9, 11}),
    ScaleDef("Neopolitan minor", {0, 1, 3, 5, 7, 8, 11}),
    ScaleDef("Hungarian minor", {0, 2, 3, 6, 7, 8, 11}),
    ScaleDef("Dorian", {0, 2, 3, 5, 7, 9, 10}),
    ScaleDef("Phrygian", {0, 1, 3, 5, 7, 8, 10}),
    ScaleDef("Lydian", {0, 2, 4, 6, 7, 9, 11}),
    ScaleDef("Mixolydian", {0, 2, 4, 5, 7, 9, 10}),
    ScaleDef("Aeolian", {0, 2, 3, 5, 7, 8, 10}),
    ScaleDef("Locrian", {0, 1, 3, 5, 6, 8, 10}),
    ScaleDef("Minor", {0, 2, 3, 5, 7, 8, 10}),
    ScaleDef("Chromatic", {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}),
    ScaleDef("Half-Whole Diminished", {0, 1, 3, 4, 6, 7, 9, 10}),
    ScaleDef("5", {0, 7}),
    ScaleDef("Phrygian dominant", {0, 1, 4, 5, 7, 8, 10}),
    ScaleDef("Persian", {0, 1, 4, 5, 6, 8, 11}));

static const vector<string> scales = scaleDefs.names();

static const map<string, vector<int>> intervals = scaleDefs.intervals();

static const vector<string> tones = {"A",     "A#/Bb", "B",     "C",
                                     "C#/Db", "D",     "D#/Eb", "E",
                                     "F",     "F#/Gb", "G",     "G#/Ab"};

static const map<string, int> toneIds = {
    {"A", 0},  {"A#", 1}, {"Bb", 1}, {"B", 2},   {"C", 3},  {"C#", 4},
    {"Db", 4}, {"D", 5},  {"D#", 6}, {"Eb", 6},  {"E", 7},  {"F", 8},
    {"F#", 9}, {"Gb", 9}, {"G", 10}, {"G#", 11}, {"Ab", 11}};

/**
 * Will receive all the valid notes for each key/scale couples
 */
static map<string, vector<Musical::Note>> notes;

Musical::Note::Note(int id_, double freq_) : id(id_), freq(freq_) {}

Musical::Note::Note(const Note &n) : id(n.id), freq(n.freq) {}

string Musical::Note::name() const {
  auto n = tones[id % 12];
  if (n.find('/' != string::npos)) {
    n = n.substr(0, 2);
  }
  int octave = id < 3 ? -1 : ((id - 3) / 12);
  return n + to_string(octave);
}

ostream &operator<<(ostream &os, const Musical::Note &n) {
  os << n.name();
  return os;
}

/**
 * For a given note name, return the index of the corresponding
 * key on a piano keyboard (lowest octave, starting with id0 = A-1 = 27.5 Hz)
 */
static inline int getIndexOfTone(const std::string &tone) {
  auto it = toneIds.find(tone.substr(0, 2));
  if (it != toneIds.end()) {
    return it->second;
  }
  throw std::invalid_argument("invalid tone : '" + tone + "'");
}

/**
 * Fills the 'notes' dictionary for every notes for a given key/scale
 */
static inline const vector<Musical::Note> &feedNotes(const string &name, int id,
                                                     const vector<int> &itvs) {
  static const double ratio = pow(2, 1.0 / 12);
  vector<Musical::Note> v;

  for (int interval : itvs) {
    int currentId = id - 12 + interval;
    if (currentId >= 0) {
      double freq = 27.5 * pow(ratio, currentId);
      v.emplace_back(currentId, freq);
    }
  }

  while (id < 89) {
    for (int interval : itvs) {
      int currentId = id + interval;
      if (currentId < 88) {
        double freq = 27.5 * pow(ratio, currentId);
        v.emplace_back(currentId, freq);
      }
    }
    id += 12;
  }
  return notes.emplace(name, v).first->second;
}

const std::vector<Musical::Note> &Musical::getNotes(const std::string &scale,
                                                    const std::string &tone) {
  int id = getIndexOfTone(tone);
  string k = scale + "/" + std::to_string(id);
  auto itNotes = notes.find(k);
  if (itNotes == notes.end()) {
    // find the scale
    auto it = intervals.find(scale);
    if (it == intervals.end()) {
      throw std::invalid_argument(string("unknown scale : '") + scale + "'");
    }
    return feedNotes(k, id, it->second);
  } else {
    return itNotes->second;
  }
}

Musical::Note Musical::getNote(const std::string& name) {
    auto vec = getNotes("Chromatic", name);
    for(auto n : vec) {
        if(n.name().compare(name) == 0) {
            return n;
        }
    }
    throw std::invalid_argument(string("unknown note : '") + name + "'");
}

Musical::Note Musical::getNearestNote(const std::vector<Musical::Note> &notes,
                                      double freq) {
  Musical::Note searched{-1, freq};
  // find the first note that is higher than the given freq
  auto it = std::lower_bound(
      notes.begin(), notes.end(), searched,
      [](const Musical::Note &n1, const Musical::Note &n2) -> bool {
        return n1.freq < n2.freq;
      });

  // if the preceding note is nearer, select it
  if (it != notes.begin()) {
    double diff = it->freq - freq;
    it--;
    double diffprev = freq - it->freq;
    if (diffprev > diff) {
      it++;
    }
  }
  return *it;
}

Musical::Note Musical::getNearestNote(const std::string &scale,
                                      const std::string &tone, double freq) {
  const auto &validNotes = Musical::getNotes(scale, tone);
  return getNearestNote(validNotes, freq);
}

const std::vector<std::string> &Musical::getScales() { return scales; }

const std::vector<std::string> &Musical::getTones() { return tones; }