#pragma once
#include <lv2.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "lv2/lv2plug.in/ns/ext/urid/urid.h"

namespace lv2cpp {

struct Lv2Port {
  int id;
  void *buffer;
};

class Lv2AudioPlugin {

  std::string path;

  std::vector<Lv2Port> ports;

  std::map<std::string, LV2_URID> urids;

  LV2_URID atom_Blank;

  LV2_URID atom_Object;

  LV2_URID atom_String;

  double sample_rate = .0;

  int nsamples = 0;

  double time_ms = .0;

public:
  Lv2AudioPlugin();

  virtual ~Lv2AudioPlugin();

  void init_features(const char *bundle_path,
                     const LV2_Feature *const *features);

  virtual void set_sample_rate(double rate);

  virtual double get_sample_rate();

  virtual void add_port(int n, void *buffer);

  virtual void process_events(int n);

  template <typename T = float> T *get_buffer(int n) {
    return (T *)ports.at(n).buffer;
  }

  virtual void activate();

  void _run(uint32_t nsamples);

  virtual void nsamples_changed(uint32_t old_nsamples, uint32_t nsamples);

  virtual void run(uint32_t nsamples);

  virtual void deactivate();
};

////////////////////////////////////////////////////////////////////////////////
#define EXPORT_AUDIO_PLUGIN(uri, className)                                    \
  extern "C" {                                                                 \
                                                                               \
  LV2_Handle plugin_instantiate(const LV2_Descriptor *descriptor, double rate, \
                                const char *bundle_path,                       \
                                const LV2_Feature *const *features) {          \
    lv2cpp::Lv2AudioPlugin *plugin = new className();                          \
    plugin->init_features(bundle_path, features);                              \
    plugin->set_sample_rate(rate);                                             \
    return static_cast<LV2_Handle>(plugin);                                    \
  }                                                                            \
                                                                               \
  void plugin_connect_port(LV2_Handle h, uint32_t port, void *data) {          \
    lv2cpp::Lv2AudioPlugin *plugin = static_cast<lv2cpp::Lv2AudioPlugin *>(h); \
    plugin->add_port(port, data);                                              \
  }                                                                            \
                                                                               \
  void plugin_activate(LV2_Handle h) {                                         \
    lv2cpp::Lv2AudioPlugin *plugin = static_cast<lv2cpp::Lv2AudioPlugin *>(h); \
    plugin->activate();                                                        \
  }                                                                            \
                                                                               \
  void plugin_run(LV2_Handle h, uint32_t n) {                                  \
    lv2cpp::Lv2AudioPlugin *plugin = static_cast<lv2cpp::Lv2AudioPlugin *>(h); \
    plugin->_run(n);                                                           \
  }                                                                            \
                                                                               \
  void plugin_deactivate(LV2_Handle h) {                                       \
    lv2cpp::Lv2AudioPlugin *plugin = static_cast<lv2cpp::Lv2AudioPlugin *>(h); \
    plugin->deactivate();                                                      \
  }                                                                            \
                                                                               \
  void plugin_cleanup(LV2_Handle h) {                                          \
    lv2cpp::Lv2AudioPlugin *plugin = static_cast<lv2cpp::Lv2AudioPlugin *>(h); \
    delete plugin;                                                             \
  }                                                                            \
                                                                               \
  const void *plugin_extension_data(const char *uri_) { return nullptr; }      \
                                                                               \
  static LV2_Descriptor plugin_descriptor{.URI = uri,                          \
                                          .instantiate = plugin_instantiate,   \
                                          .connect_port = plugin_connect_port, \
                                          .activate = plugin_activate,         \
                                          .run = plugin_run,                   \
                                          .deactivate = plugin_deactivate,     \
                                          .cleanup = plugin_cleanup,           \
                                          .extension_data =                    \
                                              plugin_extension_data};          \
                                                                               \
  const LV2_Descriptor *lv2_descriptor(uint32_t index) {                       \
    return index == 0 ? &plugin_descriptor : nullptr;                          \
  }                                                                            \
  } // extern "C"
////////////////////////////////////////////////////////////////////////////////

// The following macros are only used by the generate_manifest script
#define PLUGIN_DESCRIPTION(uri, description)

#define PLUGIN_ADD_PORT(uri, n, type, name)

#define PLUGIN_ADD_CONTROL(uri, n, name, min, max, defaultval)

#define PLUGIN_ADD_CONTROL_ENUM(uri, n, name, csv)

#define PLUGIN_ADD_UI(uri, ui_uri)

} // namespace lv2cpp