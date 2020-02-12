//
// Created by vahagn on 2/6/20.
//

#ifndef GAGO_UI_ESTIMATION_STATE_H
#define GAGO_UI_ESTIMATION_STATE_H
namespace gago {
namespace calibration {

enum EstimationState {
  PES_Unestimated = 1,
  PES_Extracted = 2,
  PES_Calibrated = 3,
  PES_Broken = 4,
  PES_Disabled = 5
};

}
}

#endif //GAGO_UI_ESTIMATION_STATE_H
