//
// Created by vahagn on 2/5/20.
//

#ifndef GAGO_UI_BATCH_CALIBRATION_RESULT_H
#define GAGO_UI_BATCH_CALIBRATION_RESULT_H

#include "pattern/pattern.h"
#include "estimation_state.h"
#include <QList>

namespace gago {
namespace calibration {

struct PatternBatch {
  PatternBatch() {}

  PatternBatch(const QStringList & file_paths) {
    for (const QString & file_path: file_paths)
      pattern_params.append(Pattern(file_path));
  }

  bool Enabled() {
    return std::all_of(pattern_params.begin(), pattern_params.end(), [](const Pattern & pattern) { return pattern.enabled; });
  }

  EstimationState CombinedPatternState() {
    if (pattern_params.isEmpty())
      return PES_Broken;

    if (std::any_of(pattern_params.begin(),
                    pattern_params.end(),
                    [](Pattern & pattern) { return PES_Broken == pattern.state; }))
      return PES_Broken;

    // All patterns should be in the same state
    if (std::adjacent_find(pattern_params.begin(),
                           pattern_params.end(),
                           [](Pattern & pattern1, Pattern & pattern2) { return pattern2.state == pattern1.state; })
        == pattern_params.end())
      return PES_Broken;

    return pattern_params[0].state;
  }

  EstimationState state = PES_Unestimated;
  QList<Pattern> pattern_params;
  double rms;
};

}
}
#endif //GAGO_UI_BATCH_CALIBRATION_RESULT_H
