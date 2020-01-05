#ifndef CAMERA_MODULE_H
#define CAMERA_MODULE_H

#include "../../imodule.h"
#include <modules/settings/iconfigurable.h>
#include "io/video/linux/v4l_driver.h"
namespace gago {
namespace gui {
namespace modules {

class CameraModule : public IModule, public configuration::IConfigurable {

 public:
  CameraModule();

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
 private:
  io::video::V4lDriver driver_;
};

}
}
}
#endif //CAMERA_MODULE_H
