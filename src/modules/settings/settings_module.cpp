
#include <QAction>
#include "settings_module.h"
#include "../main/main_module.h"
#include "settings_window.h"
#include <fstream>
#include <QtWidgets/QDialogButtonBox>
#include <iostream>
#include <boost/filesystem.hpp>

namespace gago {
namespace gui {
namespace modules {

SettingsModule::SettingsModule() : IModule("Settings", "settings") {
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

void SettingsModule::QRequiredModules(std::vector<RequiredModuleParams> & out_required_modules) {
  out_required_modules.resize(1);
  out_required_modules[0].MinMinorVersion = 0;
  out_required_modules[0].MinMajorVersion = 1;
  out_required_modules[0].Name = "main";
}

void SettingsModule::SetRequiredModules(const std::vector<IModule *> & modules) {
  MainModule *main_module = (MainModule *) modules[0];
  main_window_ = main_module->MainWindow();
  QAction *action = main_module->CreateMenuBranch("/Edit/Settings");
  QObject::connect(action,
                   &QAction::triggered,
                   [&]() {
                     std::vector<configuration::IConfigurator *> configurators;
                     for (configuration::IConfigurable *configurable: configurables_) {
                       configurators.push_back(configurable->GetConfigurator());
                     }
                     SettingsWindow *window = new SettingsWindow(configurators, main_window_);
                     QObject::connect(window, &SettingsWindow::applied, [&]() {
                       for (int i = 0; i < configurators.size(); ++i) {
                         configurables_[i]->ApplyConfiguration(configurators[i]);
                       }
                       Save(configurators);
                     });
                     window->setAttribute(Qt::WA_DeleteOnClose);
                     window->exec();
                     for (int i = 0; i < configurators.size(); ++i) {
                       configurables_[i]->DisposeConfigurator(configurators[i]);
                     }
                   });

}

void SettingsModule::RegisterConfigurable(configuration::IConfigurable *configurable) {
  configurables_.push_back(configurable);
}

void SettingsModule::Save(std::vector<configuration::IConfigurator *> & configurators) {
  nlohmann::json json;
  for (configuration::IConfigurator *configurator: configurators) {
    nlohmann::json module_json;
    configurator->GetConfiguration(module_json);
    json[configurator->ConfigWindowName()] = module_json;
  }
  std::ofstream of("settings.json");
  of << json;
  of.close();
}

void SettingsModule::Configure() {
  if (!boost::filesystem::exists("settings.json"))
    return;

  std::ifstream istream("settings.json");
  nlohmann::json json;
  istream >> json;
  for (configuration::IConfigurable *configurable: configurables_) {
    configuration::IConfigurator *configurator = configurable->GetConfigurator();
    if (json.find(configurator->ConfigWindowName()) != json.end())
      configurator->SetConfiguration(json[configurator->ConfigWindowName()]);
    configurable->ApplyConfiguration(configurator);
    configurable->DisposeConfigurator(configurator);
  }
  istream.close();
}

int SettingsModule::GetWeight() const {
  return 2;
}

}
}
}
