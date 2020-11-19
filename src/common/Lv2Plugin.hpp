#pragma once
#include <lv2.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace lv2cpp {

struct Lv2Port {
  int id;
  void *buffer;
};

class Lv2Plugin {

  std::vector<Lv2Port> ports;

  double sample_rate = .0;

  int nsamples = 0;

  double time_ms = .0;

public:
  Lv2Plugin();

  virtual ~Lv2Plugin();

  virtual void set_sample_rate(double rate);

  virtual double get_sample_rate();

  virtual void add_port(int n, void *buffer);

  template <typename T = float> T *get_buffer(int n) {
    return (T *)ports.at(n).buffer;
  }

  virtual void activate();

  void _run(uint32_t nsamples);

  virtual void nsamples_changed(uint32_t old_nsamples, uint32_t nsamples);

  virtual void run(uint32_t nsamples);

  virtual void deactivate();
};

#define DECLARE_PLUGIN(uri, className)                                         \
  extern "C" {                                                                 \
                                                                               \
  LV2_Handle plugin_instantiate(const LV2_Descriptor *descriptor, double rate, \
                                const char *bundle_path,                       \
                                const LV2_Feature *const *features) {          \
    Lv2Plugin *plugin = new className();                                       \
    plugin->set_sample_rate(rate);                                             \
    return static_cast<LV2_Handle>(plugin);                                    \
  }                                                                            \
                                                                               \
  void plugin_connect_port(LV2_Handle h, uint32_t port, void *data) {          \
    Lv2Plugin *plugin = static_cast<Lv2Plugin *>(h);                           \
    plugin->add_port(port, data);                                              \
  }                                                                            \
                                                                               \
  void plugin_activate(LV2_Handle h) {                                         \
    Lv2Plugin *plugin = static_cast<Lv2Plugin *>(h);                           \
    plugin->activate();                                                        \
  }                                                                            \
                                                                               \
  void plugin_run(LV2_Handle h, uint32_t n) {                                  \
    Lv2Plugin *plugin = static_cast<Lv2Plugin *>(h);                           \
    plugin->_run(n);                                                           \
  }                                                                            \
                                                                               \
  void plugin_deactivate(LV2_Handle h) {                                       \
    Lv2Plugin *plugin = static_cast<Lv2Plugin *>(h);                           \
    plugin->deactivate();                                                      \
  }                                                                            \
                                                                               \
  void plugin_cleanup(LV2_Handle h) {                                          \
    Lv2Plugin *plugin = static_cast<Lv2Plugin *>(h);                           \
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
    return &plugin_descriptor;                                                 \
  }                                                                            \
                                                                               \
  } // extern "C"

#define PLUGIN_DESCRIPTION(uri, description)
#define PLUGIN_ADD_PORT(uri, n, type, name)

} // namespace lv2cpp