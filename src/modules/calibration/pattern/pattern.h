//
// Created by vahagn on 1/28/20.
//

#ifndef GAGO_UI_PATTERN_H
#define GAGO_UI_PATTERN_H
#include <opencv2/opencv.hpp>
#include <QStringList>
#include "estimation_state.h"

namespace gago {
namespace calibration {

struct Pattern {
  Pattern() {}
  Pattern(const QString & filename) : filename(filename), state(PES_Unestimated) {}
  Pattern(const cv::Mat & rvecs, const cv::Mat & tvecs, float rpj_error)
      : rotation_vectors(rvecs), translation_vectors(tvecs), reprojection_error(rpj_error) {}
  cv::Mat rotation_vectors; // Rodrigues parametrization
  cv::Mat translation_vectors;
  float reprojection_error;
  QString filename;
  EstimationState state;
  bool enabled = true;
  cv::Size image_size;
};

}
}

#endif //GAGO_UI_PATTERN_H
