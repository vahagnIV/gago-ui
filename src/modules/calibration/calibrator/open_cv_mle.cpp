//
// Created by vahagn on 1/26/20.
//

#include <calibration_estimates.h>
#include "open_cv_mle.h"
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

int OpenCvMLE::Calibrate(const std::vector<std::vector<std::string>> & files, CalibrationEstimates & out_estimates) {

  int result;

  // Image points and Object Points
  // [batch_id][point_id]
  std::vector<std::vector<cv::Point3f> > object_points;
  std::vector<cv::Size> image_sizes;
  std::vector<std::vector<std::vector<cv::Point2f>>> image_points;

  result = GetImagePoints(files, image_sizes, image_points);
  if (result != 0)
    return result;

  pattern_->GetObjectPoints(object_points, image_points[0].size());

  std::vector<PatternEstimationParameters> pattern_parameters(image_sizes.size());

  if (settings_.calibrate_camera_first)
    for (int camera_idx = 0; camera_idx < image_sizes.size(); ++camera_idx) {
      cv::Mat camera_matrix, dist_coeffs;
      std::vector<cv::Mat> rvecs, tvecs;
      std::vector<float> reproj_errors;
      // TODO: overcome the bug in Opencv and allow release_object
      std::vector<cv::Point3f> new_object_points;
      int flags = 0;
      if (settings_.fix_aspect_ratio)
        flags |= cv::CALIB_FIX_ASPECT_RATIO;
      result = CalibrateSeparateMatrix(image_points[camera_idx],
                                       object_points,
                                       image_sizes[camera_idx],
                                       pattern_->GetSize(),
                                       1,
                                       false,
                                       flags,
                                       out_estimates.intrinsic_parameters[camera_idx],
                                       pattern_parameters[camera_idx],
                                       new_object_points);
      if (0 != result)
        return result;

    }
  out_estimates.rms = cv::stereoCalibrate(object_points,
                                          image_points[0],
                                          image_points[1],
                                          out_estimates.intrinsic_parameters[0].camera_matrix,
                                          out_estimates.intrinsic_parameters[0].distortion_coefficients,
                                          out_estimates.intrinsic_parameters[1].camera_matrix,
                                          out_estimates.intrinsic_parameters[1].distortion_coefficients,
                                          image_sizes[0],
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

int OpenCvMLE::GetImagePoints(const std::vector<std::vector<std::string>> & files,
                              std::vector<cv::Size> & out_image_sizes,
                              std::vector<std::vector<std::vector<cv::Point2f>>> & out_image_points) const {
  if (files.empty())
    return EmptyList;

  out_image_points.resize(files[0].size());
  out_image_sizes.resize(0);

  // All images per camera should have the same size in all batches,
  // hence, we keep the sizes of the first batch in order to
  // compare to the others

  for (const std::vector<std::string> image_batch: files) {

    bool cam_resolutions_initialized = !out_image_sizes.empty();

    // Ignore the batch if there is an inconsistent number of images
    if (cam_resolutions_initialized && image_batch.size() != out_image_sizes.size())
      continue;

    std::vector<cv::Mat> images;
    std::vector<std::vector<cv::Point2f>> pts;
    for (int cam_idx = 0; cam_idx < image_batch.size(); ++cam_idx) {
      const std::string & image_path = image_batch[cam_idx];

      images.push_back(cv::imread(image_path, cv::IMREAD_GRAYSCALE));

      // Check if image sizes are consistent
      if (!cam_resolutions_initialized)
        out_image_sizes.push_back(images.back().size());
      else if (out_image_sizes[cam_idx] != images.back().size())
        return InconsistenSize;
    }

    if (pattern_->Extract(images, pts, true))
      for (int camera_idx = 0; camera_idx < pts.size(); ++camera_idx) {
        out_image_points[camera_idx].push_back(pts[camera_idx]);
      }
  }

  return 0;
}

int OpenCvMLE::CalibrateSeparateMatrix(const std::vector<std::vector<cv::Point2f> > & image_points,
                                       const std::vector<std::vector<cv::Point3f> > & object_points,
                                       cv::Size imageSize,
                                       cv::Size boardSize,
                                       float aspectRatio,
    //float grid_width,
                                       bool release_object,
                                       int flags,
                                       IntrinsicParameters & intrinsic_parameters,
                                       PatternEstimationParameters & pattern_parameters,
                                       std::vector<cv::Point3f> & newObjPoints) {

  intrinsic_parameters.camera_matrix = cv::Mat::eye(3, 3, CV_64F);
  intrinsic_parameters.distortion_coefficients = cv::Mat::zeros(8, 1, CV_64F);

  if (flags & cv::CALIB_FIX_ASPECT_RATIO)
    intrinsic_parameters.camera_matrix.at<double>(0, 0) = aspectRatio;

  int iFixedPoint = -1;
  if (release_object)
    iFixedPoint = boardSize.width - 1;

  newObjPoints = object_points[0];

  intrinsic_parameters.rms = calibrateCameraRO(object_points,
                                               image_points,
                                               imageSize,
                                               iFixedPoint,
                                               intrinsic_parameters.camera_matrix,
                                               intrinsic_parameters.distortion_coefficients,
                                               pattern_parameters.rotation_vectors,
                                               pattern_parameters.translation_vectors,
                                               newObjPoints,
                                               flags | cv::CALIB_FIX_K3 | cv::CALIB_USE_LU);
  if (!(cv::checkRange(intrinsic_parameters.camera_matrix)
      && cv::checkRange(intrinsic_parameters.distortion_coefficients)))
    return 1;

  intrinsic_parameters.total_average_error =
      ComputeReprojectionErrors(object_points,
                                image_points,
                                pattern_parameters.rotation_vectors,
                                pattern_parameters.translation_vectors,
                                intrinsic_parameters,
                                pattern_parameters.reprojection_errors);

  std::cout << intrinsic_parameters.camera_matrix << std::endl;
  std::cout << intrinsic_parameters.distortion_coefficients << std::endl;
  std::cout << intrinsic_parameters.total_average_error << std::endl;
  std::cout << std::endl;
  return 0;

}

double OpenCvMLE::ComputeReprojectionErrors(const std::vector<std::vector<cv::Point3f> > & object_points,
                                            const std::vector<std::vector<cv::Point2f> > & image_points,
                                            const std::vector<cv::Mat> & rvecs,
                                            const std::vector<cv::Mat> & tvecs,
                                            IntrinsicParameters & intrinsic_parameters,
                                            std::vector<float> & perViewErrors) {

  std::vector<cv::Point2f> imagePoints2;
  int i, totalPoints = 0;
  double totalErr = 0, err;
  perViewErrors.resize(object_points.size());

  for (i = 0; i < (int) object_points.size(); i++) {
    cv::projectPoints(cv::Mat(object_points[i]), rvecs[i], tvecs[i],
                      intrinsic_parameters.camera_matrix, intrinsic_parameters.distortion_coefficients, imagePoints2);
    err = norm(cv::Mat(image_points[i]), cv::Mat(imagePoints2), cv::NORM_L2);
    int n = (int) object_points[i].size();
    perViewErrors[i] = (float) std::sqrt(err * err / n);
    totalErr += err * err;
    totalPoints += n;
  }
  return std::sqrt(totalErr / totalPoints);

}

}
}