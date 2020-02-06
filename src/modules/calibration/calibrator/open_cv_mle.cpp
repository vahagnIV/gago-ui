//
// Created by vahagn on 1/26/20.
//

#include <calibration_estimates.h>
#include "open_cv_mle.h"
#include <QList>

namespace gago {
namespace calibration {

OpenCvMLE::OpenCvMLE(const std::shared_ptr<pattern::IPattern> & pattern,
                     const gago::gui::calibration::MLEConfigurationSettings & settings)
    : pattern_(pattern), settings_(settings) {

}

enum CalibrationErrors {
  InconsistenSize = 1, // The image size per camera is not equal
  EmptyList = 2
};

int OpenCvMLE::Calibrate(const QList<QStringList> & files,
                         CalibrationEstimates & out_estimates,
                         QList<QList<PatternEstimationParameters>> & out_pattern_estimation_parameters,
                         QList<cv::Size> & out_sizes,
                         QList<int> & out_batch_idx) {

  int result;

  // Image points and Object Points
  // [batch_id][point_id]
  std::vector<std::vector<cv::Point3f> > object_points;
  std::vector<std::vector<std::vector<cv::Point2f>>> image_points;

  result = GetImagePoints(files, out_sizes, image_points, out_batch_idx);

  if (result != 0)
    return result;

  pattern_->GetObjectPoints(object_points, image_points[0].size());

  if (settings_.calibrate_camera_first)
    for (int camera_idx = 0; camera_idx < out_sizes.size(); ++camera_idx) {
      cv::Mat camera_matrix, dist_coeffs;
      std::vector<cv::Mat> rvecs, tvecs;
      std::vector<float> reproj_errors;
      // TODO: overcome the bug in Opencv and allow release_object
      std::vector<cv::Point3f> new_object_points;
      int flags = 0;
      if (settings_.fix_aspect_ratio)
        flags |= cv::CALIB_FIX_ASPECT_RATIO;
      QList<PatternEstimationParameters> pattern_params;
      result = CalibrateSeparateCamera(image_points[camera_idx],
                                       object_points,
                                       out_sizes[camera_idx],
                                       pattern_->GetSize(),
                                       1,
                                       false,
                                       flags,
                                       out_estimates.intrinsic_parameters[camera_idx],
                                       pattern_params,
                                       new_object_points);
      for (int batch_idx = 0; batch_idx < files.size(); ++batch_idx) {
        int valid_batch_id = out_batch_idx[batch_idx];
        if (-1 == valid_batch_id)
          continue;
        if (out_pattern_estimation_parameters.size() == valid_batch_id)
          out_pattern_estimation_parameters.append(QList<PatternEstimationParameters>());
        out_pattern_estimation_parameters[valid_batch_id].append(pattern_params[valid_batch_id]);
      }

      if (0 != result)
        return result;

    }
  if (files[0].size() < 2)
    return 0;

  out_estimates.rms = cv::stereoCalibrate(object_points,
                                          image_points[0],
                                          image_points[1],
                                          out_estimates.intrinsic_parameters[0].camera_matrix,
                                          out_estimates.intrinsic_parameters[0].distortion_coefficients,
                                          out_estimates.intrinsic_parameters[1].camera_matrix,
                                          out_estimates.intrinsic_parameters[1].distortion_coefficients,
                                          out_sizes[0],
                                          out_estimates.R,
                                          out_estimates.T,
                                          out_estimates.E,
                                          out_estimates.F);

  std::cout << out_estimates.R << std::endl;
  std::cout << out_estimates.T << std::endl;
  std::cout << out_estimates.E << std::endl;
  std::cout << out_estimates.F << std::endl;

  return 0;
}

int OpenCvMLE::GetImagePoints(const QList<QStringList> & files,
                              QList<cv::Size> & out_image_sizes,
                              std::vector<std::vector<std::vector<cv::Point2f>>> & out_image_points,
                              QList<int> & out_batch_idx) const {
  if (files.isEmpty())
    return EmptyList;

  out_image_points.resize(files[0].size());
  out_batch_idx.clear();
  out_batch_idx.reserve(files.size());

  // All images per camera should have the same size in all batches,
  // hence, we keep the sizes of the first batch in order to
  // compare to the others
  out_image_sizes.clear();

  for (const QStringList & image_batch: files) {
    bool cam_resolutions_initialized = !out_image_sizes.empty();

    // Ignore the batch if there is an inconsistent number of images
    if (cam_resolutions_initialized && image_batch.size() != out_image_sizes.size())
      continue;

    std::vector<cv::Mat> images;
    std::vector<std::vector<cv::Point2f>> pts;
    int valid_count = 0;
    for (int cam_idx = 0; cam_idx < image_batch.size(); ++cam_idx) {
      const QString & image_path = image_batch[cam_idx];

      images.push_back(cv::imread(image_path.toStdString(), cv::IMREAD_GRAYSCALE));

      // Check if image sizes are consistent
      if (!cam_resolutions_initialized)
        out_image_sizes.push_back(images.back().size());
      else if (out_image_sizes[cam_idx] != images.back().size())
        return InconsistenSize;
    }

    if (pattern_->Extract(images, pts, true)) {
      for (int camera_idx = 0; camera_idx < pts.size(); ++camera_idx) {
        out_image_points[camera_idx].push_back(pts[camera_idx]);
      }
      out_batch_idx.append(valid_count++);
    } else
      out_batch_idx.append(-1);
  }

  return 0;
}

int OpenCvMLE::CalibrateSeparateCamera(const std::vector<std::vector<cv::Point2f> > & image_points,
                                       const std::vector<std::vector<cv::Point3f> > & object_points,
                                       cv::Size imageSize,
                                       cv::Size boardSize,
                                       float aspectRatio,
    //float grid_width,
                                       bool release_object,
                                       int flags,
                                       IntrinsicParameters & intrinsic_parameters,
                                       QList<PatternEstimationParameters> & out_pattern_parameters,
                                       std::vector<cv::Point3f> & newObjPoints) {

  intrinsic_parameters.camera_matrix = cv::Mat::eye(3, 3, CV_64F);
  intrinsic_parameters.distortion_coefficients = cv::Mat::zeros(8, 1, CV_64F);

  if (flags & cv::CALIB_FIX_ASPECT_RATIO)
    intrinsic_parameters.camera_matrix.at<double>(0, 0) = aspectRatio;

  int iFixedPoint = -1;
  if (release_object)
    iFixedPoint = boardSize.width - 1;

  newObjPoints = object_points[0];
  std::vector<cv::Mat> rvecs;
  std::vector<cv::Mat> tvecs;
  std::vector<float> reprojection_errors;

  intrinsic_parameters.rms = calibrateCameraRO(object_points,
                                               image_points,
                                               imageSize,
                                               iFixedPoint,
                                               intrinsic_parameters.camera_matrix,
                                               intrinsic_parameters.distortion_coefficients,
                                               rvecs,
                                               tvecs,
                                               newObjPoints,
                                               flags | cv::CALIB_FIX_K3 | cv::CALIB_USE_LU);
  if (!(cv::checkRange(intrinsic_parameters.camera_matrix)
      && cv::checkRange(intrinsic_parameters.distortion_coefficients)))
    return 1;
  /*intrinsic_parameters.total_average_error =
      ComputeReprojectionErrors(object_points,
                                image_points,
                                rvecs,
                                tvecs,
                                intrinsic_parameters,
                                reprojection_errors);*/

  for (int i = 0; i < rvecs.size(); ++i)
    out_pattern_parameters.append(PatternEstimationParameters(rvecs[i], tvecs[i], reprojection_errors[i]));

  std::cout << intrinsic_parameters.camera_matrix << std::endl;
  std::cout << intrinsic_parameters.distortion_coefficients << std::endl;
  std::cout << intrinsic_parameters.total_average_error << std::endl;
  std::cout << std::endl;
  return 0;

}

void OpenCvMLE::ComputeSingleCamReprojectionError(const std::vector<cv::Point3f> & object_points,
                                                  const std::vector<cv::Point2f> & image_points,
                                                  const IntrinsicParameters & intrinsic_parameters,
                                                  PatternEstimationParameters & out_pattern_params
) {

  std::vector<cv::Point2f> imagePoints2;

  cv::projectPoints(cv::Mat(object_points), out_pattern_params.rotation_vectors, out_pattern_params.translation_vectors,
                    intrinsic_parameters.camera_matrix, intrinsic_parameters.distortion_coefficients, imagePoints2);
  out_pattern_params.reprojection_error = norm(cv::Mat(image_points), cv::Mat(imagePoints2), cv::NORM_L2);

}

int OpenCvMLE::GetImagePoints(QList<BatchCalibrationResult *> & out_batch_calibration_results,
                              std::vector<std::vector<std::vector<cv::Point2f>>> & out_image_points) {
  if (out_batch_calibration_results.isEmpty())
    return EmptyList;

  out_image_points.resize(out_batch_calibration_results[0]->pattern_params.size());
  for (int i = 0; i < out_image_points.size(); ++i)
    out_image_points[i].reserve(out_batch_calibration_results.size());

  for (BatchCalibrationResult *image_batch: out_batch_calibration_results) {
    std::vector<cv::Mat> images;
    std::vector<std::vector<cv::Point2f>> pts;

    bool all_cameras_are_ok = true;
    for (int cam_idx = 0; cam_idx < image_batch->pattern_params.size(); ++cam_idx) {
      const QString & image_path = image_batch->pattern_params[cam_idx].filename;
      cv::Mat image = cv::imread(image_path.toStdString(), cv::IMREAD_GRAYSCALE);
      image_batch->pattern_params[cam_idx].image_size = image.size();

      out_image_points[cam_idx].resize(out_image_points[cam_idx].size() + 1);

      if (pattern_->Extract(image, out_image_points[cam_idx].back(), true))
        image_batch->pattern_params[cam_idx].state = EstimationState::PES_Extracted;
      else {
        image_batch->pattern_params[cam_idx].state = EstimationState::PES_Broken;
        all_cameras_are_ok = false;
      }
    }
    if (!all_cameras_are_ok)
      image_batch->state = EstimationState::PES_Broken;
    else
      image_batch->state = EstimationState::PES_Extracted;
  }
  return 0;
}

int OpenCvMLE::Calibrate(QList<BatchCalibrationResult *> & out_batch_calibration_results,
                         CalibrationEstimates & out_estimates) {
  std::vector<std::vector<std::vector<cv::Point2f>>> image_points;
  std::vector<std::vector<cv::Point3f> > object_points;

  int result;
  std::vector<cv::Point3f> newObjPoints;
  result = GetImagePoints(out_batch_calibration_results, image_points);
  if (result != 0)
    return result;

  if (settings_.calibrate_camera_first)
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
                  [&](const BatchCalibrationResult *image_batch) { return image_batch->pattern_params.size() >= 2; }))
    Calibrate2Cameras(out_batch_calibration_results, image_points, pattern_->GetSize(), out_estimates);

