//
// Created by vahagn on 1/18/20.
//

#include "checkerboard_pattern.h"

namespace gago {
namespace calibration {
namespace pattern {

CheckerboardPattern::CheckerboardPattern(unsigned nx, unsigned ny, float square_size) : pattern_size_(nx, ny),
                                                                                        square_size_(square_size) {

}

void CheckerboardPattern::GetObjectPoints(std::vector<std::vector<cv::Point3f>> & out_object_points,
                                          int n_copies) const {
  out_object_points.resize(0);
  std::vector<cv::Point3f> corners;
  for (int i = 0; i < pattern_size_.height; i++)
    for (int j = 0; j < pattern_size_.width; j++)
      corners.push_back(cv::Point3f(float(j * square_size_),
                                    float(i * square_size_), 0));
  for (int i = 0; i < n_copies; ++i) {
    out_object_points.push_back(corners);
  }
}

bool CheckerboardPattern::Extract(const std::vector<cv::Mat> & image,
                                  std::vector<std::vector<cv::Point2f>> & out_detected_points, bool subpix) const {

  out_detected_points.resize(image.size());
  for (int i = 0; i < image.size(); ++i) {
    out_detected_points[i].resize(0);

    if (!cv::findChessboardCorners(image[i], pattern_size_, out_detected_points[i]))
      return false;
    if (subpix)
      cv::cornerSubPix(image[i],
                       out_detected_points[i],
                       cv::Size(11, 11),
                       cv::Size(-1, -1),
                       cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 30, 0.0001));
  }
  return true;
}

void CheckerboardPattern::DrawPattern(cv::Mat & out_image, const std::vector<cv::Point2f> & points) const {
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