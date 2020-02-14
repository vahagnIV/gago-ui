//
// Created by vahagn on 2/14/20.
//

#include "calibration_settings.h"
#include "calibration_configurator.h"
#include "consts.h"
#include "checkerboard_pattern_settings.h"
#include "mle_calibrator_settings.h"

namespace gago {
namespace gui {
namespace configuration {

CalibrationSettings::CalibrationSettings() : calibration_pattern_type_(Checkerboard), calibrator_type_(MLE_Calibrator) {
  for (CalibrationPatternType pattern_type: SupportedPatterns)
    switch (pattern_type) {
      case Checkerboard:
        pattern_configurables_.append(QSharedPointer<CheckerboardPatternSettings>::create());
        break;
    }

  for (CalibratorType calibrator_type: SupportedCalibrators)
    switch (calibrator_type) {
      case MLE_Calibrator:
        calibrator_configurables_.append(QSharedPointer<MLECalibratorSettings>::create());
        break;
    }
}

IConfigurator *CalibrationSettings::GetConfigurator() {
  QList<IConfigurator *> pattern_configurators;
  for (QSharedPointer<IConfigurable> & pattern_settings: pattern_configurables_)
    pattern_configurators.append(pattern_settings->GetConfigurator());

  QList<IConfigurator *> calibrator_configurators;
  for (QSharedPointer<IConfigurable> & calibrator_settings: calibrator_configurables_)
    calibrator_configurators.append(calibrator_settings->GetConfigurator());

  return new CalibrationConfigurator(pattern_configurators, calibrator_configurators, this);
}

void CalibrationSettings::DisposeConfigurator(IConfigurator *configurator) {
  CalibrationConfigurator *calib_configurator = (CalibrationConfigurator *) configurator;
  for (int cfg_idx = 0; cfg_idx < pattern_configurables_.size(); ++cfg_idx) {
    pattern_configurables_[cfg_idx]->DisposeConfigurator(calib_configurator->GetPatternConfigurators()[cfg_idx]);
  }
  for (int cfg_idx = 0; cfg_idx < calibrator_configurables_.size(); ++cfg_idx) {
    calibrator_configurables_[cfg_idx]->DisposeConfigurator(calib_configurator->GetCalibratorConfigurators()[cfg_idx]);
  }
  delete (CalibrationConfigurator *) configurator;
}

void CalibrationSettings::ApplyConfiguration(QSettings & settings, IConfigurator *configurator) {
  CalibrationConfigurator *calib_configuratpr = (CalibrationConfigurator *) configurator;
  calibration_pattern_type_ = calib_configuratpr->GetSelectedPatternType();
  calibrator_type_ = calib_configuratpr->GetSelectedCalibratorType();
  settings.setValue("SelectedPattern", to_string(GetCalibrationPatternType()));
  settings.setValue("SelectedCalibrator", to_string(GetCalibratorType()));
  settings.beginGroup("Pattern");
  for (int cfg_idx = 0; cfg_idx < pattern_configurables_.size(); ++cfg_idx) {
    QSharedPointer<IConfigurable> pattern_settings = pattern_configurables_[cfg_idx];
    settings.beginGroup(pattern_settings->GetName());
    pattern_settings->ApplyConfiguration(settings, calib_configuratpr->GetPatternConfigurators()[cfg_idx]);
    settings.endGroup();
  }
  settings.endGroup();
  settings.beginGroup("Calibrator");
  for (int cfg_idx = 0; cfg_idx < calibrator_configurables_.size(); ++cfg_idx) {
    QSharedPointer<IConfigurable> calibrator_settings = calibrator_configurables_[cfg_idx];
    settings.beginGroup(calibrator_settings->GetName());
    calibrator_settings->ApplyConfiguration(settings, calib_configuratpr->GetCalibratorConfigurators()[cfg_idx]);
    settings.endGroup();
  }
  settings.endGroup();
}

void CalibrationSettings::Configure(QSettings & settings) {
  if (settings.contains("SelectedPattern"))
    try_parse(settings.value("SelectedPattern").toString(), calibration_pattern_type_);
  if (settings.contains("SelectedCalibrator"))
    try_parse(settings.value("SelectedCalibrator").toString(), calibrator_type_);
  settings.beginGroup("Pattern");
  for (int cfg_idx = 0; cfg_idx < pattern_configurables_.size(); ++cfg_idx) {
    QSharedPointer<IConfigurable> pattern_settings = pattern_configurables_[cfg_idx];
    settings.beginGroup(pattern_settings->GetName());
    pattern_settings->Configure(settings);
    settings.endGroup();
  }
  settings.endGroup();
  settings.beginGroup("Calibrator");
  for (int cfg_idx = 0; cfg_idx < calibrator_configurables_.size(); ++cfg_idx) {
    QSharedPointer<IConfigurable> calibrator_settings = calibrator_configurables_[cfg_idx];
    settings.beginGroup(calibrator_settings->GetName());
    calibrator_settings->Configure(settings);
    settings.endGroup();
  }
  settings.endGroup();
}

const QString & CalibrationSettings::GetName() const {
  return name_;
}

CalibratorType CalibrationSettings::GetCalibratorType() const {
  return calibrator_type_;
}

CalibrationPatternType CalibrationSettings::GetCalibrationPatternType() const {
  return calibration_pattern_type_;
}

QSharedPointer<IConfigurable> CalibrationSettings::CalibratorSettings(CalibratorType type) const {
  for (int calibrator_idx = 0; calibrator_idx < SupportedCalibrators.size(); ++calibrator_idx) {
    if (SupportedCalibrators[calibrator_idx] == type)
      return calibrator_configurables_[calibrator_idx];
  }
  return nullptr;
}

QSharedPointer<IConfigurable> CalibrationSettings::PatternSettings(CalibrationPatternType type) const {
  for (int pattern_idx = 0; pattern_idx < SupportedPatterns.size(); ++pattern_idx) {
    if (SupportedPatterns[pattern_idx] == type)
      return pattern_configurables_[pattern_idx];
  }
  return nullptr;
}

}
}
}