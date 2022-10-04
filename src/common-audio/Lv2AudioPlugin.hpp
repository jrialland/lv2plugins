#pragma once
#include <lv2.h>

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "lv2/lv2plug.in/ns/ext/urid/urid.h"

namespace lv2cpp
{

  /** The infos that we discover at runtime have about the declared ports */
  struct Lv2Port
  {
    int id;       // port index
    void *buffer; // points to the buffer's data
  };

  /**
   * @brief This class wraps the functionnality brought by the lv2 framework
   *
   */
  class Lv2AudioPlugin
  {

    /** the plugin's directory (maybe useful for loading assets)*/
    std::string path;

    /** list of ports */
    std::vector<Lv2Port> ports;

    /** loaded schemas */
    std::map<std::string, LV2_URID> urids;

    /** shorcut to the "Blank" type */
    LV2_URID atom_Blank;

    /** shorcut to the "Object" type */
    LV2_URID atom_Object;

    /** shorcut to the "String" type */
    LV2_URID atom_String;

    /** shorcut to the "MidiEvent" type */
    LV2_URID atom_MidiEvent;

    /** the current sampling rate */
    double sample_rate = .0;

    /** saves the last number of samples that we have processed */
    int nsamples = 0;

    /** saves the time */
    double time_ms = .0;

  public:
    Lv2AudioPlugin();

    virtual ~Lv2AudioPlugin();

    /** called by the lv2 framework when the plugin is instanciated */
    void init_features(const char *bundle_path,
                       const LV2_Feature *const *features);

    /** called by the lv2 framework when the plugin is instanciated. user code should not call this directly*/
    virtual void set_sample_rate(double rate);

    /** called by the lv2 framework when the plugin is instanciated. user code should not call this directly*/
    virtual void _add_port_(int n, void *buffer);

    /** called by the lv2 framework when the plugin is instanciated. user code should not call this directly*/
    void _run_(uint32_t nsamples);

    virtual double get_sample_rate();

    /** obtain a pointer to the audio buffer that correspond to a AUDIO_IN or AUDIO_OUT port */
    template <int n>
    float *get_buffer()
    {
      return (float*)ports.at(n).buffer;
    }

    /** obtain a control value (CONTROL_IN ports) */
    template <int n>
    float get_port_value()
    {
      return *((float *)ports.at(n).buffer);
    }

    /** process the incoming events from an incoming event port*/
    void process_events(int n);

    /** send an event on the given port */
    void send_event(int n);

    /** might be overriden if the plugin has to do some initializion steps */
    virtual void activate();

    /** might be overriden if the plugin needs to be notified when the sampling rate changes */
    virtual void nsamples_changed(uint32_t old_nsamples, uint32_t nsamples);

    /** Actual process to be implemented : A plugin typically read input buffers, and writes on ouput buffers.
     *  and eventually processes and emit events.
     */
    virtual void run(uint32_t nsamples);

    /** called when the plugin is deactived, can be overriden if the plugin needs some extra actions when it is turned off */
    virtual void deactivate();
  };

////////////////////////////////////////////////////////////////////////////////
/**
 * This macro should be added in the source code of your plugin.
 * It is used for adding the symbols that Lv2 need for bootstraping a plugin library
 */
#define EXPORT_AUDIO_PLUGIN(uri, className)                                      \
  extern "C"                                                                     \
  {                                                                              \
                                                                                 \
    LV2_Handle plugin_instantiate(const LV2_Descriptor *descriptor, double rate, \
                                  const char *bundle_path,                       \
                                  const LV2_Feature *const *features)            \
    {                                                                            \
      lv2cpp::Lv2AudioPlugin *plugin = new className();                          \
      plugin->init_features(bundle_path, features);                              \
      plugin->set_sample_rate(rate);                                           \
      return static_cast<LV2_Handle>(plugin);                                    \
    }                                                                            \
                                                                                 \
    void plugin_connect_port(LV2_Handle h, uint32_t port, void *data)            \
    {                                                                            \
      lv2cpp::Lv2AudioPlugin *plugin = static_cast<lv2cpp::Lv2AudioPlugin *>(h); \
      plugin->_add_port_(port, data);                                            \
    }                                                                            \
                                                                                 \
    void plugin_activate(LV2_Handle h)                                           \
    {                                                                            \
      lv2cpp::Lv2AudioPlugin *plugin = static_cast<lv2cpp::Lv2AudioPlugin *>(h); \
      plugin->activate();                                                        \
    }                                                                            \
                                                                                 \
    void plugin_run(LV2_Handle h, uint32_t n)                                    \
    {                                                                            \
      lv2cpp::Lv2AudioPlugin *plugin = static_cast<lv2cpp::Lv2AudioPlugin *>(h); \
      plugin->_run_(n);                                                          \
    }                                                                            \
                                                                                 \
    void plugin_deactivate(LV2_Handle h)                                         \
    {                                                                            \
      lv2cpp::Lv2AudioPlugin *plugin = static_cast<lv2cpp::Lv2AudioPlugin *>(h); \
      plugin->deactivate();                                                      \
    }                                                                            \
                                                                                 \
    void plugin_cleanup(LV2_Handle h)                                            \
    {                                                                            \
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
    const LV2_Descriptor *lv2_descriptor(uint32_t index)                         \
    {                                                                            \
      return index == 0 ? &plugin_descriptor : nullptr;                          \
    }                                                                            \
  } // extern "C"
////////////////////////////////////////////////////////////////////////////////

} // namespace lv2cpp