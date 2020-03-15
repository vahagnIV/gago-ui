//
// Created by vahagn on 1/28/20.
//

#ifndef GAGO_UI_CALIBRATION_ESTIMATES_H
#define GAGO_UI_CALIBRATION_ESTIMATES_H
#include "intrinsic_parameters.h"
#include <QList>

namespace gago {
namespace calibration {

struct CalibrationEstimates {
  QList<IntrinsicParameters> intrinsic_parameters;

  cv::Mat R; // Rotation matrix
  cv::Mat T; // Translation vector
  cv::Mat E; // ?
  cv::Mat F; // Fundamental matrix
  double rms;

};

}
}

#endif //GAGO_UI_CALIBRATION_ESTIMATES_H
