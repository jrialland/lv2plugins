
#include <QApplication>
#include <QWidget>
#include <cstring>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

#include <string>

namespace lv2cpp {

class Lv2UiPlugin : public QWidget {
  Q_OBJECT

private:
  static int instances;

  static QApplication *qApplication;

  bool idleClosed;

  LV2UI_Controller controller;

  LV2UI_Write_Function writeFn;

public:
  Lv2UiPlugin(QWidget *parent = 0, Qt::WindowFlags wflags = 0);

  virtual ~Lv2UiPlugin();

  void port_event(uint32_t port, uint32_t buffer_size, uint32_t format,
                  const void *buffer);

  virtual void closeEvent(QCloseEvent *evt) override;

  bool isIdleClosed() const;

  static int countInstances();

  void setController(LV2UI_Controller controller);

  void setWriteFn(LV2UI_Write_Function writeFn);

  void sendToPlugin(uint32_t port, const std::string &data);

  static QApplication *getApp();

  static void setApp(QApplication *qApplication);
};

} // namespace lv2cpp

////////////////////////////////////////////////////////////////////////////////
#define EXPORT_UI_PLUGIN(uri, className)                                       \
  extern "C" {                                                                 \
                                                                               \
  static int ui_show(LV2UI_Handle h) {                                         \
    if (h) {                                                                   \
      lv2cpp::Lv2UiPlugin *plugin = static_cast<lv2cpp::Lv2UiPlugin *>(h);     \
      plugin->show();                                                          \
      plugin->raise();                                                         \
      plugin->activateWindow();                                                \
      return 0;                                                                \
    } else {                                                                   \
      return 1;                                                                \
    }                                                                          \
  }                                                                            \
                                                                               \
  static int ui_hide(LV2UI_Handle h) {                                         \
    if (h) {                                                                   \
      lv2cpp::Lv2UiPlugin *plugin = static_cast<lv2cpp::Lv2UiPlugin *>(h);     \
      plugin->hide();                                                          \
      return 0;                                                                \
    } else {                                                                   \
      return 1;                                                                \
    }                                                                          \
  }                                                                            \
                                                                               \
  static int ui_idle(LV2UI_Handle h) {                                         \
    if (h) {                                                                   \
      lv2cpp::Lv2UiPlugin *plugin = static_cast<lv2cpp::Lv2UiPlugin *>(h);     \
      if (!plugin->isIdleClosed()) {                                           \
        QApplication::processEvents();                                         \
        return 0;                                                              \
      }                                                                        \
    }                                                                          \
    return 1;                                                                  \
  }                                                                            \
                                                                               \
  static const LV2UI_Show_Interface ui_show_interface = {ui_show, ui_hide};    \
                                                                               \
  static const LV2UI_Idle_Interface ui_idle_interface = {ui_idle};             \
                                                                               \
  static LV2UI_Handle                                                          \
  plugin_instantiate(const struct _LV2UI_Descriptor *descriptor,               \
                     const char *plugin_uri, const char *bundle_path,          \
                     LV2UI_Write_Function write_function,                      \
                     LV2UI_Controller controller, LV2UI_Widget *widget,        \
                     const LV2_Feature *const *features) {                     \
    if (qApp == nullptr && lv2cpp::Lv2UiPlugin::getApp() == nullptr) {         \
      static int i_argc = 1;                                                   \
      static const char *s_argv[] = {__func__, nullptr};                       \
      lv2cpp::Lv2UiPlugin::setApp(                                             \
          new QApplication(i_argc, const_cast<char **>(s_argv)));              \
    }                                                                          \
    lv2cpp::Lv2UiPlugin *plugin = new className();                             \
    plugin->setController(controller);                                         \
    plugin->setWriteFn(write_function);                                        \
    *widget = plugin;                                                          \
    return plugin;                                                             \
  }                                                                            \
                                                                               \
  static void plugin_cleanup(LV2_Handle h) {                                   \
    if (h) {                                                                   \
      lv2cpp::Lv2UiPlugin *plugin = static_cast<lv2cpp::Lv2UiPlugin *>(h);     \
      delete plugin;                                                           \
      if (lv2cpp::Lv2UiPlugin::countInstances() == 0) {                        \
        QApplication *app = lv2cpp::Lv2UiPlugin::getApp();                     \
        if (app != nullptr) {                                                  \
          delete app;                                                          \
          lv2cpp::Lv2UiPlugin::setApp(nullptr);                                \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }                                                                            \
                                                                               \
  static void plugin_port_event(LV2_Handle h, uint32_t port_index,             \
                                uint32_t buffer_size, uint32_t format,         \
                                const void *buffer) {                          \
    if (h) {                                                                   \
      lv2cpp::Lv2UiPlugin *plugin = static_cast<lv2cpp::Lv2UiPlugin *>(h);     \
      plugin->port_event(port_index, buffer_size, format, buffer);             \
    }                                                                          \
  }                                                                            \
                                                                               \
  static const void *plugin_extension_data(const char *uri_) {                 \
    if (std::strcmp(uri_, LV2_UI__idleInterface)) {                            \
      return reinterpret_cast<void *>(                                         \
          const_cast<LV2UI_Idle_Interface *>(&ui_idle_interface));             \
    }                                                                          \
    if (std::strcmp(uri_, LV2_UI__showInterface)) {                            \
      return reinterpret_cast<void *>(                                         \
          const_cast<LV2UI_Show_Interface *>(&ui_show_interface));             \
    }                                                                          \
    return nullptr;                                                            \
  }                                                                            \
                                                                               \
  static LV2UI_Descriptor plugin_descriptor{.URI = uri,                        \
                                            .instantiate = plugin_instantiate, \
                                            .cleanup = plugin_cleanup,         \
                                            .port_event = plugin_port_event,   \
                                            .extension_data =                  \
                                                plugin_extension_data};        \
                                                                               \
  const LV2UI_Descriptor *lv2ui_descriptor(uint32_t index) {                   \
    return index == 0 ? &plugin_descriptor : nullptr;                          \
  }                                                                            \
                                                                               \
  } // extern "C"
////////////////////////////////////////////////////////////////////////////////