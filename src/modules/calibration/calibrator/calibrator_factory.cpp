//
// Created by vahagn on 1/19/20.
//


#include <settings/mle_calibrator_settings.h>
#include "calibrator_factory.h"
#include "settings/calibration_settings.h"
#include "settings/checkerboard_pattern_settings.h"
#include "mle_calibrator.h"
#include "pattern/checkerboard_pattern.h"

namespace gago {
namespace gui {
namespace calibration {

QSharedPointer<calibration::ICalibrator> CalibratorFactory::Create(configuration::CalibrationSettings *settings,
                                                                   QWidget *main_window) {
  configuration::CalibrationPatternType pattern_type = settings->GetCalibrationPatternType();
  QSharedPointer<gago::calibration::pattern::IPattern> pattern = nullptr;
  if (configuration::Checkerboard == pattern_type) {
    QSharedPointer<gago::gui::configuration::CheckerboardPatternSettings>
        checkerboard_settings =
        settings->PatternSettings(pattern_type).dynamicCast<gago::gui::configuration::CheckerboardPatternSettings>();
    pattern = QSharedPointer<gago::calibration::pattern::CheckerboardPattern>::create(checkerboard_settings);
  }

  if (nullptr == pattern)
    return nullptr;

  configuration::CalibratorType calibrator_type = settings->GetCalibratorType();
  if (configuration::MLE_Calibrator == calibrator_type) {
    QSharedPointer<gago::gui::configuration::MLECalibratorSettings> calibrator_settings =
        settings->CalibratorSettings(calibrator_type).dynamicCast<gago::gui::configuration::MLECalibratorSettings>();
    return QSharedPointer<MLECalibrator>::create(main_window, pattern, calibrator_settings);
  }

  return nullptr;
}

}
}
}