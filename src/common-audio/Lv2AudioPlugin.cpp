
#include "Lv2AudioPlugin.hpp"

#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"

namespace lv2cpp {

Lv2AudioPlugin::Lv2AudioPlugin() {}

Lv2AudioPlugin::~Lv2AudioPlugin() {}

void Lv2AudioPlugin::init_features(const char *bundle_path,
                                   const LV2_Feature *const *features) {

  path = bundle_path;

  LV2_URID_Map *map = nullptr;
  for (int i = 0; features[i]; i += 1) {
    map = (LV2_URID_Map *)features[i]->data;
  }
  if (map) {

    LV2_URID urid;

    urid = map->map(map->handle, LV2_ATOM__Blank);
    urids.insert(std::pair<std::string, LV2_URID>("Blank", urid));
    atom_Blank = urid;

    urid = map->map(map->handle, LV2_ATOM__String);
    urids.insert(std::pair<std::string, LV2_URID>("String", urid));
    atom_String = urid;

    urid = map->map(map->handle, LV2_ATOM__Float);
    urids.insert(std::pair<std::string, LV2_URID>("Float", urid));

    // TODO add other well-known urids
  }
}

void Lv2AudioPlugin::set_sample_rate(double rate) {
  this->sample_rate = rate;
  this->time_ms = 0.0;
}

double Lv2AudioPlugin::get_sample_rate() { return sample_rate; }

void Lv2AudioPlugin::add_port(int n, void *buffer) {
  if (buffer != nullptr) {
    Lv2Port p{n, buffer};
    if (n == ports.size()) {
      ports.push_back(p);
    } else {
      ports.insert(ports.begin(), n, p);
    }
  }
}

void Lv2AudioPlugin::activate() {}

void Lv2AudioPlugin::_run(uint32_t nsamples_) {
  if (this->nsamples != nsamples_) {
    nsamples_changed(this->nsamples, nsamples_);
    this->nsamples = nsamples_;
  }
  run(nsamples_);
  this->time_ms += nsamples_ * 1000 / sample_rate;
}

void Lv2AudioPlugin::nsamples_changed(uint32_t old_nsamples,
                                      uint32_t nsamples) {}

void Lv2AudioPlugin::run(uint32_t n_samples) {}

void Lv2AudioPlugin::deactivate() {}

void Lv2AudioPlugin::process_events(int n) {
  const LV2_Atom_Sequence *control = get_buffer<const LV2_Atom_Sequence>(n);
  const LV2_Atom_Event *ev = lv2_atom_sequence_begin(&control->body);
  while (!lv2_atom_sequence_is_end(&control->body, control->atom.size, ev)) {
    if (ev->body.type == atom_Blank || ev->body.type == atom_Object) {
      const LV2_Atom_Object *obj = (const LV2_Atom_Object *)&ev->body;
    }
    ev = lv2_atom_sequence_next(ev);
  }
}

} // namespace lv2cpp