//
// Created by vahagn on 1/26/20.
//

#ifndef GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_OPEN_CV_MLE_H_
#define GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_OPEN_CV_MLE_H_

#include <opencv2/opencv.hpp>
#include "pattern/ipattern.h"
#include "mle_configuration_settings.h"
#include "intrinsic_parameters.h"
#include "pattern/pattern_estimation_parameters.h"
#include "calibration_estimates.h"
#include <QList>

namespace gago {
namespace calibration {

enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };

class OpenCvMLE {
 public:
  OpenCvMLE(const std::shared_ptr<pattern::IPattern> &pattern,
            const gago::gui::calibration::MLEConfigurationSettings &settings);

  int Calibrate(const QList<QStringList> &files, CalibrationEstimates &out_estimates,
                QList<PatternEstimationParameters> &pattern_estimation_parameters,
                QList<int> &out_batch_idx);

 protected:
  /*!
   * Calculates the image points from the images located at "files" with shape [batch_id][camera_id].
   * @param files vector of paths to the images in the shape [batch_id][camera_id]
   * @param out_image_sizes The sizes of images per camera in the shape [camera_id]
   * @param out_image_points output vector of image points in the shape [camera_id][batch_id][point_id]
   * @return 0 on success or the error code
   */
  int GetImagePoints(const QList<QStringList> &files,
                     std::vector<cv::Size> &out_image_sizes,
                     std::vector<std::vector<std::vector<cv::Point2f>>> &out_image_points,
                     QList<int> &out_batch_idx) const;

  /*!
   * Finds the intrinsic parameters of a single camera
   * @param image_points detected points on the pattern
   * @param object_points 3D known points of the pattern
   * @param imageSize The size of images
   * @param boardSize The size of the pattern board
   * @param aspectRatio
   * @param grid_width
   * @param release_object
   * @param flags
   * @param intrinsic_parameters
   * @param pattern_parameters
   * @param newObjPoints
   * @param totalAvgErr
   * @return 0 if the calibration successfull or an error code
   */
  int CalibrateSeparateCamera(const std::vector<std::vector<cv::Point2f> > &image_points,
                              const std::vector<std::vector<cv::Point3f> > &object_points,
                              cv::Size imageSize,
                              cv::Size boardSize,
                              float aspectRatio,
      //float grid_width,
                              bool release_object,
                              int flags,
                              IntrinsicParameters &intrinsic_parameters,
                              PatternEstimationParameters &pattern_parameters,
                              std::vector<cv::Point3f> &newObjPoints);

  double ComputeReprojectionErrors(
      const std::vector<std::vector<cv::Point3f> > &object_points,
      const std::vector<std::vector<cv::Point2f> > &image_points,
      const std::vector<cv::Mat> &rvecs,
      const std::vector<cv::Mat> &tvecs,
      IntrinsicParameters &intrinsic_parameters,
      std::vector<float> &perViewErrors);

  std::shared_ptr<pattern::IPattern> pattern_;
  gago::gui::calibration::MLEConfigurationSettings settings_;

};

}
}

#endif //GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_OPEN_CV_MLE_H_
