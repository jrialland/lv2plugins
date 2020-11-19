
#include "Lv2Plugin.hpp"

namespace lv2cpp {

Lv2Plugin::Lv2Plugin() {}

Lv2Plugin::~Lv2Plugin() {}

void Lv2Plugin::set_sample_rate(double rate) {
  this->sample_rate = rate;
  this->time_ms = 0.0;
}

double Lv2Plugin::get_sample_rate() { return sample_rate; }

void Lv2Plugin::add_port(int n, void *buffer) {
  if (buffer != nullptr) {
    Lv2Port p{n, buffer};
    if (n == ports.size()) {
      ports.push_back(p);
    } else {
      ports.insert(ports.begin(), n, p);
    }
  }
}

void Lv2Plugin::activate() {}

void Lv2Plugin::_run(uint32_t nsamples_) {
  if (this->nsamples != nsamples_) {
    nsamples_changed(this->nsamples, nsamples_);
    this->nsamples = nsamples_;
  }
  run(nsamples_);
  this->time_ms += nsamples_ * 1000 / sample_rate;
}

void Lv2Plugin::nsamples_changed(uint32_t old_nsamples, uint32_t nsamples) {}

void Lv2Plugin::run(uint32_t n_samples) {}

void Lv2Plugin::deactivate() {}

} // namespace lv2cpp