//
// Created by vahagn on 2/14/20.
//

#include "mle_calibrator_settings.h"
#include "mle_calibrator_configurator.h"
namespace gago {
namespace gui {
namespace configuration {

IConfigurator * MLECalibratorSettings::GetConfigurator() {
  return new MLECalibratorConfigurator(this);
}

void MLECalibratorSettings::DisposeConfigurator(IConfigurator * configurator) {

}

void MLECalibratorSettings::ApplyConfiguration(QSettings & settings, IConfigurator * configurator) {
  MLECalibratorConfigurator * cfg = (MLECalibratorConfigurator *) configurator;
  calibrate_cameras_separately_ = cfg->CalibrateCamerasSeparately();
  capture_wait_time_ = cfg->CaptureWaitTime();
  image_save_folder_ = cfg->ImageSaveFolder();
  fix_aspect_ratio_ = cfg->FixAspectRatio();
  sounds_enabled_ = cfg->SoundEnabled();
  loop_capture_ = cfg->LoopCapture();
  distortion_model_ = cfg->Dis
  settings.setValue("calibrate_cameras_separately", calibrate_cameras_separately_);
  settings.setValue("capture_wait_time", capture_wait_time_);
  settings.setValue("image_save_folder", image_save_folder_.path());
  settings.setValue("fix_aspect_ratio", fix_aspect_ratio_);
  settings.setValue("sound_enabled", sounds_enabled_);
  settings.setValue("loop_capture", loop_capture_);
  settings.setValue("distortion_model", distortion_model_);
}

void MLECalibratorSettings::Configure(QSettings & settings) {
  if (settings.contains("calibrate_cameras_separately"))
    calibrate_cameras_separately_ = settings.value("calibrate_cameras_separately").toBool();

  if (settings.contains("capture_wait_time"))
    capture_wait_time_ = settings.value("capture_wait_time").toInt();

  if (settings.contains("image_save_folder"))
    image_save_folder_.setPath(settings.value("image_save_folder").toString());

  if (settings.contains("fix_aspect_ratio"))
    fix_aspect_ratio_ = settings.value("fix_aspect_ratio").toBool();

  if (settings.contains("sound_enabled"))
    sounds_enabled_ = settings.value("sound_enabled").toBool();

  if (settings.contains("loop_capture"))
    loop_capture_ = settings.value("loop_capture").toBool();

  if (settings.contains("distortion_model"))
    distortion_model_ = static_cast<DistModel>(settings.value("distortion_model").toInt());

}

const QString & MLECalibratorSettings::GetName() const {
  return name_;
}

bool MLECalibratorSettings::CalibrateCamerasSeparately() const {
  return calibrate_cameras_separately_;
}

int MLECalibratorSettings::CaptureWaitTime() const {
  return capture_wait_time_;
}

const QDir & MLECalibratorSettings::ImageSaveFolder() const {
  return image_save_folder_;
}

bool MLECalibratorSettings::FixAspectRatio() const {
  return fix_aspect_ratio_;
}

bool MLECalibratorSettings::SoundEnabled() const {
  return sounds_enabled_;
}

bool MLECalibratorSettings::LoopCapture() const {
  return loop_capture_;
}

DistModel MLECalibratorSettings::DistortionModel() const {
  return distortion_model_;
}
}
}
}