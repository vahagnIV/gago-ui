//
// Created by vahagn on 1/28/20.
//

#ifndef GAGO_UI_PATTERN_ESTIMATION_PARAMETERS_H
#define GAGO_UI_PATTERN_ESTIMATION_PARAMETERS_H
#include <opencv2/opencv.hpp>
#include <QStringList>
namespace gago {
namespace calibration {

enum PatternEstimationState {
  PES_Unestimated,
  PES_OK,
  PES_Broken
};

struct PatternEstimationParameters {
  PatternEstimationParameters() {}
  PatternEstimationParameters(const QString & filename) : filename(filename), state(PES_Unestimated) {}
  PatternEstimationParameters(const cv::Mat & rvecs, const cv::Mat & tvecs, float rpj_error)
      : rotation_vectors(rvecs), translation_vectors(tvecs), reprojection_error(rpj_error) {}
  cv::Mat rotation_vectors; // Rodrigues parametrization
  cv::Mat translation_vectors; //
  float reprojection_error;
  QString filename;
  PatternEstimationState state;
};

}
}

#endif //GAGO_UI_PATTERN_ESTIMATION_PARAMETERS_H
