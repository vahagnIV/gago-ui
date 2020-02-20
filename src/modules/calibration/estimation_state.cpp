//
// Created by vahagn on 2/20/20.
//

#include "estimation_state.h"

namespace gago {
namespace calibration {

QString to_string(EstimationState state) {
  switch (state) {
    case EstimationState::PES_Extracted:
      return "Extracted";
    case EstimationState::PES_Broken:
      return "Broken";
    case EstimationState::PES_Disabled:
      return "Disabled";
    case EstimationState::PES_Unestimated:
      return "Unestimated";
    case EstimationState::PES_Calibrated:
      return "Calibrated";
  }
  return QString();
}

bool try_parse(const QString & state_str, EstimationState & out_state) {
  if (0 == state_str.compare("Extracted", Qt::CaseInsensitive)) {
    out_state = EstimationState::PES_Extracted;
    return true;
  } else if (0 == state_str.compare("Broken", Qt::CaseInsensitive)) {
    out_state = EstimationState::PES_Broken;
    return true;
  } else if (0 == state_str.compare("Disabled", Qt::CaseInsensitive)) {
    out_state = EstimationState::PES_Disabled;
    return true;
  } else if (0 == state_str.compare("Unestimated", Qt::CaseInsensitive)) {
    out_state = EstimationState::PES_Unestimated;
    return true;
  } else if (0 == state_str.compare("Calibrated", Qt::CaseInsensitive)) {
    out_state = EstimationState::PES_Calibrated;
    return true;
  }

  return false;
}

}
}