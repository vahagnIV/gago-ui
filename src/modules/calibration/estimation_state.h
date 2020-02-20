//
// Created by vahagn on 2/6/20.
//

#ifndef GAGO_UI_ESTIMATION_STATE_H
#define GAGO_UI_ESTIMATION_STATE_H

#include <QString>

namespace gago {
namespace calibration {

enum EstimationState {
  PES_Unestimated = 1,
  PES_Extracted = 2,
  PES_Calibrated = 3,
  PES_Broken = 4,
  PES_Disabled = 5
};

QString to_string(EstimationState state);

bool try_parse(const QString & state_str, EstimationState & out_state);

}
}

#endif //GAGO_UI_ESTIMATION_STATE_H
