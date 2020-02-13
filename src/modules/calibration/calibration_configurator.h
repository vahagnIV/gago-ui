//
// Created by vahagn on 1/15/20.
//

#ifndef GAGO_UI_CALIBRATION_CONFIGURATOR_H
#define GAGO_UI_CALIBRATION_CONFIGURATOR_H

#include <QObject>
#include <QFrame>
#include <QComboBox>
#include <QGroupBox>

#include "modules/settings/iconfigurator.h"
#include "calib_pattern_configurator_factory.h"
#include "calibrator_type.h"

namespace gago {
namespace gui {
namespace configuration {

struct CalibrationConfiguration {
  CalibrationPatternType calib_pattern_type;
  CalibratorType calibrator_type;
};

class CalibrationConfigurator : public QObject, public IConfigurator {
 Q_OBJECT
 public:
  CalibrationConfigurator(QObject *parent = nullptr);

  void DrawConfigurationPage(QWidget *widget) override;
  void Apply() override;
  void GetConfiguration(nlohmann::json & out_json) override;
  void SetConfiguration(const nlohmann::json & json) override;
  const QString & ConfigWindowName() const override;
  virtual ~CalibrationConfigurator();

  IConfigurator *GetActivePatternConfigurator();
  IConfigurator *GetActiveCalibratorConfigurator();
 private slots:
  void CalibrationPatternChanged(int idx);
  void CalibratorChanged(int idx);

 private:
  void DrawPatternSide();
  void DrawCalibratorSide();
  QString window_name = "Calibration";
  CalibrationConfiguration current_calibration_settings_;

  QGroupBox *calibrator_group_box_;

  // Calib pattern
  std::vector<IConfigurator *> calib_pattern_configurators_;
  std::vector<QFrame *> calib_pattern_configurator_frames_;
  QGroupBox *pattern_group_box_ = nullptr;
  QComboBox *patterns_combo;

  //Calibrator
  std::vector<IConfigurator *> calibrator_configurators_;
  std::vector<QFrame *> calibrator_configurator_frames_;
  QComboBox * calibrator_combo_;

};

}
}
}
#endif //GAGO_UI_CALIBRATION_CONFIGURATOR_H
