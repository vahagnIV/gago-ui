#ifndef CALIBRATION_MODULE_H
#define CALIBRATION_MODULE_H

#include <QDir>
#include "imodule.h"
#include "pattern/ipattern_extractor.h"
#include "modules/settings/iconfigurable.h"
#include "modules/camera/camera_module.h"
#include "calibration_estimates.h"
#include "settings/calibration_settings.h"

namespace gago {
namespace gui {
namespace modules {

class CalibrationModule : public QObject,  public IModule {
  Q_OBJECT

 public:
  CalibrationModule();

  // IModule
  ModuleInitializationResult Initalize() override;
  unsigned int MajorVersion() const override;
  unsigned int MinorVersion() const override;
  void GetRequiredModules(QList<RequiredModuleParams> & out_required_modules) override;
  void SetRequiredModules(const QList<IModule *> & modules) override;
  int GetDestructorIndex() const override;
  int GetStartIndex() const override ;
  void Start() override ;

  // CalibrationModule
 private slots:
  void Calibrate();
  void SaveParamsToFolder();
 private:
  QDir GetParamSaveFolder(QVector<const io::video::CameraMeta*> cameras);
  void SaveEstimatesAsOpenCvYml(QDir folder);
  void LoadEstimatesFromOpenCvYml(QDir folder);
  gago::calibration::CalibrationEstimates estimates_;

  QWidget * main_window_;
  CameraModule * camera_module_;
  QAction *save_action_;
  configuration::CalibrationSettings settings_;

};

}
}
}
#endif //CALIBRATION_MODULE_H
