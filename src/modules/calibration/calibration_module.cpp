#include <modules/settings/settings_module.h>
#include "calibration_module.h"
#include "modules/main/main_module.h"
#include "calibration_configurator.h"
#include "pattern/calibration_pattern_factory.h"
#include <QAction>
#include "calibrator/calibrator_factory.h"

namespace gago {
namespace gui {

namespace modules {

CalibrationModule::CalibrationModule() : QObject(nullptr), IModule("Calibration", "calibration") {
}

ModuleInitializationResult CalibrationModule::Initalize() {
  return ModuleInitializationResult::OK;
}

unsigned int CalibrationModule::MajorVersion() const {
  return 1;
}

unsigned int CalibrationModule::MinorVersion() const {
  return 0;
}

void CalibrationModule::Calibrate() {
  configuration::CalibrationConfigurator *cnf = new configuration::CalibrationConfigurator();
  if (!settings_.empty())
    cnf->SetConfiguration(settings_);



  // Create calibrator
  std::shared_ptr<calibration::ICalibrator> window = calibration::CalibratorFactory::Create(cnf, main_window_);
  camera_module_->RegisterWatcher(window.get());
  window->Calibrate();
  camera_module_->UnRegisterWatcher(window.get());

}

void CalibrationModule::QRequiredModules(std::vector<RequiredModuleParams> &out_required_modules) {
  out_required_modules.resize(3);

  out_required_modules[0].Name = "main";
  out_required_modules[0].MinMajorVersion = 1;
  out_required_modules[0].MinMinorVersion = 0;

  out_required_modules[1].Name = "settings";
  out_required_modules[1].MinMajorVersion = 1;
  out_required_modules[1].MinMinorVersion = 0;

  out_required_modules[2].Name = "camera";
  out_required_modules[2].MinMajorVersion = 1;
  out_required_modules[2].MinMinorVersion = 0;
}

void CalibrationModule::SetRequiredModules(const std::vector<IModule *> &modules) {
  for (IModule *module: modules) {
    if (module->SystemName() == "main") {
      main_window_ = ((MainModule *) module)->MainWindow();
      QAction *action = ((MainModule *) module)->CreateMenuBranch("/File/Calibration/Calibrate");
      connect(action, &QAction::triggered, this, &CalibrationModule::Calibrate);
      QAction *saveAction = ((MainModule *) module)->CreateMenuBranch("/File/Calibration/Save As/Opencv.yml");
      saveAction->setEnabled(false);
    } else if (module->SystemName() == "settings") {
      ((SettingsModule *) module)->RegisterConfigurable(this);
    } else if (module->SystemName() == "camera")
      camera_module_ = (CameraModule *) module;
  }
}

configuration::IConfigurator *CalibrationModule::GetConfigurator() {
  configuration::IConfigurator *cnf = new configuration::CalibrationConfigurator();
  if (!settings_.empty())
    cnf->SetConfiguration(settings_);
  return cnf;
}

void CalibrationModule::DisposeConfigurator(configuration::IConfigurator *configurator) {
  configuration::CalibrationConfigurator *cnf = (configuration::CalibrationConfigurator *) configurator;
  delete cnf;
}

void CalibrationModule::ApplyConfiguration(configuration::IConfigurator *configurator) {
  configurator->GetConfiguration(settings_);
  //configuration::CalibrationConfigurator *cnf = (configuration::CalibrationConfigurator *) configurator;

}

}
}
}
