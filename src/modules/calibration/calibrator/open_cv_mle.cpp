//
// Created by vahagn on 1/26/20.
//

#include <calibration_estimates.h>
#include "open_cv_mle.h"
#include <QList>

namespace gago {
namespace calibration {

OpenCvMLE::OpenCvMLE(const QSharedPointer<pattern::IPatternExtractor> & pattern,
                     bool calibrate_cameras_separately)
    : pattern_(pattern), calibrate_cameras_separately_(calibrate_cameras_separately) {

}

enum CalibrationErrors {
  InconsistenSize = 1, // The image size per camera is not equal
  EmptyList = 2
};

void OpenCvMLE::ComputeSingleCamReprojectionError(const std::vector<cv::Point3f> & object_points,
                                                  const std::vector<cv::Point2f> & image_points,
                                                  const IntrinsicParameters & intrinsic_parameters,
                                                  Pattern & out_pattern_params
) {

  std::vector<cv::Point2f> imagePoints2;

  cv::projectPoints(cv::Mat(object_points), out_pattern_params.rotation_vectors, out_pattern_params.translation_vectors,
                    intrinsic_parameters.camera_matrix, intrinsic_parameters.distortion_coefficients, imagePoints2);

  float err = norm(cv::Mat(image_points), cv::Mat(imagePoints2), cv::NORM_L2);
  out_pattern_params.reprojection_error = std::sqrt(err * err / object_points.size());
}

int OpenCvMLE::GetImagePoints(QList<PatternBatch> & out_batch_calibration_results,
                              std::vector<std::vector<std::vector<cv::Point2f>>> & out_image_points) {
  if (out_batch_calibration_results.isEmpty())
    return EmptyList;

  int number_of_cameras = out_batch_calibration_results[0].pattern_params.size();

  out_image_points.resize(number_of_cameras);

  for (int i = 0; i < out_image_points.size(); ++i)
    out_image_points[i].resize(out_batch_calibration_results.size());

  for (int batch_idx = 0; batch_idx < out_batch_calibration_results.size(); ++batch_idx) {
    PatternBatch & image_batch = out_batch_calibration_results[batch_idx];
    std::vector<cv::Mat> images;
    std::vector<std::vector<cv::Point2f>> pts;
    for (int cam_idx = 0; cam_idx < image_batch.pattern_params.size(); ++cam_idx) {
      const QString & image_path = image_batch.pattern_params[cam_idx].filename;
      cv::Mat image = cv::imread(image_path.toStdString(), cv::IMREAD_GRAYSCALE);
      image_batch.pattern_params[cam_idx].image_size = image.size();

      if (EstimationState::PES_Unestimated == image_batch.pattern_params[cam_idx].state) {

        // We will extract the pattern points for all images
        if (pattern_->Extract(image, out_image_points[cam_idx][batch_idx], true)) {
          image_batch.pattern_params[cam_idx].state = EstimationState::PES_Extracted;
        } else {
          image_batch.pattern_params[cam_idx].state = EstimationState::PES_Broken;
          image_batch.state = PES_Broken;
        }
      }
    }

    if (image_batch.state == PES_Unestimated)
      image_batch.state = PES_Extracted;
  }

  return 0;
}

int OpenCvMLE::Calibrate(QList<PatternBatch> & out_batch_calibration_results,
                         CalibrationEstimates & out_estimates) {

  std::vector<std::vector<std::vector<cv::Point2f>>> image_points;
  std::vector<std::vector<cv::Point3f> > object_points;

  int result;
  std::vector<cv::Point3f> newObjPoints;
  result = GetImagePoints(out_batch_calibration_results, image_points);
  if (result != 0)
    return result;

  if (calibrate_cameras_separately_)
    for (int cam_idx = 0; cam_idx < image_points.size(); ++cam_idx) {
      if (0 == CalibrateSingleCamera(out_batch_calibration_results,
                                     cam_idx,
                                     image_points,
                                     pattern_->GetSize(),
                                     1,
                                     false,
                                     0,
                                     out_estimates.intrinsic_parameters[cam_idx],
                                     newObjPoints));// TODO: decide what to do
    }

  if (std::any_of(out_batch_calibration_results.begin(),
                  out_batch_calibration_results.end(),
                  [&](const PatternBatch & image_batch) { return image_batch.pattern_params.size() >= 2; }))
    return Calibrate2Cameras(out_batch_calibration_results,
                             image_points,
                             pattern_->GetSize(),
                             calibrate_cameras_separately_,
                             out_estimates);

  return 0;
}

int OpenCvMLE::CalibrateSingleCamera(QList<PatternBatch> & out_batch_calibration_results,
                                     int cam_idx,
                                     const std::vector<std::vector<std::vector<cv::Point2f>>> & image_points,
                                     const cv::Size & boardSize,
                                     float aspectRatio,
                                     bool release_object,
                                     int flags,
                                     IntrinsicParameters & out_intrinsic_parameters,
                                     std::vector<cv::Point3f> & newObjPoints) {

  std::vector<std::vector<cv::Point2f>> filtered_image_points;
  std::vector<std::vector<cv::Point3f>> object_points;

  for (int batch_id = 0; batch_id < out_batch_calibration_results.size(); ++batch_id) {
    PatternBatch & image_batch = out_batch_calibration_results[batch_id];
    if (image_batch.pattern_params.size() <= cam_idx)
      continue;

    Pattern & pattern = image_batch.pattern_params[cam_idx];
    if (pattern.state != PES_Extracted)
      continue;

    // Initialize the size if not previously initialized
    if (out_intrinsic_parameters.image_size == cv::Size(0, 0))
      out_intrinsic_parameters.image_size = pattern.image_size;
    else if (out_intrinsic_parameters.image_size != pattern.image_size)
      return 2;// TODO: log error that image sizes should be uniform

    //Only enabled images participate in calibration
    if (pattern.enabled)
      filtered_image_points.push_back(image_points[cam_idx][batch_id]);
  }

  if (filtered_image_points.size() == 0)
    return 3; // TODO: log error that no valid image points found

  pattern_->GetObjectPoints(object_points, filtered_image_points.size());

  out_intrinsic_parameters.camera_matrix = cv::Mat::eye(3, 3, CV_64F);
  out_intrinsic_parameters.distortion_coefficients = cv::Mat::zeros(8, 1, CV_64F);

  if (flags & cv::CALIB_FIX_ASPECT_RATIO)
    out_intrinsic_parameters.camera_matrix.at<double>(0, 0) = aspectRatio;

  int iFixedPoint = -1;
  if (release_object)
    iFixedPoint = boardSize.width - 1;

  newObjPoints = object_points[0];
  std::vector<cv::Mat> rvecs;
  std::vector<cv::Mat> tvecs;

  out_intrinsic_parameters.rms = calibrateCameraRO(object_points,
                                                   filtered_image_points,
                                                   out_intrinsic_parameters.image_size,
                                                   iFixedPoint,
                                                   out_intrinsic_parameters.camera_matrix,
                                                   out_intrinsic_parameters.distortion_coefficients,
                                                   rvecs,
                                                   tvecs,
                                                   newObjPoints,
                                                   flags | cv::CALIB_FIX_K3 | cv::CALIB_USE_LU);

  if (!(cv::checkRange(out_intrinsic_parameters.camera_matrix)
      && cv::checkRange(out_intrinsic_parameters.distortion_coefficients)))
    return 1;

  out_intrinsic_parameters.total_average_error = out_intrinsic_parameters.rms;


  // Calculate per image reprojection error
  int valid_id = -1;
  for (int batch_id = 0; batch_id < out_batch_calibration_results.size(); ++batch_id) {
    PatternBatch & image_batch = out_batch_calibration_results[batch_id];
    Pattern & pattern = image_batch.pattern_params[cam_idx];

    if (image_batch.pattern_params.size() <= cam_idx || PES_Broken == image_batch.pattern_params[cam_idx].state)
      continue;

    if (PES_Extracted == pattern.state && pattern.enabled) {
      ++valid_id;
      pattern.rotation_vectors = rvecs[valid_id];
      pattern.translation_vectors = tvecs[valid_id];
    }
      // If the image is not enabled then its pattern was not included in the
      // dataset, hence, we have to calculate its rvecs and tvecs separately
    else if (!pattern.enabled) {
      cv::solvePnP(object_points[0],
                   image_points[cam_idx][batch_id],
                   out_intrinsic_parameters.camera_matrix,
                   out_intrinsic_parameters.distortion_coefficients,
                   pattern.rotation_vectors,
                   pattern.translation_vectors);
    }
    ComputeSingleCamReprojectionError(object_points[0],
                                      image_points[cam_idx][batch_id],
                                      out_intrinsic_parameters,
                                      pattern);
    pattern.state = PES_Calibrated;

  }

  return 0;
}

int OpenCvMLE::Calibrate2Cameras(QList<PatternBatch> & out_batch_calibration_results,
                                 const std::vector<std::vector<std::vector<cv::Point2f>>> & image_points,
                                 const cv::Size & boardSize,
                                 bool fix_intrinsic,
                                 CalibrationEstimates & out_estimates) {
  std::vector<std::vector<std::vector<cv::Point2f>>> filtered_image_points(2);
  std::vector<std::vector<cv::Point3f>> object_points(2);
  for (int batch_id = 0; batch_id < out_batch_calibration_results.size(); ++batch_id) {
    PatternBatch & image_batch = out_batch_calibration_results[batch_id];
    EstimationState image_batch_state = image_batch.PatternState();
    // Only one image in batch
    if (image_batch.pattern_params.size() != 2) continue;

    if (PES_Broken == image_batch_state) {
      image_batch.state = PES_Broken;
      continue;
    }

    if (PES_Extracted == image_batch_state
        && image_batch.pattern_params[0].image_size != image_batch.pattern_params[1].image_size)
      return 5;// TODO: log that if the cameras are not calibrated then the sizes of images should be equal


    if (image_batch.Enabled())
      for (int cam_idx = 0; cam_idx < image_batch.pattern_params.size(); ++cam_idx)
        filtered_image_points[cam_idx].push_back(image_points[cam_idx][batch_id]);
  }

  if (filtered_image_points[0].empty())
    return 3; // TODO: log error no valid pairs

  pattern_->GetObjectPoints(object_points, filtered_image_points[0].size());

  out_estimates.rms = cv::stereoCalibrate(object_points,
                                          filtered_image_points[0],
                                          filtered_image_points[1],
                                          out_estimates.intrinsic_parameters[0].camera_matrix,
                                          out_estimates.intrinsic_parameters[0].distortion_coefficients,
                                          out_estimates.intrinsic_parameters[1].camera_matrix,
                                          out_estimates.intrinsic_parameters[1].distortion_coefficients,
                                          out_batch_calibration_results[0].pattern_params[0].image_size,
                                          out_estimates.R,
                                          out_estimates.T,
                                          out_estimates.E,
                                          out_estimates.F,
                                          fix_intrinsic ? cv::CALIB_FIX_INTRINSIC : 0);



  // We should calculate the reprojection errors
  if (!fix_intrinsic) {
    for (int batch_id = 0; batch_id < out_batch_calibration_results.size(); ++batch_id) {
      PatternBatch & image_batch = out_batch_calibration_results[batch_id];
      EstimationState image_batch_state = image_batch.PatternState();
      if (PES_Broken == image_batch_state)
        continue;

      for (int cam_idx = 0; cam_idx < filtered_image_points.size(); ++cam_idx) {
        if (PES_Extracted == image_batch.pattern_params[cam_idx].state)
          image_batch.pattern_params[cam_idx].state = PES_Calibrated;

        cv::solvePnP(object_points[0],
                     image_points[cam_idx][batch_id],
                     out_estimates.intrinsic_parameters[cam_idx].camera_matrix,
                     out_estimates.intrinsic_parameters[cam_idx].distortion_coefficients,
                     image_batch.pattern_params[cam_idx].rotation_vectors,
                     image_batch.pattern_params[cam_idx].translation_vectors);

        ComputeSingleCamReprojectionError(object_points[0],
                                          image_points[cam_idx][batch_id],
                                          out_estimates.intrinsic_parameters[cam_idx],
                                          image_batch.pattern_params[cam_idx]);
      }

    }
  }

  // Compute stereo reprojection error
  for (int batch_id = 0; batch_id < out_batch_calibration_results.size(); ++batch_id) {
    PatternBatch & image_batch = out_batch_calibration_results[batch_id];
    if (image_batch.pattern_params.size() != 2 || PES_Broken == image_batch.state)
      continue;

    image_batch.state = PES_Calibrated;

    ComputeStereoRigReprojectionError(object_points[0],
                                      std::vector<std::vector<cv::Point2f>>{image_points[0][batch_id],
                                                                            image_points[1][batch_id]},
                                      out_estimates,
                                      image_batch);

  }
  return 0;
}

void OpenCvMLE::ComputeStereoRigReprojectionError(const std::vector<cv::Point3f> & object_points,
                                                  const std::vector<std::vector<cv::Point2f>> & image_points,
                                                  const CalibrationEstimates & estimates,
                                                  PatternBatch & out_result) {
  std::vector<cv::Vec3f> lines[2];
  cv::Mat imgpt[2];
  out_result.rms = 0;
  for (int camera_idx = 0; camera_idx < image_points.size(); ++camera_idx) {
    imgpt[camera_idx] = cv::Mat(image_points[camera_idx]);
    undistortPoints(imgpt[camera_idx],
                    imgpt[camera_idx],
                    estimates.intrinsic_parameters[camera_idx].camera_matrix,
                    estimates.intrinsic_parameters[camera_idx].distortion_coefficients,
                    cv::Mat(),
                    estimates.intrinsic_parameters[camera_idx].camera_matrix);
    computeCorrespondEpilines(imgpt[camera_idx], camera_idx + 1, estimates.F, lines[camera_idx]);
  }

  for (int pt_idx = 0; pt_idx < object_points.size(); ++pt_idx) {
    double errij = fabs(image_points[0][pt_idx].x * lines[1][pt_idx][0] +
        image_points[0][pt_idx].y * lines[1][pt_idx][1] + lines[1][pt_idx][2]) +
        fabs(image_points[1][pt_idx].x * lines[0][pt_idx][0] +
            image_points[1][pt_idx].y * lines[0][pt_idx][1] + lines[0][pt_idx][2]);
    out_result.rms += errij;
  }
  out_result.rms /= object_points.size();

}

}
}