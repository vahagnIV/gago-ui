//
// Created by vahagn on 12/30/19.
//

#include "settings_window.h"
#include "ui_settings_window.h"
#include <QTabWidget>
#include <QDialogButtonBox>
#include <iostream>

namespace gago {
namespace gui {
namespace modules {

SettingsWindow::SettingsWindow(std::vector<configuration::IConfigurator *> &configurators, QWidget *parent)
    : configurators_(configurators), QDialog(parent), ui(new Ui::SettingsWindow) {
  ui->setupUi(this);
  connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &SettingsWindow::clicked);
  for (configuration::IConfigurator *configurator: configurators_) {
    QWidget *tab = new QWidget();
    ui->tabWidget->addTab(tab, configurator->ConfigWindowName().c_str());
    configurator->DrawConfigurationPage(tab);
  }
}

SettingsWindow::~SettingsWindow() {
  delete ui;
}

void SettingsWindow::apply() {
  applied();
  for (configuration::IConfigurator *configurator: configurators_)
    configurator->Apply();
}

void SettingsWindow::clicked(QAbstractButton *btn) {
  switch (ui->buttonBox->standardButton(btn)) {
    case QDialogButtonBox::StandardButton::Apply:apply();
      break;
    case QDialogButtonBox::StandardButton::Ok:apply();
      close();
      break;
    case QDialogButtonBox::StandardButton::Cancel:close();
      break;
    default:break;
  }
}
}
}
}