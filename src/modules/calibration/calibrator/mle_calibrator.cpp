//
// Created by vahagn on 1/18/20.
//

#include "mle_calibrator.h"
#include "ui_mle_calibration_window.h"
#include "common/video_player.h"
#include "thumb_show_item_model.h"
#include "open_cv_mle.h"
#include <QHBoxLayout>
#include <chrono>
#include <QDir>
#include <QImageWriter>
#include <QMediaPlayer>

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
                                                                          player(settings.sounds_enabled
                                                                                 ? new QMediaPlayer : nullptr),
                                                                          next_capture_time_(std::numeric_limits<typeof(next_capture_time_)>::max()) {
  ui_->setupUi(this);
  connect(ui_->pushButton, &QPushButton::pressed, this, &MLECalibrator::CaptureRequested);
  ui_->listView->setAutoFillBackground(true);

  ;

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
  long now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  if (next_capture_time_ < now) {
    next_capture_time_ = std::numeric_limits<typeof(next_capture_time_)>::max();
    if (std::all_of(pts.begin(), pts.end(), [](const std::vector<cv::Point2f> & item) { return item.size(); })) {

      if (player) {
        player->setMedia(QUrl::fromLocalFile(
            "/home/vahagn/CLionProjects/gago-ui/sounds/Camera Shutter Click-SoundBible.com-228518582.mp3"));
        player->setVolume(50);
        player->play();
      }

      QStringList filenames;
      files_.push_back(std::vector<std::string>());
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
        filenames.append(filename);
        files_.back().push_back(filename.toStdString());
      }
      ((ThumbShowItemModel *) ui_->listView->model())->AppendByFilenames(filenames);

      ++last_image_index;
    } else {
      if (settings_.sounds_enabled) {
        player->setMedia(QUrl::fromLocalFile(
            "/home/vahagn/CLionProjects/gago-ui/sounds/Computer Error-SoundBible.com-1655839472.mp3"));
        player->setVolume(50);
        player->play();
      }
    }
    ui_->pushButton->setEnabled(true);
  }

  for (int i = 0; i < ptr->size(); ++i) {
    pattern_->DrawPattern((*ptr)[i].data, pts[i]);
    players_[i]->ShowImage((*ptr)[i].data);
  }

}

void MLECalibrator::SetCameras(const std::vector<const io::video::CameraMeta *> & vector) {

  QStringList cam_names;
  for (int i = 0; i < vector.size(); ++i) {
    QString camera_format(vector[i]->GetName().c_str());
    cam_names.append(camera_format);

    players_.push_back(new common::VideoPlayer());
    this->ui_->horizontalLayout_3->addWidget(players_[i]);
  }

  ui_->listView->setModel(new ThumbShowItemModel(QDir(settings_.image_save_folder), cam_names));
}

void MLECalibrator::CaptureRequested() {
  ui_->pushButton->setEnabled(false);
  next_capture_time_ = (std::chrono::high_resolution_clock::now().time_since_epoch()
      + std::chrono::seconds(settings_.wait_time)).count();
}

void MLECalibrator::on_pushButton_2_clicked() {
  ui_->pushButton->setEnabled(false);
  ui_->pushButton_2->setEnabled(false);

  gago::calibration::OpenCvMLE mle;
  std::vector<Point3f> corners;
  mle.calcChessboardCorners(pattern_->GetSize(), 1, corners, pattern_->GetType());

  std::vector<std::vector<std::vector<Point2f>>> image_points(files_[0].size());

  for (int capture_idx = 0; capture_idx < files_.size(); ++capture_idx) {
    std::vector<std::string> & image_filenames = files_[capture_idx];
    image_points[capture_idx].resize(image_filenames.size());

    std::vector<cv::Mat> images(image_filenames.size());
    for (int i = 0; i < images.size(); ++i) {
      images[i] = cv::imread(image_filenames[i], cv::IMREAD_GRAYSCALE);
    }
    std::vector<std::vector<cv::Point2f>> points;
    pattern_->Extract(images, points);
    for (int camera_idx = 0; camera_idx < images.size(); ++camera_idx) {
      if (gago::calibration::Pattern::CHESSBOARD == pattern_->GetType())
        cornerSubPix(images[camera_idx], points[camera_idx], cv::Size(11, 11),
                     Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.0001));
      image_points[camera_idx].push_back(points[camera_idx]);

    }
  }
  for (int camera_idx = 0; camera_idx < image_points.size(); ++camera_idx) {
    Mat cameraMatrix, distCoeffs;
    vector<Mat> rvecs, tvecs;
    vector<float> reprojErrs;
    vector<Point3f> newObjPoints;
    double totalAvgErr;
//    mle.runCalibration(image_points[camera_idx], )
  }

  ui_->pushButton->setEnabled(true);
  ui_->pushButton_2->setEnabled(true);
}

}
}
}


