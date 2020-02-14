//
// Created by vahagn on 1/19/20.
//

#ifndef GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_CALIBRATOR_FACTORY_H_
#define GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_CALIBRATOR_FACTORY_H_
#include <QSharedPointer>

#include "settings/calibration_configurator.h"
#include "icalibrator.h"

namespace gago {
namespace gui {
namespace calibration {

class CalibratorFactory {
 public:
  static QSharedPointer<calibration::ICalibrator> Create(configuration::CalibrationSettings * settings, QWidget * main_window);

};

}}}
#endif //GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_CALIBRATOR_FACTORY_H_
