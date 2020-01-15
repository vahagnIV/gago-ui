#ifndef CALIBRATION_MODULE_H
#define CALIBRATION_MODULE_H

#include "../../imodule.h"
#include "modules/settings/iconfigurable.h"

namespace gago {
namespace gui {
namespace modules {

class CalibrationModule : public IModule, public configuration::IConfigurable {

 public:
  CalibrationModule();

  // IModule
  ModuleInitializationResult Initalize() override;
  unsigned int MajorVersion() const override;
  unsigned int MinorVersion() const override;
  void QRequiredModules(std::vector<RequiredModuleParams> & out_required_modules) override;
  void SetRequiredModules(const std::vector<IModule *> & modules) override;

  // IConfigurable
  configuration::IConfigurator *GetConfigurator() override;
  void DisposeConfigurator(configuration::IConfigurator *configurator) override;
  void ApplyConfiguration(configuration::IConfigurator *configurator) override;

  // CalibrationModule
};

}
}
}
#endif //CALIBRATION_MODULE_H
