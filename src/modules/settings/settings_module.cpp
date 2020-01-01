
#include <QAction>
#include "settings_module.h"
#include "../main/main_module.h"
#include "settings_window.h"

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

void SettingsModule::QRequiredModules(std::vector<RequiredModuleParams> &out_required_modules) {
  out_required_modules.resize(1);
  out_required_modules[0].MinMinorVersion = 0;
  out_required_modules[0].MinMajorVersion = 1;
  out_required_modules[0].Name = "main";
}

void SettingsModule::SetRequiredModules(const std::vector<IModule *> &modules) {
  MainModule *main_module = (MainModule *) modules[0];
  main_window_ = main_module->MainWindow();
  QAction *action = main_module->CreateMenuBranch("/Edit/Settings");
  QObject::connect(action,
                   &QAction::triggered,
                   [&]() {
                     SettingsWindow *window = new SettingsWindow(main_window_);
                     window->setAttribute(Qt::WA_DeleteOnClose);
                     for (configuration::IConfigurable *configurable: configurables_) {
                       window->AddConfigurator(configurable->GetConfigurator(), configurable->ConfigWindowName());
                     }
                     window->show();
                   });

}

void SettingsModule::RegisterConfigurable(configuration::IConfigurable *configurable) {
  configurables_.push_back(configurable);

}

}
}
}
