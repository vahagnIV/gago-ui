//
// Created by vahagn on 1/26/20.
//

#ifndef GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_OPEN_CV_MLE_H_
#define GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_OPEN_CV_MLE_H_

#include <opencv2/opencv.hpp>
#include "pattern/ipattern.h"
using namespace cv;
using namespace std;

namespace gago {
namespace calibration {

enum { DETECTION = 0, CAPTURING = 1, CALIBRATED = 2 };


class OpenCvMLE {
 public:
  static void calcChessboardCorners(Size boardSize, float squareSize, vector<Point3f>& corners, Pattern patternType = CHESSBOARD);
  static bool runCalibration( vector<vector<Point2f> > imagePoints,
                              Size imageSize, Size boardSize, Pattern patternType,
                              float squareSize, float aspectRatio,
                              float grid_width, bool release_object,
                              int flags, Mat& cameraMatrix, Mat& distCoeffs,
                              vector<Mat>& rvecs, vector<Mat>& tvecs,
                              vector<float>& reprojErrs,
                              vector<Point3f>& newObjPoints,
                              double& totalAvgErr);
  static double computeReprojectionErrors(
      const vector<vector<Point3f> >& objectPoints,
      const vector<vector<Point2f> >& imagePoints,
      const vector<Mat>& rvecs, const vector<Mat>& tvecs,
      const Mat& cameraMatrix, const Mat& distCoeffs,
      vector<float>& perViewErrors );

};

}
}

#endif //GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_OPEN_CV_MLE_H_
