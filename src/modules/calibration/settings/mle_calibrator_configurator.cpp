//
// Created by vahagn on 1/17/20.
//

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>

#include "mle_calibrator_configurator.h"
#include "mle_calibrator_settings.h"
#include "calibrator_type.h"

namespace gago {
namespace gui {
namespace configuration {

MLECalibratorConfigurator::MLECalibratorConfigurator(MLECalibratorSettings * settings)
    : QObject(nullptr), settings_(settings) {

}

const QMap<QString, DistModel>
    DistortionModelStr = {{QString("Barrel (5)"), BARREL5}, {QString("Kannala Brandt"), KANNALA_BRANDT}};
const QStringList DistortionModels = {QString("Barrel (5)"), QString("Kannala Brandt")};

void MLECalibratorConfigurator::DrawConfigurationPage(QWidget * widget) {
  QGridLayout * layout = new QGridLayout();
  widget->setLayout(layout);

  camera_first_chkbx_ = new QCheckBox("Calibrate cameras separately:");
  camera_first_chkbx_->setChecked(settings_->CalibrateCamerasSeparately());
  layout->addWidget(camera_first_chkbx_, 0, 0, 1, 4);
  folder_line_edit_ = new QLineEdit(settings_->ImageSaveFolder().path());
  layout->addWidget(new QLabel("Image folder:"), 1, 0, 1, 1);
  layout->addWidget(folder_line_edit_, 1, 1, 1, 2);
  QPushButton * pick_folder_btn = new QPushButton("Choose");
  QObject::connect(pick_folder_btn, &QPushButton::pressed, [=]() {
    QFileDialog * dialog = new QFileDialog(widget);
    dialog->setOption(QFileDialog::ShowDirsOnly);
    dialog->setFileMode(QFileDialog::Directory);
    if (dialog->exec() == QFileDialog::Accepted)
      folder_line_edit_->setText(dialog->directory().absolutePath());
    delete dialog;
  });
  layout->addWidget(pick_folder_btn, 1, 3, 1, 1);
  layout->addWidget(new QLabel("Wait time:"), 2, 0, 1, 2);
  wait_time_spinbox_ = new QSpinBox();
  wait_time_spinbox_->setValue(settings_->CaptureWaitTime());
  layout->addWidget(wait_time_spinbox_, 2, 1, 1, 3);
  connect(wait_time_spinbox_, SIGNAL(valueChanged(int)), this, SLOT(ValidateWaitTime(int)));

  sound_enabled_chkbx_ = new QCheckBox("Sound enabled:");
  sound_enabled_chkbx_->setChecked(settings_->SoundEnabled());
  layout->addWidget(sound_enabled_chkbx_, 3, 0, 1, 4);

  loop_capture_chkbx_ = new QCheckBox("Loop capture:");
  loop_capture_chkbx_->setChecked(settings_->LoopCapture());
  layout->addWidget(loop_capture_chkbx_, 4, 0, 1, 4);
  connect(loop_capture_chkbx_, SIGNAL(stateChanged(int)), this, SLOT(ValidateWaitTime(int)));

  distortion_model_combo_ = new QComboBox();
  int selected_distortion_model_i = 0;
  for (int i = 0; i < DistortionModels.size(); ++i) {
    distortion_model_combo_->insertItem(i, DistortionModels[i]);
    if (settings_->DistortionModel() == DistortionModelStr[DistortionModels[i]]) {
      selected_distortion_model_i = i;
    }
  }
  layout->addWidget(distortion_model_combo_, 4, 0, 1, 4);
  distortion_model_combo_->setCurrentIndex(selected_distortion_model_i);
  distortion_model_combo_->activated(selected_distortion_model_i);
}



void MLECalibratorConfigurator::ValidateWaitTime(int value) {
  if (loop_capture_chkbx_->isChecked())
    wait_time_spinbox_->setValue(std::max(1, wait_time_spinbox_->value()));
}

void MLECalibratorConfigurator::Apply() {
}

const QString & MLECalibratorConfigurator::ConfigWindowName() const {
  return window_name_;
}

bool MLECalibratorConfigurator::CalibrateCamerasSeparately() const {
  return camera_first_chkbx_->checkState() == Qt::Checked;
}

int MLECalibratorConfigurator::CaptureWaitTime() const {
  return wait_time_spinbox_->value();
}

QDir MLECalibratorConfigurator::ImageSaveFolder() const {
  return QDir(folder_line_edit_->text());
}

bool MLECalibratorConfigurator::FixAspectRatio() const {
  return false;
}

bool MLECalibratorConfigurator::SoundEnabled() const {
  return sound_enabled_chkbx_->checkState() == Qt::Checked;
}

bool MLECalibratorConfigurator::LoopCapture() const {
  return loop_capture_chkbx_->checkState() == Qt::Checked;
}

DistModel MLECalibratorConfigurator::DistortionModel() {
  return DistortionModelStr[distortion_model_combo_->currentText()];
}

}
}
}