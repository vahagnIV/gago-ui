//
// Created by vahagn on 1/28/20.
//

#ifndef GAGO_UI_INTRINSIC_PARAMETERS_H
#define GAGO_UI_INTRINSIC_PARAMETERS_H
#include <opencv2/opencv.hpp>

namespace gago {
namespace calibration {

struct IntrinsicParameters {
  cv::Mat camera_matrix;
  cv::Mat distortion_coefficients;
  double rms;
  double total_average_error;
};

}
}

#endif //GAGO_UI_INTRINSIC_PARAMETERS_H
