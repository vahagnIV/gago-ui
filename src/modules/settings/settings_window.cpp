//
// Created by vahagn on 12/30/19.
//

#include "settings_window.h"
#include "ui_settings_window.h"
#include <QTabWidget>

namespace gago {
namespace gui {
namespace modules {

SettingsWindow::SettingsWindow(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsWindow) {
  ui->setupUi(this);
  connect(this, SIGNAL(accepted()), this, SLOT(accepted()));
}

void SettingsWindow::AddConfigurator(configuration::IConfigurator *configurator,
                                     std::string name) {
  configurators_.push_back(configurator);
  QWidget * tab = new QWidget();
  ui->tabWidget->addTab(tab, name.c_str());
  configurator->DrawConfigurationPage(tab);
}

SettingsWindow::~SettingsWindow() {
  delete ui;
}

void SettingsWindow::accepted(){
  close();
}
}
}
}