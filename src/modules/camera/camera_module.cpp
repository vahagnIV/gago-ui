#include <modules/settings/settings_module.h>
#include "camera_module.h"
#include "camera_configurator.h"

namespace gago {
namespace gui {
namespace modules {

CameraModule::CameraModule() : IModule("Camera", "camera") {
  driver_.Initialize();
}

ModuleInitializationResult CameraModule::Initalize() {
  return ModuleInitializationResult::OK;
}

unsigned int CameraModule::MajorVersion() const {
  return 1;
}

unsigned int CameraModule::MinorVersion() const {
  return 0;
}

void CameraModule::QRequiredModules(std::vector<RequiredModuleParams> &out_required_modules) {
  out_required_modules.resize(1);
  out_required_modules[0].Name = "settings";
  out_required_modules[0].MinMajorVersion = 1;
  out_required_modules[0].MinMinorVersion = 0;
}

void CameraModule::SetRequiredModules(const std::vector<IModule *> &modules) {
  ((SettingsModule *) modules[0])->RegisterConfigurable(this);
}

configuration::IConfigurator *CameraModule::GetConfigurator() {
  return new configuration::CameraConfigurator(driver_.GetDevices(), driver_.GetSettings());
}

void CameraModule::DisposeConfigurator(configuration::IConfigurator *configurator) {
  configuration::CameraConfigurator * cfr = (configuration::CameraConfigurator *) configurator;
  delete cfr;
}

void CameraModule::ApplyConfiguration(configuration::IConfigurator *configurator) {
  configuration::CameraConfigurator * cfr = (configuration::CameraConfigurator *) configurator;
  const std::vector<io::video::CameraSettings>  settings = cfr->GetSettings();
  driver_.SetSettings(settings);

}

}
}
}
