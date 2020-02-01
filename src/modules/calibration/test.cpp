//
// Created by vahagn on 1/16/20.
//
#include "calibrator/mle_calibrator.h"
#include <QApplication>
#include <QDir>
#include "calibrator/open_cv_mle.h"
#include "pattern/checkerboard_pattern.h"

QStringList GetFilenames(int idx, const char *format, QStringList cameras_, QDir directory_) {
  QStringList filenames;
  for (const QString & cam_forma: cameras_) {
    QString filename = QString::asprintf(format, cam_forma.toStdString().c_str(), idx);
    if (!directory_.exists(filename))
      break;
    else
      filenames.append(filename);
  }
  return filenames;
}

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);
  gago::gui::calibration::MLEConfigurationSettings settings;
  settings.fix_aspect_ratio = false;
  settings.calibrate_camera_first = true;
  std::shared_ptr<gago::calibration::pattern::CheckerboardPattern>
      pattern = std::make_shared<gago::calibration::pattern::CheckerboardPattern>(13, 8);
  gago::calibration::OpenCvMLE mle(pattern, settings);
  QDir dir("/home/vahagn/Pictures");
  std::vector<std::vector<std::string>> files;
  for (int i = 1; i < 11; ++i) {

    std::vector<std::string> batch;
    for (QString q: GetFilenames(i, "%s_%03d.jpg", {"left", "right"}, dir))
      batch.push_back(dir.filePath(q).toStdString());
    files.push_back(batch);
  }
  gago::calibration::CalibrationEstimates out_estimates;
//  mle.Calibrate(files, out_estimates);

  // gago::gui::calibration::MLECalibrator c(nullptr, nullptr, settings);
  application.exec();

}
