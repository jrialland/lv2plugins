#pragma once

#include <iostream>
#include <string>
#include <vector>

class Musical {

public:
  struct Note {
    int id;
    double freq;
    Note(int id, double freq);
    Note(const Note &n);
    std::string name() const;
  };

  /**
   * @return The names of known scales
   */
  static const std::vector<std::string> &getScales();

  /**
   * @return The names of the 12 tones of an octave
   */
  static const std::vector<std::string> &getTones();

  static Note getNote(const std::string& name);

  /**
   * @return All the notes on a grand piano which belong to a particular scale
   */
  static const std::vector<Musical::Note> &getNotes(const std::string &scale,
                                                    const std::string &tone);

  /**
   * the array must be sorted by ascending frequencies
   * @return the note in the array with the nearest frequency
   */
  static Note getNearestNote(const std::vector<Musical::Note> &notes,
                             double freq);

  /**
   * the nearest note in a scale
   */
  static Note getNearestNote(const std::string &scale, const std::string &tone,
                             double freq);
};

std::ostream &operator<<(std::ostream &os, const Musical::Note &n);