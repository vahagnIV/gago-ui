//
// Created by vahagn on 1/26/20.
//

#ifndef GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_OPEN_CV_MLE_H_
#define GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_OPEN_CV_MLE_H_
#include <QList>
#include <QSharedPointer>

#include <opencv2/opencv.hpp>
#include "pattern/ipattern.h"
#include "intrinsic_parameters.h"
#include "pattern/pattern_estimation_parameters.h"
#include "batch_calibration_result.h"

namespace gago {
namespace calibration {

enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };

class OpenCvMLE {
 public:
  OpenCvMLE(const QSharedPointer<pattern::IPattern> & pattern,
            bool calibrate_cameras_separately);

  int Calibrate(QList<BatchCalibrationResult> & out_batch_calibration_results, CalibrationEstimates & out_estimates);

 protected:

  int GetImagePoints(QList<BatchCalibrationResult> & out_batch_calibration_results,
                     std::vector<std::vector<std::vector<cv::Point2f>>> & out_image_points);

  int CalibrateSingleCamera(QList<BatchCalibrationResult> & out_batch_calibration_results,
                            int cam_idx,
                            const std::vector<std::vector<std::vector<cv::Point2f>>> & image_points,
                            const cv::Size & boardSize,
                            float aspectRatio,
      //float grid_width,
                            bool release_object,
                            int flags,
                            IntrinsicParameters & out_intrinsic_parameters,
                            std::vector<cv::Point3f> & newObjPoints);
  int Calibrate2Cameras(QList<BatchCalibrationResult> & out_batch_calibration_results,
                        const std::vector<std::vector<std::vector<cv::Point2f>>> & image_points,
                        const cv::Size & boardSize, bool fix_intrinsic, CalibrationEstimates & out_estimates);

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
   * @param pattern_parameters shape [batch_id][camera_id]
   * @param newObjPoints
   * @param totalAvgErr
   * @return 0 if the calibration successfull or an error code
   */


  void ComputeSingleCamReprojectionError(const std::vector<cv::Point3f> & object_points,
                                         const std::vector<cv::Point2f> & image_points,
                                         const IntrinsicParameters & intrinsic_parameters,
                                         PatternEstimationParameters & out_pattern_params);

  void ComputeStereoRigReprojectionError(const std::vector<cv::Point3f> & object_points,
                                         const std::vector<std::vector<cv::Point2f>> & image_points,
                                         const CalibrationEstimates & estimates,
                                         BatchCalibrationResult & out_result);

  QSharedPointer<pattern::IPattern> pattern_;
  bool calibrate_cameras_separately_;

};

}
}

#endif //GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_OPEN_CV_MLE_H_
