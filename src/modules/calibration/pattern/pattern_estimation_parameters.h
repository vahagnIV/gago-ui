//
// Created by vahagn on 1/28/20.
//

#ifndef GAGO_UI_PATTERN_ESTIMATION_PARAMETERS_H
#define GAGO_UI_PATTERN_ESTIMATION_PARAMETERS_H
#include <opencv2/opencv.hpp>

namespace gago {
namespace calibration {

struct PatternEstimationParameters {
  PatternEstimationParameters(const cv::Mat & rvecs, const cv::Mat & tvecs, float rpj_error)
      : rotation_vectors(rvecs), translation_vectors(tvecs), reprojection_error(rpj_error) {}
  cv::Mat rotation_vectors; // Rodrigues parametrization
  cv::Mat translation_vectors; //
  float reprojection_error;
};

}
}

#endif //GAGO_UI_PATTERN_ESTIMATION_PARAMETERS_H
