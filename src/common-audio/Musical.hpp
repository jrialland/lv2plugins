#pragma once

#include <string>
#include <vector>

class Musical {

public:
  struct Note {
    int id;
    double freq;
    Note(int id, double freq);
    Note(const Note &n);
  };

  /**
   * The names of the known scales
   */
  static const std::vector<std::string> &getScales();

  /**
   * The names of the 12 tones in an octave
   */
  static const std::vector<std::string> &getTones();

  static const std::vector<Musical::Note> &getNotes(const std::string &scale,
                                                    const std::string &tone);

  static Note getNearestNote(const std::string &scale, const std::string &tone,
                             double freq);
};