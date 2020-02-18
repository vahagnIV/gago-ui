//
// Created by vahagn on 1/15/20.
//

#ifndef GAGO_UI_I_CALIBRATOR_H
#define GAGO_UI_I_CALIBRATOR_H

#include "modules/camera/camera_module.h"
#include "calibration_estimates.h"

namespace gago {
namespace gui {
namespace calibration {

class ICalibrator : public modules::CameraWatcher {
 public:
  virtual int Calibrate() = 0;
  virtual const gago::calibration::CalibrationEstimates &GetEstimates() const = 0;
  virtual ~ICalibrator() = default;
};

}
}
}

#endif //GAGO_UI_I_CALIBRATOR_H
