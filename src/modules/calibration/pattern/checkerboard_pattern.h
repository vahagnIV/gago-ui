//
// Created by vahagn on 1/18/20.
//

#ifndef LIBGAGO_SRC_CALIBRATION_PATTERN_CHECKERBOARD_PATTERN_H_
#define LIBGAGO_SRC_CALIBRATION_PATTERN_CHECKERBOARD_PATTERN_H_

#include <QSharedPointer>
#include "ipattern_extractor.h"
#include "settings/checkerboard_pattern_settings.h"

namespace gago {
namespace calibration {
namespace pattern {

class CheckerboardPattern : public IPatternExtractor {
 public:
  CheckerboardPattern(const QSharedPointer<gago::gui::configuration::CheckerboardPatternSettings > & settings);
  bool Extract(const std::vector<cv::Mat> & image,
               std::vector<std::vector<cv::Point2f>> & out_detected_points, bool subpix = false) const override;
  void DrawPattern(cv::Mat & out_image, const std::vector<cv::Point2f> & points) const override;
  const cv::Size & GetSize() const override;
  PatternType GetType() const override;
  void GetObjectPoints(std::vector<std::vector<cv::Point3f>> & out_object_points, int n_copies) const override;
  bool Extract(const cv::Mat & image, std::vector<cv::Point2f> & out_detected_points, bool subpix) const override;
 private:
  cv::Size pattern_size_;
  float square_size_;

};

}
}
}
#endif //LIBGAGO_SRC_CALIBRATION_PATTERN_CHECKERBOARD_PATTERN_H_
