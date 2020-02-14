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
#include "calibration_pattern_type.h"
#include "calibrator_type.h"

namespace gago {
namespace gui {
namespace configuration {
class CalibrationSettings;

class CalibrationConfigurator : public QObject, public IConfigurator {

 Q_OBJECT
 public:
  CalibrationConfigurator(const QList<IConfigurator *> & calib_pattern_configurators,
      const QList<IConfigurator *> & calibrator_configurators,
                          CalibrationSettings * settings     );

  void DrawConfigurationPage(QWidget *widget) override;
  void Apply() override;
  const QString & ConfigWindowName() const override;
  virtual ~CalibrationConfigurator();
  CalibrationPatternType GetSelectedPatternType() const;
  CalibratorType  GetSelectedCalibratorType() const;
  const QList<IConfigurator *> & GetPatternConfigurators() const;
  const QList<IConfigurator *> & GetCalibratorConfigurators() const;
  QList<IConfigurator *> & GetPatternConfigurators() ;
  QList<IConfigurator *> & GetCalibratorConfigurators();
 private slots:
  void CalibrationPatternChanged(int idx);
  void CalibratorChanged(int idx);

 private:
  void DrawPatternSide();
  void DrawCalibratorSide();
  QString window_name = "Calibration";

  // Calib pattern
  QList<IConfigurator *> calib_pattern_configurators_;
  std::vector<QFrame *> calib_pattern_configurator_frames_;
  QGroupBox *pattern_group_box_ = nullptr;
  QComboBox *patterns_combo;

  //Calibrator
  QGroupBox *calibrator_group_box_;
  QList<IConfigurator *> calibrator_configurators_;
  std::vector<QFrame *> calibrator_configurator_frames_;
  QComboBox *calibrator_combo_;

  CalibrationSettings * settings_;

};

}
}
}
#endif //GAGO_UI_CALIBRATION_CONFIGURATOR_H
