
#include "Lv2AudioPlugin.hpp"
#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"
#include "lv2/lv2plug.in/ns/ext/midi/midi.h"

#include <stdexcept>

namespace lv2cpp
{

  Lv2AudioPlugin::Lv2AudioPlugin() {}

  Lv2AudioPlugin::~Lv2AudioPlugin() {}

  void Lv2AudioPlugin::init_features(const char *bundle_path,
                                     const LV2_Feature *const *features)
  {

    path = bundle_path;

    for (int i = 0; features[i] != nullptr; i += 1)
    {
      if (std::string(LV2_URID_MAP_URI).compare(features[i]->URI) == 0)
      {
        map = (LV2_URID_Map *)features[i]->data;
        atom_Blank = map->map(map->handle, LV2_ATOM__Blank);
        atom_String = map->map(map->handle, LV2_ATOM__String);
        atom_MidiEvent = map->map(map->handle, LV2_MIDI__MidiEvent);
      }
    }

    if (map == nullptr)
    {
      throw std::runtime_error("Required feature <" LV2_URID_MAP_URI "> was not provided");
    }
  }

  void Lv2AudioPlugin::set_sample_rate(double rate)
  {
    this->sample_rate = rate;
    this->time_ms = 0.0;
  }

  double Lv2AudioPlugin::get_sample_rate() { return sample_rate; }

  void Lv2AudioPlugin::_add_port_(int n, void *buffer)
  {
    if (ports.size() == n)
    {
      Lv2Port p{n, buffer};
      ports.push_back(p);
    }
    else
    {
      ports.at(n).buffer = buffer;
    }
  }

  void Lv2AudioPlugin::process_events(int n)
  {
    LV2_Atom_Sequence *sequence = (LV2_Atom_Sequence *)ports.at(n).buffer;
    LV2_ATOM_SEQUENCE_FOREACH(sequence, ev)
    {
      if (ev->body.type == atom_MidiEvent)
      {
        const uint8_t *const msg = (const uint8_t *)(ev + 1);
        LV2_Midi_Message_Type msgType = lv2_midi_message_type(msg);
        // FIXME what to do next ?
      }
    }
  }

  void Lv2AudioPlugin::send_event(int n)
  {
    LV2_Atom_Sequence *sequence = (LV2_Atom_Sequence *)ports.at(n).buffer;
    LV2_Atom_Event evt; // FIXME how to feed the evt
    lv2_atom_sequence_append_event(sequence, sequence->atom.size, &evt);
  }

  void Lv2AudioPlugin::activate() {}

  void Lv2AudioPlugin::_run_(uint32_t nsamples_)
  {
    if (this->nsamples != nsamples_)
    {
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

} // namespace lv2cpp