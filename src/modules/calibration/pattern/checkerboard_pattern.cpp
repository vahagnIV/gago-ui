//
// Created by vahagn on 1/18/20.
//

#include "checkerboard_pattern.h"

namespace gago {
namespace calibration {
namespace pattern {

CheckerboardPattern::CheckerboardPattern(unsigned nx, unsigned ny) : pattern_size_(nx, ny) {

}

void CheckerboardPattern::Extract(const std::vector<cv::Mat> & image,
                                  std::vector<std::vector<cv::Point2f>> & out_detected_points) {

  out_detected_points.resize(image.size());
  for (int i = 0; i < image.size(); ++i) {
    out_detected_points[i].resize(0);
    cv::findChessboardCorners(image[i], pattern_size_, out_detected_points[i]);
  }
}

void CheckerboardPattern::DrawPattern(cv::Mat & out_image, const std::vector<cv::Point2f> & points) {
  cv::drawChessboardCorners(out_image, pattern_size_, points, !points.empty());
}

const cv::Size & CheckerboardPattern::GetSize() const {
  return pattern_size_;
}

Pattern CheckerboardPattern::GetType() const {
  return CHESSBOARD;
}

}
}
}