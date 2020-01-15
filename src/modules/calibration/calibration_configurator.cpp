//
// Created by vahagn on 1/15/20.
//

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include "calibration_configurator.h"

namespace gago {
namespace gui {
namespace configuration {

void CalibrationConfigurator::DrawConfigurationPage(QWidget *widget) {
  QHBoxLayout * main_layout = new QHBoxLayout();
  widget->setLayout(main_layout);

  QVBoxLayout * left_layout = new QVBoxLayout();
  QVBoxLayout * right_layout = new QVBoxLayout();
  main_layout->addLayout(left_layout);
  main_layout->addLayout(right_layout);
  QGroupBox *calibrator_choose_box = new QGroupBox("Calibrator");
  left_layout->addWidget(calibrator_choose_box);

  left_layout->addStretch(1);

  QGroupBox *pattern_group_box = new QGroupBox("Pattern");
  right_layout->addWidget(pattern_group_box );
  right_layout->addStretch(1);
}

void CalibrationConfigurator::Apply() {

}

void CalibrationConfigurator::GetConfiguration(nlohmann::json & out_json) {

}

void CalibrationConfigurator::SetConfiguration(const nlohmann::json & json) {

}

const std::string & CalibrationConfigurator::ConfigWindowName() const {
  return window_name;
}

}
}
}