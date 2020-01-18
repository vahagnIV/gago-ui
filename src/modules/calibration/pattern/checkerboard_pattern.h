//
// Created by vahagn on 1/18/20.
//

#ifndef LIBGAGO_SRC_CALIBRATION_PATTERN_CHECKERBOARD_PATTERN_H_
#define LIBGAGO_SRC_CALIBRATION_PATTERN_CHECKERBOARD_PATTERN_H_

#include "ipattern.h"

namespace gago {
namespace calibration {
namespace pattern {

class CheckerboardPattern: public IPattern {
 public:
  CheckerboardPattern(unsigned nx, unsigned ny);
  void Extract(const std::vector<cv::Mat> &image,
               std::vector<std::vector<cv::Point2f>> &out_detected_points) override;
  void DrawPattern(cv::Mat &out_image, const std::vector<cv::Point2f> &points) override;
 private:
  cv::Size pattern_size_;

};

}
}
}
#endif //LIBGAGO_SRC_CALIBRATION_PATTERN_CHECKERBOARD_PATTERN_H_
