//
// Created by vahagn on 1/17/20.
//

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>

#include "mle_calibrator_configurator.h"
#include "calibrator_type.h"

namespace gago {
namespace gui {
namespace configuration {

MLECalibratorConfigurator::MLECalibratorConfigurator() : window_name_(to_string(CalibratorType::MLE_Calibrator)) {

}

void MLECalibratorConfigurator::DrawConfigurationPage(QWidget *widget) {
  QGridLayout *layout = new QGridLayout();
  widget->setLayout(layout);

  camera_first_chkbx_ = new QCheckBox("Calibrate cameras separately:");
  camera_first_chkbx_->setChecked(current_settings_.calibrate_camera_first);
  layout->addWidget(camera_first_chkbx_, 0, 0, 1, 4);
  folder_line_edit_ = new QLineEdit(current_settings_.image_save_folder.isEmpty() ? "/tmp"
                                                                                  : current_settings_.image_save_folder);
  layout->addWidget(new QLabel("Image folder:"), 1, 0, 1, 1);
  layout->addWidget(folder_line_edit_, 1, 1, 1, 2);
  QPushButton *pick_folder_btn = new QPushButton("Choose");
  QObject::connect(pick_folder_btn, &QPushButton::pressed, [=]() {
    QFileDialog *dialog = new QFileDialog(widget);
    dialog->setOption(QFileDialog::ShowDirsOnly);
    dialog->setFileMode(QFileDialog::Directory);
    if (dialog->exec() == QFileDialog::Accepted)
      folder_line_edit_->setText(dialog->directory().absolutePath());
    delete dialog;
  });
  layout->addWidget(pick_folder_btn, 1, 3, 1, 1);
  layout->addWidget(new QLabel("Wait time:"), 2, 0, 1, 2);
  wait_time_spinbox_ = new QSpinBox();
  wait_time_spinbox_->setValue(current_settings_.wait_time);
  layout->addWidget(wait_time_spinbox_, 2, 1, 1, 3);
  sound_enabled_chkbx_ = new QCheckBox("Sound enabled:");
  sound_enabled_chkbx_->setChecked(current_settings_.sounds_enabled);
  layout->addWidget(sound_enabled_chkbx_, 3, 0, 1, 4);

}

void MLECalibratorConfigurator::Apply() {
  current_settings_.calibrate_camera_first = camera_first_chkbx_->isChecked();
  current_settings_.image_save_folder = folder_line_edit_->text();
  current_settings_.wait_time = wait_time_spinbox_->value();
  current_settings_.sounds_enabled = sound_enabled_chkbx_->isChecked();
}
/*
void MLECalibratorConfigurator::GetConfiguration(nlohmann::json & out_json) {
  out_json["CalibrateSeparately"] = current_settings_.calibrate_camera_first;
  out_json["ImageSaveFolder"] = current_settings_.image_save_folder.toStdString();
  out_json["WaitTime"] = current_settings_.wait_time;
  out_json["SoundEnabled"] = current_settings_.sounds_enabled;
}

void MLECalibratorConfigurator::SetConfiguration(const nlohmann::json & json) {
  if (json.find("CalibrateSeparately") != json.end())
    current_settings_.calibrate_camera_first = json["CalibrateSeparately"];

  if (json.find("ImageSaveFolder") != json.end())
    current_settings_.image_save_folder = QString::fromStdString(json["ImageSaveFolder"]);

  if (json.find("WaitTime") != json.end())
    current_settings_.wait_time = json["WaitTime"];

  if (json.find("SoundEnabled") != json.end())
    current_settings_.sounds_enabled = json["SoundEnabled"];
}*/

const QString & MLECalibratorConfigurator::ConfigWindowName() const {
  return window_name_;
}

}
}
}