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

enum DistModel {
  BARREL5,
  KANNALA_BRANDT
};

class MLECalibratorSettings : public IConfigurable {
 public:
  // IConfigurable
  IConfigurator * GetConfigurator() override;
  void DisposeConfigurator(IConfigurator * configurator) override;
  void ApplyConfiguration(QSettings & settings, IConfigurator * configurator) override;
  void Configure(QSettings & settings) override;
  const QString & GetName() const override;
  bool CalibrateCamerasSeparately() const;
  int CaptureWaitTime() const;
  const QDir & ImageSaveFolder() const;
  bool FixAspectRatio() const;
  bool SoundEnabled() const;
  bool LoopCapture() const;
  DistModel DistortionModel() const;

 private:
  const QString name_ = "MLE";
  bool calibrate_cameras_separately_ = true;
  int capture_wait_time_ = 0;
  QDir image_save_folder_;
  bool sounds_enabled_ = true;
  bool fix_aspect_ratio_ = false;
  bool loop_capture_ = false;
  DistModel distortion_model_;

};

}
}
}

#endif //GAGO_UI_MLE_CALIBRATOR_SETTINGS_H
