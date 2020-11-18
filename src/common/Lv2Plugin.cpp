
#include "Lv2Plugin.hpp"

namespace lv2cpp {

  Lv2Plugin::Lv2Plugin() {
  }

  Lv2Plugin::~Lv2Plugin() {
  }

  void Lv2Plugin::set_sample_rate(double rate) {
      this->sample_rate = rate;
  }

  double Lv2Plugin::get_sample_rate() {
      return sample_rate;
  }

  void Lv2Plugin::add_port(int n, void* buffer) {
      if(buffer != nullptr) {
        Lv2Port p{n, buffer};
        if(n == ports.size()) {
            ports.push_back(p);
        } else {
            ports.insert(ports.begin(), n, p);
        }
      }
  }

  void Lv2Plugin::activate() {

  } 

  void Lv2Plugin::run(uint32_t n_samples) {

  }
  
  void Lv2Plugin::deactivate() {

  }

}