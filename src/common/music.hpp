#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>
using namespace std;

#define DEFAULT_PITCH_OF_A3 440.0

namespace music {

////////////////////////////////////////////////////////////////////////////////
/**
 * Names of the different tones that form an octave
 */
class Key {

private:
  int id;

  string name;

  Key(int id, const string &name);

public:
  static const Key A;
  static const Key ASharp;
  static const Key B;
  static const Key C;
  static const Key CSharp;
  static const Key D;
  static const Key DSharp;
  static const Key E;
  static const Key F;
  static const Key FSharp;
  static const Key G;
  static const Key GSharp;

  string get_name() const;

  int ordinal() const;

  static const vector<const Key *> AllKeys;

  string str() const;
};

////////////////////////////////////////////////////////////////////////////////
class Note {

private:
  int id_;

  double freq_;

public:
  Note(int id, double freq);

  int id() const;

  int octave() const;

  const Key *key() const;

  double freq() const;

  string str() const;

  bool operator<(const Note &other) const;
};

ostream &operator<<(ostream &s, const Note &note);

////////////////////////////////////////////////////////////////////////////////
/*
 * Defines the pitch relation between note
 */
class Temperament {

private:
  function<double(const Key *key)> ratioFn;

  Temperament(function<double(const Key *key)> ratioFn);

public:
  double frequency(int n, double pitch_a = DEFAULT_PITCH_OF_A3) const;

  Note note(double freq, double pitch_a = DEFAULT_PITCH_OF_A3) const;

  static const Temperament Equal;

  static const Temperament PerfectFifths;

  static const Temperament JustIntonation;
};

////////////////////////////////////////////////////////////////////////////////
enum Interval {
  Unison = 0,
  MinorSecond = 1,
  MajorSecond = 2,
  MinorThird = 3,
  MajorThird = 4,
  Fourth = 5,
  Tritone = 6,
  Fifth = 7,
  MinorSixth = 8,
  MajorSixth = 9,
  MinorSeventh = 10,
  MajorSeventh = 11,
  Octave = 12
};

////////////////////////////////////////////////////////////////////////////////
class Scale {

  const string name;

  vector<Interval> intervals;

  Scale(const string &name, vector<Interval> intervals);

  friend class ScaleInKey;

public:
  static const Scale Chromatic;

  static const Scale Diatonic;

  static const Scale PentatonicMajor;

  static const Scale PentatonicMinor;

  static const Scale Ionian;

  static const Scale Dorian;

  static const Scale Phrygian;

  static const Scale Lydian;

  static const Scale Mixolydian;

  static const Scale Aeolian;

  static const Scale Locrian;

  static const Scale Zalzaian;

  static const Scale HarmonicMinor;
};

class ScaleInKey {

private:
  const Scale &scale_;

  const Key &key_;

  double highest;

  map<double, Note> mnotes;

  vector<double> freqs;

public:
  ScaleInKey(const Temperament &temperament, const Scale &scale, const Key &key,
             double pitch_a = DEFAULT_PITCH_OF_A3);

  Note nearest_note(double freq) const;
};

} // namespace music
