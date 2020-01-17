//
// Created by vahagn on 1/17/20.
//

#ifndef GAGO_UI_CONSTS_H
#define GAGO_UI_CONSTS_H

#include "calibrator_type.h"
#include "calibration_pattern_type.h"

namespace gago {
namespace gui {
namespace configuration {

const CalibrationPatternType SupportedPatterns[1] = {Checkerboard};
const CalibratorType SupportedCalibrators[1] = {MLE_Calibrator};

}
}
}
#endif //GAGO_UI_CONSTS_H
