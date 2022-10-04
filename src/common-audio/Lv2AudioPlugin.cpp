
#include "Lv2AudioPlugin.hpp"

#include "lv2/lv2plug.in/ns/ext/atom/atom.h"
#include "lv2/lv2plug.in/ns/ext/atom/util.h"
#include "lv2/lv2plug.in/ns/ext/midi/midi.h"

namespace lv2cpp
{

  Lv2AudioPlugin::Lv2AudioPlugin() {}

  Lv2AudioPlugin::~Lv2AudioPlugin() {}

  void Lv2AudioPlugin::init_features(const char *bundle_path,
                                     const LV2_Feature *const *features)
  {

    path = bundle_path;

    LV2_URID_Map *map = nullptr;
    for (int i = 0; features[i]; i += 1)
    {
      map = (LV2_URID_Map *)features[i]->data;
    }
    if (map)
    {

      LV2_URID urid;

      urid = map->map(map->handle, LV2_ATOM__Blank);
      urids.insert(std::pair<std::string, LV2_URID>("Blank", urid));
      atom_Blank = urid;

      urid = map->map(map->handle, LV2_ATOM__String);
      urids.insert(std::pair<std::string, LV2_URID>("String", urid));
      atom_String = urid;

      urid = map->map(map->handle, LV2_ATOM__Float);
      urids.insert(std::pair<std::string, LV2_URID>("Float", urid));

      urid = map->map(map->handle, LV2_MIDI__MidiEvent);
      urids.insert(std::pair<std::string, LV2_URID>("MidiEvent", urid));
      atom_MidiEvent = urid;

      // TODO add other well-known urids
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
    if (buffer != nullptr)
    {
      Lv2Port p{n, buffer};
      if (n == ports.size())
      {
        ports.push_back(p);
      }
      else
      {
        ports.insert(ports.begin(), n, p);
      }
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