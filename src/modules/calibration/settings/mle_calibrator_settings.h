//
// Created by vahagn on 2/14/20.
//

#ifndef GAGO_UI_MLE_CALIBRATOR_SETTINGS_H
#define GAGO_UI_MLE_CALIBRATOR_SETTINGS_H
#include <QDir>
#include "modules/settings/iconfigurable.h"

namespace gago {
namespace gui {
namespace configuration {

class MLECalibratorSettings : public IConfigurable {
 public:
  // IConfigurable
  IConfigurator *GetConfigurator() override;
  void DisposeConfigurator(IConfigurator *configurator) override;
  void ApplyConfiguration(QSettings & settings, IConfigurator *configurator) override;
  void Configure(QSettings & settings) override;
  const QString & GetName() const override;
  bool CalibrateCamerasSeparately() const;
  int CaptureWaitTime() const;
  const QDir & ImageSaveFolder() const;
  bool FixAspectRatio() const;

 private:
  const QString name_ = "MLE";
  bool calibrate_cameras_separately_;
  int capture_wait_time_;
  QDir image_save_folder_;
  bool sounds_enabled_;
  bool fix_aspect_ratio_ = false;

};

}
}
}

#endif //GAGO_UI_MLE_CALIBRATOR_SETTINGS_H
