//
// Created by vahagn on 1/18/20.
//

#include "mle_calibrator.h"
#include "ui_mle_calibration_window.h"
#include "common/video_player.h"
#include <QHBoxLayout>
#include <chrono>
#include <QtCore/QDir>
#include <QtGui/QImageWriter>

namespace gago {
namespace gui {
namespace calibration {

MLECalibrator::MLECalibrator(QWidget *parent,
                             const std::shared_ptr<gago::calibration::pattern::IPattern> & pattern,
                             const MLEConfigurationSettings & settings) : QDialog(parent),
                                                                          ui_(new Ui::MLECalibrationWindow()),
                                                                          pattern_(pattern),
                                                                          settings_(settings),
                                                                          last_image_index(0),
                                                                          next_capture_time_(std::numeric_limits<typeof(next_capture_time_)>::max()) {
  ui_->setupUi(this);
  connect(ui_->pushButton, &QPushButton::pressed, this, &MLECalibrator::CaptureRequested);

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

void MLECalibrator::Notify(const std::shared_ptr<std::vector<io::video::Capture>> & ptr) {
  std::vector<cv::Mat> images;
  for (int j = 0; j < ptr->size(); ++j) {
    images.push_back((*ptr)[j].data);
  }
  std::vector<std::vector<cv::Point2f>> pts;
  pattern_->Extract(images, pts);
  if (!std::all_of(pts.begin(), pts.end(), [](const std::vector<cv::Point2f> & item) { return item.size(); })) {
    long now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    if (next_capture_time_ < now) {
      next_capture_time_ = std::numeric_limits<typeof(next_capture_time_)>::max();
      char buffer[512];
      for (const io::video::Capture & capture: *ptr) {

        QString filename = QString::asprintf(format, capture.camera->GetName().c_str(), last_image_index);
        QDir dir(settings_.image_save_folder);
        QImage image(capture.data.data,
                     capture.data.cols,
                     capture.data.rows,
                     capture.data.cols * capture.data.channels(),
                     QImage::Format_RGB888);
        QImageWriter writer(dir.filePath(filename));
        writer.write(image);

      }
      ++last_image_index;
    }
  }

  for (int i = 0; i < ptr->size(); ++i) {
    pattern_->DrawPattern((*ptr)[i].data, pts[i]);
    players_[i]->ShowImage((*ptr)[i].data);
  }

}

void MLECalibrator::SetCameras(const std::vector<const io::video::CameraMeta *> & vector) {

  QStringList cam_names;
  for (int i = 0; i < vector.size(); ++i) {
    cam_names.append(vector[i]->GetName().c_str());
    players_.push_back(new common::VideoPlayer());
    this->ui_->horizontalLayout_3->addWidget(players_[i]);
  }
  //ui_->listView->setModel(new Thum)
}

void MLECalibrator::CaptureRequested() {
  next_capture_time_ = (std::chrono::high_resolution_clock::now().time_since_epoch()
      + std::chrono::seconds(settings_.wait_time)).count();
}
}
}
}