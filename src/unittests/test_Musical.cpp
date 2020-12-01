#include <catch2/catch_all.hpp>

#include "common-audio/Musical.hpp"
#include <iostream>
using namespace std;

TEST_CASE("findNearestNote", "[Musical]") {

  Musical::Note ab6 = Musical::getNearestNote("Mixolydian", "Ab", 3323.0);
  REQUIRE(ab6.id == 83);
  REQUIRE(ab6.freq > 3322.0);
  REQUIRE(ab6.freq < 3323.0);
}