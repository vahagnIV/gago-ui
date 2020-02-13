#include "camera_module.h"
#include "modules/settings/settings_module.h"
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

void CameraModule::GetRequiredModules(QList<RequiredModuleParams> & out_required_modules) {
  out_required_modules = {RequiredModuleParams{Name : "settings", MinMajorVersion : 1, MinMinorVersion : 0}};
}

void CameraModule::SetRequiredModules(const QList<IModule *> & modules) {
  settings_module_ = ((SettingsModule *) modules[0]);
  settings_module_->RegisterConfigurable(this);
}

configuration::IConfigurator *CameraModule::GetConfigurator() {
  std::vector<io::video::CameraSettings> settings;
  driver_.GetSettings(settings);
  return new configuration::CameraConfigurator(settings);
}

void CameraModule::DisposeConfigurator(configuration::IConfigurator *configurator) {
  configuration::CameraConfigurator *cfr = (configuration::CameraConfigurator *) configurator;
  delete cfr;
}

void CameraModule::ApplyConfiguration(configuration::IConfigurator *configurator) {
  configuration::CameraConfigurator *cfr = (configuration::CameraConfigurator *) configurator;
  const std::vector<io::video::CameraSettings> settings = cfr->GetSettings();
  driver_.SetSettings(settings);
  driver_.Start();
}
void CameraModule::RegisterWatcher(CameraWatcher *watcher) {
  driver_.RegisterWatcher(watcher);
}

void CameraModule::UnRegisterWatcher(CameraWatcher *watcher) {
  driver_.UnRegister(watcher);
}

QVector<const io::video::CameraMeta *> CameraModule::GetCameras() {
  return QVector<const io::video::CameraMeta *>::fromStdVector(driver_.GetCameras());;
}

int CameraModule::GetWeight() const {
  return settings_module_->GetWeight() + 1;
}

CameraModule::~CameraModule() {
  driver_.Stop();
}

}
}
}
