//
// Created by vahagn on 1/19/20.
//

#include <memory>
#include <calibrator_type.h>
#include "calibrator_factory.h"
#include "../pattern/calibration_pattern_factory.h"
#include "mle_calibrator.h"
#include "../mle_calibrator_configurator.h"
namespace gago {
namespace gui {
namespace calibration {

std::shared_ptr<calibration::ICalibrator> CalibratorFactory::Create(configuration::CalibrationConfigurator *configurator,
                                                                    QWidget *main_window) {
  configuration::IConfigurator *pattern_configurator = configurator->GetActivePatternConfigurator();
  std::shared_ptr<gago::calibration::pattern::IPattern>
      calibration_pattern = gago::calibration::pattern::CalibrationPatternFactory::Create(pattern_configurator);

  configuration::CalibratorType type;
  if (try_parse(configurator->GetActiveCalibratorConfigurator()->ConfigWindowName(), type))
    switch (type) {
      case configuration::CalibratorType::MLE_Calibrator: {
        return std::make_shared<MLECalibrator>(main_window,
                                               calibration_pattern,
                                               ((configuration::MLECalibratorConfigurator *) configurator->GetActiveCalibratorConfigurator())->GetSettings());
      }
    }

  return nullptr;
}

}
}
}