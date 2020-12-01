#include <catch2/catch_all.hpp>

#include "common-audio/Musical.hpp"
#include <sstream>
using namespace std;

TEST_CASE("getNearestNote", "[Musical]") {

  Musical::Note ab6 = Musical::getNearestNote("Mixolydian", "Ab", 3323.0);
  REQUIRE(ab6.id == 83);
  REQUIRE(ab6.freq > 3322.0);
  REQUIRE(ab6.freq < 3323.0);

  Musical::Note g0 = Musical::getNearestNote("Chromatic", "A", 50);
  REQUIRE_THAT(g0.name(), Catch::Matchers::Equals("G0"));
}

TEST_CASE("names", "[Musical]") {
  ostringstream ss;
  for (auto note : Musical::getNotes("Major pentatonic", "G")) {
    ss << note.id << " " << note << " " << note.freq;
  }
  REQUIRE_THAT(ss.str(), Catch::Matchers::Equals("0 A-1 27.5"
                                                 "2 B-1 30.8677"
                                                 "5 D0 36.7081"
                                                 "7 E0 41.2034"
                                                 "10 G0 48.9994"
                                                 "12 A0 55"
                                                 "14 B0 61.7354"
                                                 "17 D1 73.4162"
                                                 "19 E1 82.4069"
                                                 "22 G1 97.9989"
                                                 "24 A1 110"
                                                 "26 B1 123.471"
                                                 "29 D2 146.832"
                                                 "31 E2 164.814"
                                                 "34 G2 195.998"
                                                 "36 A2 220"
                                                 "38 B2 246.942"
                                                 "41 D3 293.665"
                                                 "43 E3 329.628"
                                                 "46 G3 391.995"
                                                 "48 A3 440"
                                                 "50 B3 493.883"
                                                 "53 D4 587.33"
                                                 "55 E4 659.255"
                                                 "58 G4 783.991"
                                                 "60 A4 880"
                                                 "62 B4 987.767"
                                                 "65 D5 1174.66"
                                                 "67 E5 1318.51"
                                                 "70 G5 1567.98"
                                                 "72 A5 1760"
                                                 "74 B5 1975.53"
                                                 "77 D6 2349.32"
                                                 "79 E6 2637.02"
                                                 "82 G6 3135.96"
                                                 "84 A6 3520"
                                                 "86 B6 3951.07"));
}
