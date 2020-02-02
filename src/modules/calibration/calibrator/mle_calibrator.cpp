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

namespace gago {
namespace gui {
namespace calibration {

MLECalibrator::MLECalibrator(QWidget *parent,
                             const std::shared_ptr<gago::calibration::pattern::IPattern> &pattern,
                             const MLEConfigurationSettings &settings) : QDialog(parent),
                                                                         ui_(new Ui::MLECalibrationWindow()),
                                                                         pattern_(pattern),
                                                                         settings_(settings),
                                                                         last_image_index(0),
                                                                         player(settings.sounds_enabled
                                                                                ? new QMediaPlayer : nullptr),
                                                                         next_capture_time_(std::numeric_limits<typeof(next_capture_time_)>::max()) {
  ui_->setupUi(this);
  connect(ui_->pushButton, &QPushButton::pressed, this, &MLECalibrator::CaptureRequested);
  connect(ui_->pushButton_2, &QPushButton::pressed, this, &MLECalibrator::OnCalibrateButtonClicked);
  connect(ui_->listView, &gago::calibration::ImageSetView::BatchRemoved, this, &MLECalibrator::BatchesRemoved);
  connect(this, &MLECalibrator::DisableControlElements, this, &MLECalibrator::DisableControlElementsSlot);
  connect(this, &MLECalibrator::EnableControlElements, this, &MLECalibrator::EnableControlElementsSlot);
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

void MLECalibrator::Notify(const std::shared_ptr<std::vector<io::video::Capture>> &ptr) {
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
      QList<QImage> images;
      for (const io::video::Capture &capture: *ptr) {
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
        files_.back().push_back(dir.filePath(filename));
        images.push_back(image);
      }
      ui_->listView->Append(GetThumbnail(images, 140));

      ++last_image_index;
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

void MLECalibrator::SetCameras(const std::vector<const io::video::CameraMeta *> &vector) {
  emit DisableControlElements();

  QStringList cam_names;
  for (int i = 0; i < vector.size(); ++i) {
    QString camera_format(vector[i]->GetName().c_str());
    cam_names.append(camera_format);

    players_.push_back(new common::VideoPlayer());
    QVBoxLayout *player_layout = new QVBoxLayout();
    this->ui_->horizontalLayout_3->addLayout(player_layout);
    players_[i]->setMinimumWidth(200);
    players_[i]->setMinimumHeight(200);
    player_layout->addWidget(players_[i], 0, Qt::AlignHCenter);
    QString label_text = "Matrix\t Distortion\n";
    label_text += "1450.256\t0\t541.68\t0.0012, 0.168\n";
    label_text += "0\t1449.368\t321.19\t0.00785,0.0147\n";
    label_text += "0\t0\t1\n";
    label_text = "Average error:\t0.1587549\n"
                 "Maximal error:\t0.4578\n"
                 "Minimal error:\t0.078547";

    player_layout->addStretch(1);
    player_layout->addWidget(new QLabel(label_text));

  }

  RestoreFilenames(format, cam_names);

  for (QStringList &filenames: files_) {
    QList<QImage> images = GetImages(filenames);
    QImage thumbnail = GetThumbnail(images, 140);
    ui_->listView->Append(thumbnail);
  }
  emit EnableControlElements();
}

void MLECalibrator::CaptureRequested() {
  DisableControlElements();
  ui_->pushButton->setEnabled(false);
  next_capture_time_ = (std::chrono::high_resolution_clock::now().time_since_epoch()
      + std::chrono::seconds(settings_.wait_time)).count();
}

void MLECalibrator::OnCalibrateButtonClicked() {
  emit DisableControlElements();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  gago::calibration::OpenCvMLE mle(pattern_, settings_);
  gago::calibration::CalibrationEstimates estimates;
  QList<int> batch_map;
  QList<QList<gago::calibration::PatternEstimationParameters>> pattern_estimation_parameters;
  QList<cv::Size> sizes;
  mle.Calibrate(files_, estimates, pattern_estimation_parameters, sizes, batch_map);

  for (int batch_idx = 0; batch_idx < files_.size(); ++batch_idx) {
    int image_idx = batch_map[batch_idx];
    if (-1 == image_idx)
      continue;
    QList<QImage> images = GetImages(files_[batch_idx]);
    for (int camera_idx = 0; camera_idx < files_[batch_idx].size(); ++camera_idx) {
      QImage &image = images[camera_idx];
      QColor color;
      if (pattern_estimation_parameters[batch_idx][camera_idx].reprojection_error < 0.5)
        color = QColor(0, 255, 0);
      else if (pattern_estimation_parameters[batch_idx][camera_idx].reprojection_error < 1.0)
        color = QColor(0, 0, 255);
      else
        color = QColor(255, 0, 0);

      QPainter painter(&image);
      QPen pen(color, 50);
      painter.setPen(pen);
      painter.drawRect(0, 0, image.width(), image.height());
    }
    ui_->listView->Replace(batch_idx, GetThumbnail(images, 140));
  }

  cv::Mat rmap[2][2];
  cv::Mat R1, R2, P1, P2, Q;
  cv::Rect validRoi[2];
  stereoRectify(estimates.intrinsic_parameters[0].camera_matrix,
                estimates.intrinsic_parameters[0].distortion_coefficients,
                estimates.intrinsic_parameters[1].camera_matrix,
                estimates.intrinsic_parameters[1].distortion_coefficients,
                sizes[0],
                estimates.R,
                estimates.T,
                R1,
                R2,
                P1,
                P2,
                Q,
                cv::CALIB_ZERO_DISPARITY,
                1,
                sizes[0],
                &validRoi[0],
                &validRoi[1]);

  std::cout << R1 << std::endl;
  std::cout << R2 << std::endl;
  std::cout << P1 << std::endl;
  std::cout << P2 << std::endl;

  initUndistortRectifyMap(estimates.intrinsic_parameters[0].camera_matrix,
                          estimates.intrinsic_parameters[0].distortion_coefficients,
                          R1,
                          P1,
                          sizes[0],
                          CV_16SC2,
                          rmap[0][0],
                          rmap[0][1]);
  initUndistortRectifyMap(estimates.intrinsic_parameters[1].camera_matrix,
                          estimates.intrinsic_parameters[1].distortion_coefficients,
                          R2,
                          P2,
                          sizes[0],
                          CV_16SC2,
                          rmap[1][0],
                          rmap[1][1]);

  cv::Mat canvas;
  double sf;
  int w, h;
  sf = 600. / MAX(sizes[0].width, sizes[0].height);
  w = cvRound(sizes[0].width * sf);
  h = cvRound(sizes[0].height * sf);
  canvas.create(h, w * 2, CV_8UC3);

  for (int i = 0; i < files_.size(); i++) {
    if (batch_map[i] == -1)
      continue;

    for (int k = 0; k < 2; k++) {
      cv::Mat img = cv::imread(files_[i][k].toStdString(), 0), rimg, cimg;
      cv::remap(img, rimg, rmap[k][0], rmap[k][1], cv::INTER_LINEAR);
      cv::cvtColor(rimg, cimg, cv::COLOR_GRAY2BGR);
//      cv::Mat canvasPart = !isVerticalStereo ? canvas(Rect(w * k, 0, w, h)) : canvas(Rect(0, h * k, w, h));
      cv::Mat canvasPart = canvas(cv::Rect(w * k, 0, w, h));
      cv::resize(cimg, canvasPart, canvasPart.size(), 0, 0, cv::INTER_AREA);

      cv::Rect vroi(cvRound(validRoi[k].x * sf), cvRound(validRoi[k].y * sf),
                    cvRound(validRoi[k].width * sf), cvRound(validRoi[k].height * sf));
      rectangle(canvasPart, vroi, cv::Scalar(0, 0, 255), 3, 8);

    }

    for (int j = 0; j < canvas.rows; j += 16)
      line(canvas, cv::Point(0, j), cv::Point(canvas.cols, j), cv::Scalar(0, 255, 0), 1, 8);

    imshow("rectified", canvas);
    char c = (char) cv::waitKey();
    if (c == 27 || c == 'q' || c == 'Q')
      break;
  }

  emit EnableControlElements();
}

void MLECalibrator::RestoreFilenames(const char *format, QStringList cameras_) {
  last_image_index = 0;
  QDir directory(settings_.image_save_folder);
  QStringList filters = {QString(format).replace("%s", cameras_[0]).replace("%03d", "*")};
  for (const QString &filename: directory.entryList(filters)) {
    int idx = filename.right(7).left(3).toInt();
    QStringList idx_files = {directory.filePath(filename)};
    for (int i = 1; i < cameras_.size(); ++i) {
      QString cam_filename = QString::asprintf(format, cameras_[i].toStdString().c_str(), idx);
      if (directory.exists(cam_filename))
        idx_files.push_back(directory.filePath(cam_filename));
    }

    if (idx_files.size() == cameras_.size()) {
      files_.push_back(idx_files);
      last_image_index = std::max(last_image_index, idx);
    }
  }
}

void MLECalibrator::BatchesRemoved(int start_idx, int count) {
  emit DisableControlElements();
  for (int i = start_idx; i < start_idx + count; ++i) {
    for (int j = 0; j < files_[i].size(); ++j) {
      if (QFile::exists(files_[i][j]))
        QFile::remove(files_[i][j]);
    }
  }
  files_.erase(files_.begin() + start_idx, files_.begin() + start_idx + count);
  emit EnableControlElements();

}

void MLECalibrator::DisableControlElementsSlot() {
  control_disable_bit_mask_ = 0;
  if (ui_->pushButton->isEnabled()) {
    control_disable_bit_mask_ |= 1;
    ui_->pushButton->setEnabled(false);
  }

  if (ui_->pushButton_2->isEnabled()) {
    control_disable_bit_mask_ |= 2;
    ui_->pushButton_2->setEnabled(false);
  }

  if (ui_->listView->isEnabled()) {
    control_disable_bit_mask_ |= 4;
    ui_->listView->setEnabled(false);
  }
  qApp->processEvents();
}

void MLECalibrator::EnableControlElementsSlot() {
  if (control_disable_bit_mask_ & 1)
    ui_->pushButton->setEnabled(true);
  if (control_disable_bit_mask_ & 2)
    ui_->pushButton_2->setEnabled(true);

  if (control_disable_bit_mask_ & 4)
    ui_->listView->setEnabled(true);
  qApp->processEvents();
}

QImage MLECalibrator::GetThumbnail(const QList<QImage> &images, int max_width) {
  int total_width, max_height;
  GetTotalWidthMaxHeight(images, total_width, max_height);

  QImage thumbnail(total_width, max_height, QImage::Format_RGB888);
  QPainter painter;
  int offset = 0;
  painter.begin(&thumbnail);
  for (const QImage &image: images) {
    painter.drawImage(offset, 0, image);
    offset += image.width();
  }
  painter.end();
  return thumbnail.scaledToWidth(max_width);
}

void MLECalibrator::GetTotalWidthMaxHeight(const QList<QImage> &images,
                                           int &out_total_width,
                                           int &out_max_height) {
  out_max_height = 0;
  out_total_width = 0;
  for (const QImage &image: images) {
    out_max_height = std::max(image.height(), out_max_height);
    out_total_width += image.width();
  }
}

QList<QImage> MLECalibrator::GetImages(const QStringList &filenames) {
  QList<QImage> images;
  for (const QString &filename: filenames)
    images.append(QImage(filename));
  return images;
}

QImage MLECalibrator::CreateRectifiedImage(const QList<QImage> &images,
                                           const QList<gago::calibration::PatternEstimationParameters> &pattern_parameters,
                                           const gago::calibration::CalibrationEstimates &calibration_estimates,
                                           QImage &out) {
  if (images.size() != pattern_parameters.size())
    return QImage();

}

}
}
}


