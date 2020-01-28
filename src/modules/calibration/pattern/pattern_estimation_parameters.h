//
// Created by vahagn on 1/28/20.
//

#ifndef GAGO_UI_PATTERN_ESTIMATION_PARAMETERS_H
#define GAGO_UI_PATTERN_ESTIMATION_PARAMETERS_H
#include <opencv2/opencv.hpp>

namespace gago {
namespace calibration {

struct PatternEstimationParameters {
  std::vector<cv::Mat> rotation_vectors; // Rodrigues parametrization
  std::vector<cv::Mat> translation_vectors; //
  std::vector<float> reprojection_errors;
};

}
}

#endif //GAGO_UI_PATTERN_ESTIMATION_PARAMETERS_H
