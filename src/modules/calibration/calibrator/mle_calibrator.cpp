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
#include <QPainter>
#include <QColormap>
#include <iostream>

namespace gago {
namespace gui {
namespace calibration {

MLECalibrator::MLECalibrator(QWidget *parent,
                             const std::shared_ptr<gago::calibration::pattern::IPattern> & pattern,
                             const MLEConfigurationSettings & settings) : QDialog(parent),
                                                                          ui_(new Ui::MLECalibrationWindow()),
                                                                          pattern_(pattern),
                                                                          settings_(settings),
                                                                          last_capture_index_(0),
                                                                          player(settings.sounds_enabled
                                                                                 ? new QMediaPlayer : nullptr),
                                                                          next_capture_time_(std::numeric_limits<typeof(next_capture_time_)>::max()) {
  ui_->setupUi(this);

  connect(ui_->captureButton, &QPushButton::pressed, this, &MLECalibrator::CaptureRequested);
  connect(ui_->calibrateButton, &QPushButton::pressed, this, &MLECalibrator::OnCalibrateButtonClicked);
  connect(ui_->saveButton, &QPushButton::pressed, this, &MLECalibrator::OnSaveButtonClicked);

  connect(this, &MLECalibrator::DisableControlElements, this, &MLECalibrator::DisableControlElementsSlot);
  connect(this, &MLECalibrator::EnableControlElements, this, &MLECalibrator::EnableControlElementsSlot);

  ui_->listView->setAutoFillBackground(true);
}

MLECalibrator::~MLECalibrator() {
  delete ui_;

}

void MLECalibrator::Close() {
  close();
}

int MLECalibrator::Calibrate() {
  return exec();
}

void MLECalibrator::Notify(const std::shared_ptr<std::vector<io::video::Capture>> & ptr) {
  std::vector<cv::Mat> images;
  for (int j = 0; j < ptr->size(); ++j) {
    images.push_back((*ptr)[j].data);
  }
  std::vector<std::vector<cv::Point2f>> pts;
  bool found = pattern_->Extract(images, pts);
  long now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  if (next_capture_time_ < now) {
    next_capture_time_ = std::numeric_limits<typeof(next_capture_time_)>::max();
    if (found) {
      if (player) {
        player->setMedia(QUrl::fromLocalFile(
            "/home/vahagn/CLionProjects/gago-ui/sounds/Camera Shutter Click-SoundBible.com-228518582.mp3"));
        player->setVolume(50);
        player->play();
      }

      QStringList filenames;
      files_.push_back(QStringList());
      for (const io::video::Capture & capture: *ptr) {
        QString filename = QString::asprintf(format, capture.camera->GetName().c_str(), last_capture_index_);
        QDir dir(settings_.image_save_folder);
        QImage image(capture.data.data,
                     capture.data.cols,
                     capture.data.rows,
                     capture.data.cols * capture.data.channels(),
                     QImage::Format_RGB888);
        QImageWriter writer(dir.filePath(filename));
        writer.write(image);
        filenames.append(filename);
        files_.back().push_back(dir.filePath(filename));
      }
      ui_->listView->Append(files_.back());

      ++last_capture_index_;
    } else {
      if (settings_.sounds_enabled) {
        player->setMedia(QUrl::fromLocalFile(
            "/home/vahagn/CLionProjects/gago-ui/sounds/Computer Error-SoundBible.com-1655839472.mp3"));
        player->setVolume(50);
        player->play();
      }
    }
    emit EnableControlElements();
  }

  for (int i = 0; i < ptr->size(); ++i) {
    pattern_->DrawPattern((*ptr)[i].data, pts[i]);
    players_[i]->ShowImage((*ptr)[i].data);
  }

}

void MLECalibrator::SetCameras(const std::vector<const io::video::CameraMeta *> & vector) {
  emit DisableControlElements();

  QStringList cam_names;
  for (int i = 0; i < vector.size(); ++i) {
    QString camera_format(vector[i]->GetName().c_str());
    cam_names.append(camera_format);

    players_.push_back(new common::VideoPlayer());
    QVBoxLayout *player_layout = new QVBoxLayout();
    this->ui_->videoLayout->addLayout(player_layout);
    players_[i]->setMinimumWidth(200);
    players_[i]->setMinimumHeight(200);
    player_layout->addWidget(players_[i], 0, Qt::AlignHCenter);
    QLabel *cam_label = new QLabel("Uncalibrated");
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    font.setWeight(75);
    cam_label->setFont(font);
    cam_labels_.append(cam_label);
    player_layout->addStretch(1);
    player_layout->addWidget(cam_label);
  }
  ui_->listView->SetCameraNames(cam_names);

  RestoreFilenames(format, cam_names);

  for (QStringList & filenames: files_) {
    ui_->listView->Append(filenames);
  }
  emit EnableControlElements();
}

void MLECalibrator::CaptureRequested() {
  DisableControlElements();
  next_capture_time_ = (std::chrono::high_resolution_clock::now().time_since_epoch()
      + std::chrono::seconds(settings_.wait_time)).count();
}

void MLECalibrator::OnCalibrateButtonClicked() {
  emit DisableControlElements();

  gago::calibration::OpenCvMLE mle(pattern_, settings_);

  QList<gago::calibration::BatchCalibrationResult> & batches = ui_->listView->GetBatchCalibrationResults();

  // Reset the state if it was calibrated previously
  for (gago::calibration::BatchCalibrationResult & image_batch: batches) {
    if (image_batch.state == gago::calibration::PES_Calibrated)
      image_batch.state = gago::calibration::PES_Unestimated;

    for (gago::calibration::PatternEstimationParameters & param : image_batch.pattern_params)
      if (param.state == gago::calibration::PES_Calibrated)
        param.state = gago::calibration::PES_Unestimated;
  }

  if (0 == mle.Calibrate(batches, estimates_)) {
    ui_->listView->Update();
    if (!estimates_.R.empty()) {
      ui_->listView->SetCalibrationEstimates(estimates_);
      ui_->multiviewInfoLabel->setText(QString::asprintf("Stereo Calibration Error: %.4f", estimates_.rms));
    }

    for (int cam_idx = 0; cam_idx < batches[0].pattern_params.size(); ++cam_idx) {
      QString label_text = QString::asprintf("Average error: %.4f", estimates_.intrinsic_parameters[cam_idx].rms);

      cam_labels_[cam_idx]->setText(label_text);
    }
    ui_->saveButton->setEnabled(true);
  }


  emit EnableControlElements();
}

void MLECalibrator::RestoreFilenames(const char *format, QStringList cameras_) {
  last_capture_index_ = 0;
  QDir directory(settings_.image_save_folder);
  QStringList filters = {QString(format).replace("%s", cameras_[0]).replace("%03d", "*")};
  for (const QString & filename: directory.entryList(filters, QDir::NoFilter, QDir::SortFlag::Name)) {
    int idx = filename.right(7).left(3).toInt();
    QStringList idx_files = {directory.filePath(filename)};
    for (int i = 1; i < cameras_.size(); ++i) {
      QString cam_filename = QString::asprintf(format, cameras_[i].toStdString().c_str(), idx);
      if (directory.exists(cam_filename))
        idx_files.push_back(directory.filePath(cam_filename));
    }

    if (idx_files.size() == cameras_.size()) {
      files_.push_back(idx_files);
      last_capture_index_ = std::max(last_capture_index_, idx);
    }
  }
}

void MLECalibrator::DisableControlElementsSlot() {
  control_disable_bit_mask_ = 0;
  if (ui_->captureButton->isEnabled()) {
    control_disable_bit_mask_ |= 1;
    ui_->captureButton->setEnabled(false);
  }

  if (ui_->calibrateButton->isEnabled()) {
    control_disable_bit_mask_ |= 2;
    ui_->calibrateButton->setEnabled(false);
  }

  if (ui_->listView->isEnabled()) {
    control_disable_bit_mask_ |= 4;
    ui_->listView->setEnabled(false);
  }
  if (ui_->saveButton->isEnabled()) {
    control_disable_bit_mask_ |= 8;
    ui_->saveButton->setEnabled(false);
  }
  qApp->processEvents();
}

void MLECalibrator::EnableControlElementsSlot() {
  if (control_disable_bit_mask_ & 1)
    ui_->captureButton->setEnabled(true);
  if (control_disable_bit_mask_ & 2)
    ui_->calibrateButton->setEnabled(true);

  if (control_disable_bit_mask_ & 4)
    ui_->listView->setEnabled(true);

  if (control_disable_bit_mask_ & 8)
    ui_->saveButton->setEnabled(true);
  qApp->processEvents();
}

const gago::calibration::CalibrationEstimates & MLECalibrator::GetEstimates() const {
  return estimates_;
}

void MLECalibrator::OnSaveButtonClicked() {
  setResult(QDialog::Accepted);
  accept();
}

}
}
}




