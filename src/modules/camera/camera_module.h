#ifndef CAMERA_MODULE_H
#define CAMERA_MODULE_H

#include <QVector>

#include "io/video/linux/v4l_driver.h"
#include "imodule.h"
#include "modules/settings/iconfigurable.h"
#include "modules/settings/settings_module.h"

namespace gago {
namespace gui {
namespace modules {

typedef gago::io::video::CameraWatcher CameraWatcher;

class CameraModule : public IModule, public configuration::IConfigurable {

 public:
  CameraModule();
  virtual ~CameraModule() ;

  // IConfigurable
  void ApplyConfiguration(QSettings & settings, configuration::IConfigurator *configurator) override;
  configuration::IConfigurator *GetConfigurator() override;
  void DisposeConfigurator(configuration::IConfigurator *configurator) override;
  const QString & GetName() const override;

  // IModule
  ModuleInitializationResult Initalize() override;
  unsigned int MajorVersion() const override;
  unsigned int MinorVersion() const override;
  void GetRequiredModules(QList<RequiredModuleParams> & out_required_modules) override;
  void SetRequiredModules(const QList<IModule *> &modules) override;
  int GetWeight() const override;

  // CameraModule
  virtual void RegisterWatcher(CameraWatcher *watcher);
  virtual void UnRegisterWatcher(CameraWatcher *watcher);
  virtual QVector<const io::video::CameraMeta *> GetCameras();
  void Configure( QSettings & settings) override;
 private:
  io::video::V4lDriver driver_;
  SettingsModule *settings_module_;

};

}
}
}
#endif //CAMERA_MODULE_H
