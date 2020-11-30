#pragma once

#include "common-ui/Lv2UiPlugin.hpp"

#include <QtWidgets/QLabel>
/**
 * UI for the autopitch plugin
 *
 * @lv2cpp.meta {
 *      "plugin_type" : "ui",
 *      "uri" : "https://github.com/jrialland/lv2plugins/autopitch#UI"
 * }
 *
 */
class AutoPitchUi : public lv2cpp::Lv2UiPlugin {
  Q_OBJECT

private:
  QLabel *label;

public:
  AutoPitchUi(QWidget *parent = 0, Qt::WindowFlags wflags = 0);

  virtual ~AutoPitchUi();
};
