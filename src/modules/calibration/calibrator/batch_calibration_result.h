//
// Created by vahagn on 2/5/20.
//

#ifndef GAGO_UI_BATCH_CALIBRATION_RESULT_H
#define GAGO_UI_BATCH_CALIBRATION_RESULT_H

#include "pattern/pattern_estimation_parameters.h"
#include "estimation_state.h"
#include <QList>

namespace gago {
namespace calibration {

struct BatchCalibrationResult {
  BatchCalibrationResult() : enabled(false), state(PES_Unestimated) {}

  BatchCalibrationResult(const QStringList & file_paths) : enabled(false), state(PES_Unestimated) {
    for (const QString & file_path: file_paths)
      pattern_params.append(PatternEstimationParameters(file_path));
  }

  QList<PatternEstimationParameters> pattern_params;
  bool enabled;
  EstimationState state;
  double rms;
};

}
}
#endif //GAGO_UI_BATCH_CALIBRATION_RESULT_H
