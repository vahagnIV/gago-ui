//
// Created by vahagn on 1/18/20.
//

#include <QDir>
#include <QImageWriter>
#include <QStandardPaths>
#include <QMediaPlayer>
#include <QPainter>
#include <QDebug>
#include <QHBoxLayout>

#include "mle_calibrator.h"
#include "ui_mle_calibration_window.h"
#include "common/video_player.h"
#include "open_cv_mle.h"
#include <chrono>

namespace gago {
namespace gui {
namespace calibration {

MLECalibrator::MLECalibrator(QWidget *parent,
                             const QSharedPointer<gago::calibration::pattern::IPatternExtractor> &pattern,
                             const QSharedPointer<gago::gui::configuration::MLECalibratorSettings> &settings,
                             const gago::calibration::CalibrationEstimates & current_estimates,
                             const QDir &cache_folder)
    : QDialog(parent),
      ui_(new Ui::MLECalibrationWindow()),
      estimates_(current_estimates),
      pattern_(pattern),
      settings_(settings),
      last_capture_index_(0),
      cache_folder_(cache_folder),
      sound_dir_("/usr/share/sounds/freedesktop/"),
      capture_button_format_(settings->LoopCapture() ? "Stop Capturing (%ld)" : "Capture: (%ld)"),
      timer_(new QTimer(this)),
      next_capture_time_(next_capture_time_.max()) {
  ui_->setupUi(this);

  connect(this,
          &MLECalibrator::NewImagesArrived,
          ui_->listView,
          static_cast<void (gago::calibration::ImageSetView::*)(const QStringList &)>(&gago::calibration::ImageSetView::Append));

  connect(ui_->captureButton, &QPushButton::pressed, this, &MLECalibrator::OnCaptureButtonClicked);

  connect(ui_->calibrateButton, &QPushButton::pressed, this, &MLECalibrator::OnCalibrateButtonClicked);
  connect(ui_->saveButton, &QPushButton::pressed, this, &MLECalibrator::OnSaveButtonClicked);

  connect(this, &MLECalibrator::DisableControlElements, this, &MLECalibrator::DisableControlElementsSlot);
  connect(this, &MLECalibrator::EnableControlElements, this, &MLECalibrator::EnableControlElementsSlot);
  connect(this, &MLECalibrator::PlaySound, this, &MLECalibrator::PlaySoundFromPath);
  connect(this, &MLECalibrator::CamerasSet, this, &MLECalibrator::SetLabelText);
  connect(timer_, SIGNAL(timeout()), this, SLOT(TimerElapsed()));

  connect(this, SIGNAL(PictureTaken()), timer_, SLOT(stop()));

  if (settings_->LoopCapture()) {
    connect(this, SIGNAL(PictureTaken()), this, SLOT(SetNextCaptureTime()));
    ui_->captureButton->setText("Start capturing");
  }

  sound_effects_["capture"] = new QMediaPlayer(this);
  sound_effects_["capture"]->setMedia(QUrl::fromLocalFile(sound_dir_.filePath("stereo/screen-capture.oga")));
  sound_effects_["error"] = new QMediaPlayer(this);
  sound_effects_["error"]->setMedia(QUrl::fromLocalFile(sound_dir_.filePath("stereo/dialog-error.oga")));

  sound_effects_["tick"] = new QMediaPlayer(this);
  sound_effects_["tick"]->setMedia(QUrl::fromLocalFile("/usr/share/sounds/ubuntu/notifications/Slick.ogg"));

  ui_->listView->setAutoFillBackground(true);
}

MLECalibrator::~MLECalibrator() {
  delete ui_;
  if (timer_)
    delete timer_;
}

void MLECalibrator::Close() {
  close();
}

int MLECalibrator::Calibrate() {
  return exec();
}

void MLECalibrator::Notify(const std::shared_ptr<std::vector<io::video::Capture>> &ptr) {
  std::vector<cv::Mat> images;
  for (int j = 0; j < ptr->size(); ++j) {
    cv::Mat mat((*ptr)[j].height, (*ptr)[j].width, CV_8UC3,(void *)(*ptr)[j].data.data());
    images.push_back(mat.clone());
  }
  std::vector<std::vector<cv::Point2f>> pts;
  bool found = pattern_->Extract(images, pts);

  if (next_capture_time_ < std::chrono::high_resolution_clock::now()) {

    next_capture_time_ = next_capture_time_.max();
    if (found) {
      ++last_capture_index_;
      emit PlaySound("capture");

      QStringList filenames;

      for (const io::video::Capture &capture: *ptr) {
        QString filename = QString::asprintf(format, capture.camera->GetName().c_str(), last_capture_index_);
        filename = settings_->ImageSaveFolder().filePath(filename);

        QImage image(capture.data.data(),
                     capture.width,
                     capture.height,
                     capture.width * capture.channels,
                     QImage::Format_RGB888);
        QImageWriter writer(filename);
        writer.write(image);
        filenames.append(filename);
      }
      emit NewImagesArrived(filenames);

    } else
        emit PlaySound("error");

    if (!settings_->LoopCapture()) {
      capture_in_progress_ = false;
      ResetNextCaptureTime();
      ui_->captureButton->setText("Capture");
      emit EnableControlElements();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    emit PictureTaken();
  }

  for (int i = 0; i < ptr->size(); ++i) {
    cv::Mat mat = cv::Mat((*ptr)[i].height, (*ptr)[i].width, CV_8UC3, (void *)(*ptr)[i].data.data()).clone();
    pattern_->DrawPattern(mat, pts[i]);
    players_[i]->ShowImage(mat);
  }

}

void MLECalibrator::SetCameras(const std::vector<const io::video::CameraMeta *> &vector) {
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

    player_layout->addWidget(players_[i]);
    QLabel *cam_label = new QLabel("Uncalibrated");
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    font.setWeight(75);
    cam_label->setFont(font);
    cam_labels_.append(cam_label);
    cam_label->setMaximumHeight(50);
    cam_label->setFixedHeight(50);
    player_layout->addWidget(cam_label);
  }
  ui_->listView->SetCameraNames(cam_names);

  RestoreFilenames(format, cam_names);
  if(!estimates_.intrinsic_parameters.empty())
    emit CamerasSet();


  emit EnableControlElements();
}

void MLECalibrator::OnCaptureButtonClicked() {

  if (settings_->LoopCapture()) {
    if (capture_in_progress_) {
      capture_in_progress_ = false;
      timer_->stop();
      ResetNextCaptureTime();
      ui_->captureButton->setText("Start capturing");
      EnableControlElementsSlot();
    } else {
      DisableControlElementsSlot();
      capture_in_progress_ = true;
      SetNextCaptureTime();
      ui_->captureButton->setEnabled(true);
    }
  } else {
    DisableControlElementsSlot();
    capture_in_progress_ = true;
    SetNextCaptureTime();
  }

}

void MLECalibrator::OnCalibrateButtonClicked() {
  emit DisableControlElements();

  gago::calibration::OpenCvMLE mle(pattern_, settings_->CalibrateCamerasSeparately());

  QList<gago::calibration::PatternBatch> &batches = ui_->listView->GetBatchCalibrationResults();

  // Reset the state if it was calibrated previously
  for (gago::calibration::PatternBatch &image_batch: batches) {
    image_batch.state = gago::calibration::PES_Unestimated;

    for (gago::calibration::Pattern &param : image_batch.pattern_params)
      param.state = gago::calibration::PES_Unestimated;
  }

  if (0 == mle.Calibrate(batches, estimates_)) {
    ui_->listView->Update();
    SetLabelText();
    ui_->saveButton->setEnabled(true);
  }


  emit EnableControlElements();
}

void MLECalibrator::SetLabelText() {
  if (!estimates_.R.empty()) {
    ui_->listView->SetCalibrationEstimates(estimates_);
    ui_->multiviewInfoLabel->setText(QString::asprintf("Stereo Calibration Error: %.4f", estimates_.rms));
  }

  for (int cam_idx = 0; cam_idx < estimates_.intrinsic_parameters.size(); ++cam_idx) {
    QString label_text = QString::asprintf("Average error: %.4f", estimates_.intrinsic_parameters[cam_idx].rms);

    cam_labels_[cam_idx]->setText(label_text);
  }
}

void MLECalibrator::RestoreFilenames(const char *format, QStringList cameras_) {
  QFile params_file(cache_folder_.filePath("params.csv"));
  QMap<QString, QStringList> param_map;
  if (params_file.exists()) {
    params_file.open(QFile::ReadOnly);
    QTextStream stream(&params_file);
    while (!stream.atEnd()) {
      QStringList values = stream.readLine().split(',');
      QString batch_key;
      for (int i = 6; i < values.size(); i += 6) {
        batch_key += values[i];
      }
      param_map[batch_key] = values;
    }
  }
  last_capture_index_ = 0;

  QStringList filters = {QString(format).replace("%s", cameras_[0]).replace("%03d", "*")};
  for (const QString &filename: settings_->ImageSaveFolder().entryList(filters,
                                                                       QDir::NoFilter,
                                                                       QDir::SortFlag::Name)) {
    int idx = filename.right(7).left(3).toInt();
    QStringList idx_files = {settings_->ImageSaveFolder().filePath(filename)};
    for (int i = 1; i < cameras_.size(); ++i) {
      QString cam_filename = QString::asprintf(format, cameras_[i].toStdString().c_str(), idx);
      if (settings_->ImageSaveFolder().exists(cam_filename))
        idx_files.push_back(settings_->ImageSaveFolder().filePath(cam_filename));
    }
    if (idx_files.size() != cameras_.size())
      continue;

    QString batch_key = idx_files.join("");
    gago::calibration::PatternBatch image_batch(idx_files);
    if (param_map.contains(batch_key)) {
      QStringList &str_params = param_map[batch_key];
      try_parse(str_params[1], image_batch.state);
      image_batch.rms = str_params[0].toFloat();
      for (int cam_idx = 0; cam_idx < idx_files.size(); ++cam_idx) {
        image_batch.pattern_params[cam_idx].reprojection_error = str_params[3 + 6 * cam_idx].toFloat();
        try_parse(str_params[2 + 6 * cam_idx], image_batch.pattern_params[cam_idx].state);
        image_batch.pattern_params[cam_idx].enabled = str_params[7 + 6 * cam_idx] == "true";
      }
    }
    ui_->listView->Append(image_batch);

    last_capture_index_ = std::max(last_capture_index_, idx);

  }
  ui_->listView->Update();
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

const gago::calibration::CalibrationEstimates &MLECalibrator::GetEstimates() const {
  return estimates_;
}

void MLECalibrator::OnSaveButtonClicked() {
  QList<gago::calibration::PatternBatch> &image_batches = ui_->listView->GetBatchCalibrationResults();
  QFile params_file(cache_folder_.filePath("params.csv"));
  params_file.open(QFile::WriteOnly);
  QTextStream stream(&params_file);
  for (gago::calibration::PatternBatch &image_batch: image_batches) {
    stream << image_batch.rms << "," << to_string(image_batch.state);
    for (gago::calibration::Pattern &params: image_batch.pattern_params) {
      stream << "," << to_string(params.state) << "," << params.reprojection_error << "," << params.image_size.width
             << ","
             << params.image_size.height << "," << params.filename << "," << (params.enabled ? "true" : "false");

    }
    stream << "\n";
  }

  accept();
}

void MLECalibrator::PlaySoundFromPath(const QString &path) {
  if (settings_->SoundEnabled())
    sound_effects_[path]->play();
}

void MLECalibrator::TimerElapsed() {
  auto time_remaining = next_capture_time_ - std::chrono::system_clock::now();
  long remaining_time_in_seconds = std::chrono::duration_cast<std::chrono::seconds>(time_remaining).count();
  if (next_capture_time_ != next_capture_time_.max())
    ui_->captureButton->setText(QString::asprintf(capture_button_format_,
                                                  remaining_time_in_seconds));
  qApp->processEvents();
  if (remaining_time_in_seconds > 0)
    PlaySoundFromPath("tick");

}

void MLECalibrator::SetNextCaptureTime() {
  if (!capture_in_progress_)
    return;

  if (settings_->LoopCapture() != 0)
    timer_->start();
  next_capture_time_ = (std::chrono::high_resolution_clock::now()
      + std::chrono::seconds(settings_->CaptureWaitTime()));
}

void MLECalibrator::ResetNextCaptureTime() {
  next_capture_time_ = next_capture_time_.max();
}

}
}
}