  return 0;
}

int OpenCvMLE::CalibrateSingleCamera(QList<BatchCalibrationResult *> & out_batch_calibration_results,
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
    BatchCalibrationResult *image_batch = out_batch_calibration_results[batch_id];
    if (image_batch->pattern_params.size() <= cam_idx || image_batch->pattern_params[cam_idx].state != PES_Extracted)
      continue;
    if (out_intrinsic_parameters.image_size == cv::Size(0, 0))
      out_intrinsic_parameters.image_size = image_batch->pattern_params[cam_idx].image_size;
    else if (out_intrinsic_parameters.image_size != image_batch->pattern_params[cam_idx].image_size)
      return 2;// TODO: log error that image sizes should be uniform
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

  int valid_id = 0;
  for (int batch_id = 0; batch_id < out_batch_calibration_results.size(); ++batch_id) {
    BatchCalibrationResult *image_batch = out_batch_calibration_results[batch_id];
    if (image_batch->pattern_params.size() <= cam_idx || image_batch->pattern_params[cam_idx].state == PES_Broken)
      continue;
    image_batch->pattern_params[cam_idx].state = PES_Calibrated;
    image_batch->pattern_params[cam_idx].rotation_vectors = rvecs[valid_id];
    image_batch->pattern_params[cam_idx].translation_vectors = tvecs[valid_id];
    ComputeSingleCamReprojectionError(object_points[valid_id],
                                      image_points[cam_idx][batch_id],
                                      out_intrinsic_parameters,
                                      image_batch->pattern_params[cam_idx]);
    ++valid_id;
  }

  std::cout << out_intrinsic_parameters.camera_matrix << std::endl;
  std::cout << out_intrinsic_parameters.distortion_coefficients << std::endl;
  std::cout << out_intrinsic_parameters.total_average_error << std::endl;
  std::cout << std::endl;
  return 0;
}

