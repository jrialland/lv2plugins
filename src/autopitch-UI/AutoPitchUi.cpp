#include "AutoPitchUi.hpp"

#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

AutoPitchUi::AutoPitchUi(QWidget *parent, Qt::WindowFlags wflags)
    : lv2cpp::Lv2UiPlugin(parent, wflags) {
  label = new QLabel("Hello", this);
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addStretch(1);
  layout->addWidget(label);
  setLayout(layout);
  resize(100, 100);
}

AutoPitchUi::~AutoPitchUi() {}

EXPORT_UI_PLUGIN("https://github.com/jrialland/lv2plugins/autopitch#UI",
                 AutoPitchUi);