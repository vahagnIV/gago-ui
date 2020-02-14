//
// Created by vahagn on 2/14/20.
//

#ifndef GAGO_UI_CALIBRATION_SETTINGS_H
#define GAGO_UI_CALIBRATION_SETTINGS_H

#include "modules/settings/iconfigurable.h"
#include "calibrator_type.h"
#include "calibration_pattern_type.h"

namespace gago {
namespace gui {
namespace configuration {

class CalibrationSettings : public IConfigurable {
 public:
  CalibrationSettings();
  IConfigurator *GetConfigurator() override;
  void DisposeConfigurator(IConfigurator *configurator) override;
  void ApplyConfiguration(QSettings & settings, IConfigurator *configurator) override;
  void Configure(QSettings & settings) override;
  const QString & GetName() const override;
  CalibratorType GetCalibratorType() const;
  CalibrationPatternType GetCalibrationPatternType() const;
  QSharedPointer<IConfigurable> CalibratorSettings(CalibratorType type) const;
  QSharedPointer<IConfigurable> PatternSettings(CalibrationPatternType type) const;
 private:
  QString name_ = "Calibration";
  CalibratorType calibrator_type_;
  CalibrationPatternType calibration_pattern_type_;
  QList<QSharedPointer<IConfigurable>> calibrator_configurables_;
  QList<QSharedPointer<IConfigurable>> pattern_configurables_;

};

}
}
}
#endif //GAGO_UI_CALIBRATION_SETTINGS_H
