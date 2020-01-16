#include <modules/settings/settings_module.h>
#include "calibration_module.h"
#include "modules/main/main_module.h"
#include "calibration_configurator.h"

namespace gago {
namespace gui {
namespace modules {

CalibrationModule::CalibrationModule() : IModule("Calibration", "calibration") {
}

ModuleInitializationResult CalibrationModule::Initalize() {
  return ModuleInitializationResult::OK;
}

unsigned int CalibrationModule::MajorVersion() const {
  return 1;
}

unsigned int CalibrationModule::MinorVersion() const {
  return 0;
}

void CalibrationModule::QRequiredModules(std::vector<RequiredModuleParams> & out_required_modules) {
  out_required_modules.resize(2);

  out_required_modules[0].Name = "main";
  out_required_modules[0].MinMajorVersion = 1;
  out_required_modules[0].MinMinorVersion = 0;

  out_required_modules[1].Name = "settings";
  out_required_modules[1].MinMajorVersion = 1;
  out_required_modules[1].MinMinorVersion = 0;
}

void CalibrationModule::SetRequiredModules(const std::vector<IModule *> & modules) {
  for (IModule *module: modules) {
    if (module->SystemName() == "main") {
      QAction *action = ((MainModule *) module)->CreateMenuBranch("/File/Calibration/Calibrate");
    } else if (module->SystemName() == "settings") {
      ((SettingsModule *) module)->RegisterConfigurable(this);
    }
  }
}

configuration::IConfigurator *CalibrationModule::GetConfigurator() {
  configuration::IConfigurator *cnf = new configuration::CalibrationConfigurator();
  if (!settings_.empty())
    cnf->SetConfiguration(settings_);
  return cnf;
}

void CalibrationModule::DisposeConfigurator(configuration::IConfigurator *configurator) {
  configuration::CalibrationConfigurator *cnf = (configuration::CalibrationConfigurator *) configurator;
  delete cnf;
}

void CalibrationModule::ApplyConfiguration(configuration::IConfigurator *configurator) {
  configurator->GetConfiguration(settings_);
}

}
}
}
