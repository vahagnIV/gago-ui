#ifndef CAMERA_MODULE_H
#define CAMERA_MODULE_H

#include "../../imodule.h"
#include "modules/settings/iconfigurable.h"
#include "modules/settings/settings_module.h"
#include "io/video/linux/v4l_driver.h"

namespace gago {
namespace gui {
namespace modules {

typedef gago::io::video::CameraWatcher CameraWatcher;

class CameraModule : public IModule, public configuration::IConfigurable {

 public:
  CameraModule();
  virtual ~CameraModule() ;

  // IConfigurable
  void ApplyConfiguration(configuration::IConfigurator *configurator) override;
  configuration::IConfigurator *GetConfigurator() override;
  void DisposeConfigurator(configuration::IConfigurator *configurator) override;

  // IModule
  ModuleInitializationResult Initalize() override;
  unsigned int MajorVersion() const override;
  unsigned int MinorVersion() const override;
  void QRequiredModules(std::vector<RequiredModuleParams> &out_required_modules) override;
  void SetRequiredModules(const std::vector<IModule *> &modules) override;
  int GetWeight() const override;

  virtual void RegisterWatcher(CameraWatcher *watcher);
  virtual void UnRegisterWatcher(CameraWatcher *watcher);
  virtual QVector<const io::video::CameraMeta *> GetCameras();
 private:
  io::video::V4lDriver driver_;
  SettingsModule *settings_module_;

};

}
}
}
#endif //CAMERA_MODULE_H
