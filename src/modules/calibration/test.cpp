//
// Created by vahagn on 1/16/20.
//
#include "calibrator/mle_calibrator.h"
#include <QApplication>
int main(int argc, char *argv[]) {
  QApplication application(argc,argv);
  gago::gui::calibration::MLEConfigurationSettings settings;
  gago::gui::calibration::MLECalibrator c(nullptr, nullptr, settings);

}
