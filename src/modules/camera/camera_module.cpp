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

void CameraModule::ApplyConfiguration(QSettings & settings, configuration::IConfigurator *configurator) {
  configuration::CameraConfigurator *cfr = (configuration::CameraConfigurator *) configurator;
  const std::vector<io::video::CameraSettings> & cam_settings = cfr->GetSettings();
  driver_.SetSettings(cam_settings);

  settings.beginWriteArray(Name(), cam_settings.size());
  for (int cam_idx = 0; cam_idx < cam_settings.size(); ++cam_idx) {
    settings.setArrayIndex(cam_idx);
    const io::video::CameraSettings & cam_setting = cam_settings[cam_idx];
    settings.setValue("name", QString::fromStdString(cam_setting.config.name));
    settings.setValue("format", cam_setting.config.format_index);
    settings.setValue("resolution", cam_setting.config.resolution_index);
    settings.setValue("status", QString::fromStdString(to_string(cam_setting.config.status)));
    settings.setValue("uniqueId", QString::fromStdString(cam_setting.camera->GetUniqueId()));
  }
  settings.endArray();
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

int CameraModule::GetDestructorIndex() const {
  return settings_module_->GetDestructorIndex() + 1;
}

CameraModule::~CameraModule() {
  driver_.Stop();
}

void CameraModule::Configure(QSettings & settings) {
  std::vector<io::video::CameraSettings> default_settings;
  driver_.GetSettings(default_settings);
  std::vector<io::video::CameraSettings> cam_settings = default_settings;

  int size = settings.beginReadArray(Name());
  QMap<QString, int> uid_to_index;
  for (int cam_idx = 0; cam_idx < default_settings.size(); ++cam_idx)
    uid_to_index[QString::fromStdString(default_settings[cam_idx].camera->GetUniqueId())] = cam_idx;

  for (int cam_idx = 0; cam_idx < size; ++cam_idx) {
    settings.setArrayIndex(cam_idx);
    QString uid = settings.value("uniqueId").value<QString>();
    if (!uid_to_index.contains(uid))
      continue;
    int idx = uid_to_index[uid];
    cam_settings[idx].config.name = settings.value("name").toString().toStdString();
    cam_settings[idx].config.format_index = settings.value("format").toInt();
    cam_settings[idx].config.resolution_index = settings.value("resolution").toInt();
    try_parse(settings.value("status").toString().toStdString(), cam_settings[idx].config.status);

  }
  settings.endArray();
  driver_.SetSettings(cam_settings);
}

const QString & CameraModule::GetName() const {
  return Name();
}

void CameraModule::Start() {
  driver_.Start();
}

}
}
}
