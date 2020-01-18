//
// Created by vahagn on 1/18/20.
//

#include "mle_calibrator.h"
#include "ui_mle_calibration_window.h"
#include "common/video_player.h"
#include <QVBoxLayout>

namespace gago {
namespace gui {
namespace calibration {

MLECalibrator::MLECalibrator(QWidget *parent,
                             const std::shared_ptr<gago::calibration::pattern::IPattern> &pattern) : QDialog(parent),
                                                                                                     pattern_(pattern),
                                                                                                     ui_(new Ui::MLECalibrationWindow()) {
  ui_->setupUi(this);
  this->setLayout(new QVBoxLayout());

}

MLECalibrator::~MLECalibrator() {
  delete ui_;
}

void MLECalibrator::Close() {
  close();
}

void MLECalibrator::Calibrate() {
  exec();

}
void MLECalibrator::Notify(const std::shared_ptr<std::vector<io::video::Capture>> &ptr) {
  std::vector<cv::Mat> images;
  for (int j = 0; j < ptr->size(); ++j) {
    images.push_back((*ptr)[j].data);
  }
  std::vector<std::vector<cv::Point2f>> pts;
  pattern_->Extract(images, pts);
  for (int i = 0; i < ptr->size(); ++i) {
    pattern_->DrawPattern((*ptr)[i].data, pts[i]);
    players_[i]->ShowImage((*ptr)[i].data);
  }

}

void MLECalibrator::SetCameras(const std::vector<const io::video::CameraMeta *> &vector) {
  for (int i = 0; i < vector.size(); ++i) {
    players_.push_back(new common::VideoPlayer());
    layout()->addWidget(players_[i]);
  }
}
}
}
}