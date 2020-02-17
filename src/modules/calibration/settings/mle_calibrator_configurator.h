//
// Created by vahagn on 1/17/20.
//

#ifndef GAGO_UI_MLE_CALIBRATOR_CONFIGURATOR_H
#define GAGO_UI_MLE_CALIBRATOR_CONFIGURATOR_H

#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QDir>

#include "modules/settings/iconfigurator.h"

namespace gago {
namespace gui {
namespace configuration {

class MLECalibratorSettings;

class MLECalibratorConfigurator : public IConfigurator {
 public:
  MLECalibratorConfigurator(class MLECalibratorSettings * settings);
  void DrawConfigurationPage(QWidget *widget) override;
  void Apply() override;
  const QString & ConfigWindowName() const override;
  bool CalibrateCamerasSeparately() const;
  int CaptureWaitTime() const;
  QDir ImageSaveFolder() const;
  bool FixAspectRatio() const;
  bool SoundEnabled() const;
 private:
  class MLECalibratorSettings * settings_;

  QCheckBox *camera_first_chkbx_;
  QLineEdit *folder_line_edit_;
  QSpinBox *wait_time_spinbox_;
  QCheckBox *sound_enabled_chkbx_;

  const QString window_name_ = "MLE";

};

}
}
}
#endif //GAGO_UI_MLE_CALIBRATOR_H
