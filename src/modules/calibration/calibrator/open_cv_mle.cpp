//
// Created by vahagn on 1/26/20.
//

#include "open_cv_mle.h"
namespace gago {
namespace calibration {

void OpenCvMLE::calcChessboardCorners(Size boardSize, float squareSize, vector<Point3f> &corners, Pattern patternType) {
  corners.resize(0);

  switch (patternType) {
    case CHESSBOARD:
    case CIRCLES_GRID:
      for (int i = 0; i < boardSize.height; i++)
        for (int j = 0; j < boardSize.width; j++)
          corners.push_back(Point3f(float(j * squareSize),
                                    float(i * squareSize), 0));
      break;

    case ASYMMETRIC_CIRCLES_GRID:
      for (int i = 0; i < boardSize.height; i++)
        for (int j = 0; j < boardSize.width; j++)
          corners.push_back(Point3f(float((2 * j + i % 2) * squareSize),
                                    float(i * squareSize), 0));
      break;

    default:CV_Error(Error::StsBadArg, "Unknown pattern type\n");
  }
}

bool OpenCvMLE::runCalibration(vector<vector<Point2f> > imagePoints,
                               Size imageSize,
                               Size boardSize,
                               Pattern patternType,
                               float squareSize,
                               float aspectRatio,
                               float grid_width,
                               bool release_object,
                               int flags,
                               Mat &cameraMatrix,
                               Mat &distCoeffs,
                               vector<Mat> &rvecs,
                               vector<Mat> &tvecs,
                               vector<float> &reprojErrs,
                               vector<Point3f> &newObjPoints,
                               double &totalAvgErr) {
  cameraMatrix = Mat::eye(3, 3, CV_64F);
  if( flags & CALIB_FIX_ASPECT_RATIO )
    cameraMatrix.at<double>(0,0) = aspectRatio;

  distCoeffs = Mat::zeros(8, 1, CV_64F);

  vector<vector<Point3f> > objectPoints(1);
  calcChessboardCorners(boardSize, squareSize, objectPoints[0], patternType);
  objectPoints[0][boardSize.width - 1].x = objectPoints[0][0].x + grid_width;
  newObjPoints = objectPoints[0];

  objectPoints.resize(imagePoints.size(),objectPoints[0]);

  double rms;
  int iFixedPoint = -1;
  if (release_object)
    iFixedPoint = boardSize.width - 1;
  rms = calibrateCameraRO(objectPoints, imagePoints, imageSize, iFixedPoint,
                          cameraMatrix, distCoeffs, rvecs, tvecs, newObjPoints,
                          flags | CALIB_FIX_K3 | CALIB_USE_LU);
  printf("RMS error reported by calibrateCamera: %g\n", rms);

  bool ok = checkRange(cameraMatrix) && checkRange(distCoeffs);

  if (release_object) {
    cout << "New board corners: " << endl;
    cout << newObjPoints[0] << endl;
    cout << newObjPoints[boardSize.width - 1] << endl;
    cout << newObjPoints[boardSize.width * (boardSize.height - 1)] << endl;
    cout << newObjPoints.back() << endl;
  }

  objectPoints.clear();
  objectPoints.resize(imagePoints.size(), newObjPoints);
  totalAvgErr = computeReprojectionErrors(objectPoints, imagePoints,
                                          rvecs, tvecs, cameraMatrix, distCoeffs, reprojErrs);

  return ok;
}

double OpenCvMLE::computeReprojectionErrors(const vector<vector<Point3f> > &objectPoints,
                                            const vector<vector<Point2f> > &imagePoints,
                                            const vector<Mat> &rvecs,
                                            const vector<Mat> &tvecs,
                                            const Mat &cameraMatrix,
                                            const Mat &distCoeffs,
                                            vector<float> &perViewErrors) {
  vector<Point2f> imagePoints2;
  int i, totalPoints = 0;
  double totalErr = 0, err;
  perViewErrors.resize(objectPoints.size());

  for( i = 0; i < (int)objectPoints.size(); i++ )
  {
    projectPoints(Mat(objectPoints[i]), rvecs[i], tvecs[i],
                  cameraMatrix, distCoeffs, imagePoints2);
    err = norm(Mat(imagePoints[i]), Mat(imagePoints2), NORM_L2);
    int n = (int)objectPoints[i].size();
    perViewErrors[i] = (float)std::sqrt(err*err/n);
    totalErr += err*err;
    totalPoints += n;
  }

  return std::sqrt(totalErr/totalPoints);
}

}
}