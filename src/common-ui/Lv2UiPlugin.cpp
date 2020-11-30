
#include "Lv2UiPlugin.hpp"

#include <QCloseEvent>

#include <iostream>
using namespace std;

namespace lv2cpp {

int Lv2UiPlugin::instances = 0;

QApplication *Lv2UiPlugin::qApplication = nullptr;

QApplication *Lv2UiPlugin::getApp() { return qApplication; }

void Lv2UiPlugin::setApp(QApplication *app) { qApplication = app; }

int Lv2UiPlugin::countInstances() { return instances; }

Lv2UiPlugin::Lv2UiPlugin(QWidget *parent, Qt::WindowFlags wflags)
    : QWidget(parent, wflags), controller(nullptr), writeFn(nullptr),
      idleClosed(false) {
  instances += 1;
  QPalette pal;
  if (pal.base().color().value() < 0x7f) {
    const QColor &color = pal.window().color();
    const int iGroups = int(QPalette::Active | QPalette::Inactive) + 1;
    for (int i = 0; i < iGroups; ++i) {
      const QPalette::ColorGroup group = QPalette::ColorGroup(i);
      pal.setBrush(group, QPalette::Light, color.lighter(150));
      pal.setBrush(group, QPalette::Midlight, color.lighter(120));
      pal.setBrush(group, QPalette::Dark, color.darker(150));
      pal.setBrush(group, QPalette::Mid, color.darker(120));
      pal.setBrush(group, QPalette::Shadow, color.darker(200));
    }
    pal.setColor(QPalette::Disabled, QPalette::ButtonText, pal.mid().color());
    QWidget::setPalette(pal);
  }
}

Lv2UiPlugin::~Lv2UiPlugin() { instances -= 1; }

void Lv2UiPlugin::setController(LV2UI_Controller controller) {
  this->controller = controller;
}

void Lv2UiPlugin::setWriteFn(LV2UI_Write_Function writeFn) {
  this->writeFn = writeFn;
}

void Lv2UiPlugin::port_event(uint32_t port, uint32_t buffer_size,
                             uint32_t format, const void *buffer) {}

void Lv2UiPlugin::closeEvent(QCloseEvent *evt) {
  QWidget::closeEvent(evt);
  if (evt->isAccepted()) {
    idleClosed = true;
  }
}

void Lv2UiPlugin::sendToPlugin(uint32_t port, const string &data) {
  if (controller && writeFn) {
    writeFn(controller, port, data.length(), 0, data.c_str());
  }
}

bool Lv2UiPlugin::isIdleClosed() const { return idleClosed; }

} // namespace lv2cpp
