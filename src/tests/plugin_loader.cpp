#include "plugin_loader.hpp"
#include <lv2.h>
#include <dlfcn.h>
#include <map>
#include <iostream>

typedef const LV2_Descriptor *(*lv2_descriptor_fn)(uint32_t index);

static std::map<std::string, LV2_URID> urids;

LV2_URID uridMapFn(LV2_URID_Map_Handle handle, const char* uri) {
    using namespace std;
    string sUri(uri);
    auto it = urids.find(sUri);    
    if(it == urids.end()) {
        LV2_URID newUrid = urids.size();
        urids.insert({sUri, newUrid});
    }
    auto urid = urids[sUri];
    clog << "URI Map : <" <<  sUri << "> => " << urid << endl;
    return urid;
}

lv2cpp::Lv2AudioPlugin *loadPlugin(const std::string &module)
{
    using namespace std;

    auto library = dlopen(module.c_str(), RTLD_NOW);
    if (!library)
    {
        throw std::runtime_error("could not load library");
    }
    clog << "library '" << module << "' loaded at " << library << endl;

    lv2_descriptor_fn descriptor_fn = (lv2_descriptor_fn)dlsym(library, "lv2_descriptor");
    if (!descriptor_fn)
    {
        throw std::runtime_error("lv2_descriptor symbol not found in library (did you forget the EXPORT_AUDIO_PLUGIN macro ?)");
    }

    const LV2_Descriptor *descriptor = descriptor_fn(0);
    clog << "Plugin URI : " << descriptor->URI << endl;

    LV2_Feature* uridMapFeature = new LV2_Feature;
    LV2_URID_Map* uridMapIface = new LV2_URID_Map;
    uridMapIface->handle = nullptr;
    uridMapIface->map = uridMapFn;
    uridMapFeature->URI = LV2_URID_MAP_URI;
    uridMapFeature->data = (void*)uridMapIface;

    LV2_Feature* features[] = {uridMapFeature, nullptr};

    LV2_Handle handle = descriptor->instantiate(descriptor, 48000.0, module.c_str(), features);

    return static_cast<lv2cpp::Lv2AudioPlugin *>(handle);
}