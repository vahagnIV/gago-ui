//
// Created by vahagn on 1/17/20.
//

#include <QGridLayout>
#include <QLabel>
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
}

void MLECalibratorConfigurator::Apply() {
  current_settings_.calibrate_camera_first = camera_first_chkbx_->isChecked();

}

void MLECalibratorConfigurator::GetConfiguration(nlohmann::json & out_json) {
  out_json["CalibrateSeparately"] = current_settings_.calibrate_camera_first;
}

void MLECalibratorConfigurator::SetConfiguration(const nlohmann::json & json) {
  if (json.find("CalibrateSeparately") != json.end())
    current_settings_.calibrate_camera_first = json["CalibrateSeparately"];
}

const std::string & MLECalibratorConfigurator::ConfigWindowName() const {
  return window_name_;
}

}
}
}