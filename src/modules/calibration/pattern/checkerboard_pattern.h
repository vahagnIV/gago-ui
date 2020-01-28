//
// Created by vahagn on 1/18/20.
//

#ifndef LIBGAGO_SRC_CALIBRATION_PATTERN_CHECKERBOARD_PATTERN_H_
#define LIBGAGO_SRC_CALIBRATION_PATTERN_CHECKERBOARD_PATTERN_H_

#include "ipattern.h"

namespace gago {
namespace calibration {
namespace pattern {

class CheckerboardPattern : public IPattern {
 public:
  CheckerboardPattern(unsigned nx, unsigned ny, float square_size = 1);
  bool Extract(const std::vector<cv::Mat> & image,
               std::vector<std::vector<cv::Point2f>> & out_detected_points, bool subpix = false) const override;
  void DrawPattern(cv::Mat & out_image, const std::vector<cv::Point2f> & points) const override;
  const cv::Size & GetSize() const override;
  Pattern GetType() const override;
  void GetObjectPoints(std::vector<std::vector<cv::Point3f>> & out_object_points, int n_copies) const override;
 private:
  cv::Size pattern_size_;
  float square_size_;

};

}
}
}
#endif //LIBGAGO_SRC_CALIBRATION_PATTERN_CHECKERBOARD_PATTERN_H_
