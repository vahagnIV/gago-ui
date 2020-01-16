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
 private:
  // Since we cn plug and unplug different calibration patterns and calibrators,
  // and we want to remeber the configurations even for unused patterns, we have
  // to keep the configuration loaded
  nlohmann::json settings_;

  // CalibrationModule
};

}
}
}
#endif //CALIBRATION_MODULE_H
