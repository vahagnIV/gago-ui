//
// Created by vahagn on 1/17/20.
//

#ifndef GAGO_UI_CONSTS_H
#define GAGO_UI_CONSTS_H

#include <QList>
#include "calibrator_type.h"
#include "calibration_pattern_type.h"

namespace gago {
namespace gui {
namespace configuration {

const QList<CalibrationPatternType> SupportedPatterns = {Checkerboard};
const QList<CalibratorType> SupportedCalibrators = {MLE_Calibrator};

}
}
}
#endif //GAGO_UI_CONSTS_H
