
#include <QJsonDocument>
#include <QAction>
#include <QDebug>
#include "settings_module.h"
#include "../main/main_module.h"
#include "settings_window.h"
#include <fstream>
#include <QtWidgets/QDialogButtonBox>
#include <iostream>

namespace gago {
namespace gui {
namespace modules {

SettingsModule::SettingsModule() : QObject(nullptr), IModule("Settings", "settings"), settings_("NVision", "Gago") {
}

ModuleInitializationResult SettingsModule::Initalize() {
  return ModuleInitializationResult::OK;
}

unsigned int SettingsModule::MajorVersion() const {
  return 1;
}

unsigned int SettingsModule::MinorVersion() const {
  return 0;
}

void SettingsModule::GetRequiredModules(QList<RequiredModuleParams> & out_required_modules) {
  out_required_modules = {RequiredModuleParams{Name : "main", MinMajorVersion : 1, MinMinorVersion : 0}};
}

void SettingsModule::SetRequiredModules(const QList<IModule *> & modules) {
  MainModule *main_module = (MainModule *) modules[0];
  main_window_ = main_module->MainWindow();
  QAction *action = main_module->GetAction("/Edit/Settings");
  action->setIcon(QIcon::fromTheme("gnome-settings"));
  QObject::connect(action,
                   &QAction::triggered,
                   this, &SettingsModule::ShowSettings);

}

void SettingsModule::RegisterConfigurable(configuration::IConfigurable *configurable) {
  configurables_.push_back(configurable);
}

void SettingsModule::Save() {
  settings_.sync();
}

void SettingsModule::Configure() {
  for (configuration::IConfigurable *configurable: configurables_) {
    settings_.beginGroup(configurable->GetName());
    configurable->Configure(settings_);
    settings_.endGroup();
  }
}

int SettingsModule::GetWeight() const {
  return 2;
}

void SettingsModule::ShowSettings() {

  QList<configuration::IConfigurator *> configurators;
  for (configuration::IConfigurable *configurable: configurables_) {
    configurators.push_back(configurable->GetConfigurator());
  }
  SettingsWindow *window = new SettingsWindow(configurators, main_window_);
  QObject::connect(window, &SettingsWindow::applied, [&]() {
    for (int i = 0; i < configurators.size(); ++i) {
      settings_.beginGroup(configurables_[i]->GetName());
      configurables_[i]->ApplyConfiguration(settings_, configurators[i]);
      settings_.endGroup();
    }
    Save();
  });
  window->setAttribute(Qt::WA_DeleteOnClose);
  window->exec();
  for (int i = 0; i < configurators.size(); ++i) {
    configurables_[i]->DisposeConfigurator(configurators[i]);
  }

}

}
}
}
