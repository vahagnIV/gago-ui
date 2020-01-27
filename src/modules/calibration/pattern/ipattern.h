//
// Created by vahagn on 1/18/20.
//

#ifndef LIBGAGO_SRC_CALIBRATION_PATTERN_I_PATTERN_H_
#define LIBGAGO_SRC_CALIBRATION_PATTERN_I_PATTERN_H_

#include <opencv2/opencv.hpp>

namespace gago {
namespace calibration {
enum Pattern { CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };

namespace pattern {

class IPattern {
 public:
  virtual void Extract(const std::vector<cv::Mat> &image,
                       std::vector<std::vector<cv::Point2f>> &out_detected_points) = 0;

  virtual void DrawPattern(cv::Mat &out_matrix, const std::vector<cv::Point2f> &points) = 0;

  virtual const cv::Size & GetSize() const = 0;
  virtual Pattern GetType() const = 0;


};

}
}
}

#endif //LIBGAGO_SRC_CALIBRATION_PATTERN_I_PATTERN_H_
