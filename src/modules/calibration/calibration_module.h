#ifndef CALIBRATION_MODULE_H
#define CALIBRATION_MODULE_H

#include "../../imodule.h"
#include "pattern/ipattern.h"
#include "modules/settings/iconfigurable.h"
#include "modules/camera/camera_module.h"
#include "calibration_estimates.h"
#include <QDir>

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
  int GetWeight() const override;
  void Start() override ;

  // IConfigurable
  configuration::IConfigurator *GetConfigurator() override;
  void DisposeConfigurator(configuration::IConfigurator *configurator) override;
  void ApplyConfiguration(configuration::IConfigurator *configurator) override;
  // CalibrationModule
 private slots:
  void Calibrate();
  void SaveParamsToFolder();
 private:
  QDir GetParamSaveFolder(QVector<const io::video::CameraMeta*> cameras);
  void SaveEstimatesAsOpenCvYml(QDir folder);
  void LoadEstimatesFromOpenCvYml(QDir folder);
  gago::calibration::CalibrationEstimates estimates_;
  // Since we can plug and unplug different calibration patterns and calibrators,
  // and we want to remeber the configurations even for unused patterns, we have
  // to keep the configuration loaded
  nlohmann::json settings_;
  QWidget * main_window_;
  CameraModule * camera_module_;
  QAction *save_action_;

};

}
}
}
#endif //CALIBRATION_MODULE_H
