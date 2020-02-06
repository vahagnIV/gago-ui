//
// Created by vahagn on 2/6/20.
//

#ifndef GAGO_UI_ESTIMATION_STATE_H
#define GAGO_UI_ESTIMATION_STATE_H
namespace gago {
namespace calibration {

enum EstimationState {
  PES_Unestimated,
  PES_Extracted,
  PES_Calibrated,
  PES_Broken
};

}
}

#endif //GAGO_UI_ESTIMATION_STATE_H
