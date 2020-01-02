#ifndef CAMERA_MODULE_H
#define CAMERA_MODULE_H

#include "../../imodule.h"
#include <modules/settings/iconfigurable.h>
#include "io/video/v4l_driver.h"
namespace gago {
namespace gui {
namespace modules {

class CameraModule : public IModule, public configuration::IConfigurable {

 public:
  void ApplyConfiguration(configuration::IConfigurator *configurator) override;
  CameraModule();

  // IModule
  ModuleInitializationResult Initalize() override;
  unsigned int MajorVersion() const override;
  unsigned int MinorVersion() const override;
  void QRequiredModules(std::vector<RequiredModuleParams> &out_required_modules) override;
  void SetRequiredModules(const std::vector<IModule *> &modules) override;
  configuration::IConfigurator *GetConfigurator() override;
  void DisposeConfigurator(configuration::IConfigurator *configurator) override;
 private:
  io::video::V4lDriver driver_;
  std::vector<io::video::CameraDeviceInfo> cameras_;

  // CameraModule
};

}
}
}
#endif //CAMERA_MODULE_H