int OpenCvMLE::Calibrate2Cameras(QList<BatchCalibrationResult *> & out_batch_calibration_results,
                                 const std::vector<std::vector<std::vector<cv::Point2f>>> & image_points,
                                 const cv::Size & boardSize,
                                 CalibrationEstimates & out_estimates) {
  std::vector<std::vector<std::vector<cv::Point2f>>> filtered_image_points(2);
  std::vector<std::vector<cv::Point3f>> object_points(2);
  bool all_calibrated = true;
  for (int batch_id = 0; batch_id < out_batch_calibration_results.size(); ++batch_id) {
    BatchCalibrationResult *image_batch = out_batch_calibration_results[batch_id];
    // Only one image in batch
    if (image_batch->pattern_params.size() != 2 || image_batch->state == PES_Broken)
      continue;

    if (image_batch->state == PES_Extracted) {
      all_calibrated = false;
      if (image_batch->pattern_params[0].image_size != image_batch->pattern_params[1].image_size)
        return 5;// TODO: log that if the cameras are not calibrated then the sizes of images should be equal
    }

    for (int cam_idx = 0; cam_idx < image_batch->pattern_params.size(); ++cam_idx) {
      filtered_image_points[cam_idx].push_back(image_points[cam_idx][batch_id]);
    }
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
                                          out_batch_calibration_results[0]->pattern_params[0].image_size,
                                          out_estimates.R,
                                          out_estimates.T,
                                          out_estimates.E,
                                          out_estimates.F,
                                          all_calibrated ? cv::CALIB_FIX_INTRINSIC : 0);

  int valid_idx = 0;
  // We should calculate the reprojection errors
  if (!all_calibrated) {
    for (int batch_id = 0; batch_id < out_batch_calibration_results.size(); ++batch_id) {
      BatchCalibrationResult *image_batch = out_batch_calibration_results[batch_id];
      if (image_batch->state == PES_Broken)
        continue;
      for (int cam_idx = 0; cam_idx < filtered_image_points.size(); ++cam_idx) {
        image_batch->pattern_params[cam_idx].state = PES_Calibrated;
        cv::solvePnP(object_points[valid_idx],
                     image_points[cam_idx][batch_id],
                     out_estimates.intrinsic_parameters[cam_idx].camera_matrix,
                     out_estimates.intrinsic_parameters[cam_idx].distortion_coefficients,
                     image_batch->pattern_params[cam_idx].rotation_vectors,
                     image_batch->pattern_params[cam_idx].translation_vectors);

        ComputeSingleCamReprojectionError(object_points[valid_idx],
                                          filtered_image_points[cam_idx][batch_id],
                                          out_estimates.intrinsic_parameters[cam_idx],
                                          image_batch->pattern_params[cam_idx]);
      }
      ++valid_idx;
    }
  }

  valid_idx = 0;
  for (int batch_id = 0; batch_id < out_batch_calibration_results.size(); ++batch_id) {
    BatchCalibrationResult *image_batch = out_batch_calibration_results[batch_id];
    if (image_batch->pattern_params.size() != 2 || image_batch->state == PES_Broken)
      continue;
    image_batch->state = PES_Calibrated;
    ComputeStereoRigReprojectionError(object_points[valid_idx],
                                      std::vector<std::vector<cv::Point2f>>{image_points[0][batch_id],
                                                                            image_points[1][batch_id]},
                                      out_estimates,
                                      image_batch);
    ++valid_idx;
  }
  return 0;
}

void OpenCvMLE::ComputeStereoRigReprojectionError(const std::vector<cv::Point3f> & object_points,
                                                  const std::vector<std::vector<cv::Point2f>> & image_points,
                                                  const CalibrationEstimates & estimates,
                                                  BatchCalibrationResult *out_result) {
  std::vector<cv::Vec3f> lines[2];
  cv::Mat imgpt[2];
  out_result->rms = 0;
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
    out_result->rms += errij;
  }
  out_result->rms /= object_points.size();

}

}
}