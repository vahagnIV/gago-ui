//
// Created by vahagn on 1/15/20.
//

#ifndef GAGO_UI_I_CALIBRATOR_H
#define GAGO_UI_I_CALIBRATOR_H

#include "modules/camera/camera_module.h"

namespace gago {
namespace gui {
namespace calibration {

class ICalibrator : public modules::CameraWatcher {
 public:
  virtual void Calibrate() = 0;

};

}
}
}

#endif //GAGO_UI_I_CALIBRATOR_H
