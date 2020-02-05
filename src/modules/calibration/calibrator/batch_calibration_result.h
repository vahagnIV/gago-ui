//
// Created by vahagn on 2/5/20.
//

#ifndef GAGO_UI_BATCH_CALIBRATION_RESULT_H
#define GAGO_UI_BATCH_CALIBRATION_RESULT_H

#include "pattern/pattern_estimation_parameters.h"
#include <QList>

namespace gago {
namespace calibration {

struct BatchCalibrationResult {
  BatchCalibrationResult(){}

  BatchCalibrationResult(const QStringList & file_paths) {
    for (const QString & file_path: file_paths)
      pattern_params.append(PatternEstimationParameters(file_path));
  }

  QList<PatternEstimationParameters> pattern_params;
  double rms;
};

}
}
#endif //GAGO_UI_BATCH_CALIBRATION_RESULT_H
