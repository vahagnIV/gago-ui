#ifndef CALIBRATION_MODULE_H
#define CALIBRATION_MODULE_H

#include "../../imodule.h"
#include "modules/settings/iconfigurable.h"
#include "pattern/ipattern.h"
#include "modules/camera/camera_module.h"
namespace gago {
namespace gui {
namespace modules {

class CalibrationModule : public QObject,  public IModule, public configuration::IConfigurable {
  Q_OBJECT

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
 private slots:
  void Calibrate();
 private:
  // Since we cn plug and unplug different calibration patterns and calibrators,
  // and we want to remeber the configurations even for unused patterns, we have
  // to keep the configuration loaded
  nlohmann::json settings_;
  QWidget * main_window_;
  CameraModule * camera_module_;

};

}
}
}
#endif //CALIBRATION_MODULE_H
