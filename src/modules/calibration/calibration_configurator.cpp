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

CalibrationConfigurator::CalibrationConfigurator(QObject *parent) : QObject(parent) {
  for (int i = 0; i < sizeof(SupportedPatterns) / sizeof(CalibrationPatternType); ++i) {
    calib_pattern_configurators_.push_back(calib_pattern_configurator_factory_.CreateConfigurator(SupportedPatterns[i]));
  }
}

void CalibrationConfigurator::DrawConfigurationPage(QWidget *widget) {
  QHBoxLayout *main_layout = new QHBoxLayout();
  widget->setLayout(main_layout);

  QVBoxLayout *left_layout = new QVBoxLayout();
  QVBoxLayout *right_layout = new QVBoxLayout();
  main_layout->addLayout(left_layout);
  main_layout->addLayout(right_layout);

  // Calibrator
  calibrator_group_box_ = new QGroupBox("Calibrator");
  left_layout->addWidget(calibrator_group_box_);
  DrawCalibratorSide();

  left_layout->addStretch(1);

  // Pattern
  pattern_group_box_ = new QGroupBox("Calibration Pattern");
  right_layout->addWidget(pattern_group_box_);
  DrawPatternSide();

  right_layout->addStretch(1);
}

void CalibrationConfigurator::DrawCalibratorSide()  {
  QComboBox * calibrator = new QComboBox();
  calibrator_group_box_->setLayout(new QVBoxLayout());
  calibrator_group_box_->layout()->addWidget(calibrator);
}

void CalibrationConfigurator::DrawPatternSide() {
  patterns_combo = new QComboBox();
  pattern_group_box_->setLayout(new QVBoxLayout());
  pattern_group_box_->layout()->addWidget(patterns_combo);

  for (int i = 0; i < sizeof(SupportedPatterns) / sizeof(CalibrationPatternType); ++i) {
    patterns_combo->insertItem(i, to_string(SupportedPatterns[i]).c_str());
    calib_pattern_configurator_frames_.push_back(new QFrame());
    calib_pattern_configurator_frames_[i]->hide();
    pattern_group_box_->layout()->addWidget(calib_pattern_configurator_frames_[i]);
    calib_pattern_configurators_[i]->DrawConfigurationPage(calib_pattern_configurator_frames_[i]);
  }
  QObject::connect(patterns_combo,
                   QOverload<int>::of(&QComboBox::activated),
                   this,
                   &CalibrationConfigurator::CalibrationPatternChanged);
  patterns_combo->setCurrentIndex(0);
  patterns_combo->activated(0);

}

void CalibrationConfigurator::Apply() {
  for (IConfigurator *configurator: calib_pattern_configurators_)
    configurator->Apply();
  current_calibration_settings_.calib_pattern_type = SupportedPatterns[patterns_combo->currentIndex()];
}

void CalibrationConfigurator::GetConfiguration(nlohmann::json & out_json) {
  for (IConfigurator *configurator: calib_pattern_configurators_)
    configurator->GetConfiguration(out_json["Pattern"][configurator->ConfigWindowName()]);
}

void CalibrationConfigurator::SetConfiguration(const nlohmann::json & json) {
  if (json.find("Pattern") != json.end())
    for (IConfigurator *configurator: calib_pattern_configurators_) {
      if (json["Pattern"].find(configurator->ConfigWindowName()) != json["Pattern"].end()) {
        configurator->SetConfiguration(json["Pattern"][configurator->ConfigWindowName()]);
      }
    }
}

const std::string & CalibrationConfigurator::ConfigWindowName() const {
  return window_name;
}

void CalibrationConfigurator::CalibrationPatternChanged(int idx) {
  for (int i = 0; i < calib_pattern_configurator_frames_.size(); ++i)
    idx == i ? calib_pattern_configurator_frames_[i]->show() : calib_pattern_configurator_frames_[i]->hide();
}

IConfigurator *CalibrationConfigurator::GetActivePatternConfigurator() {
  for (int i = 0; i < calib_pattern_configurators_.size(); ++i) {
    if (current_calibration_settings_.calib_pattern_type == SupportedPatterns[i])
      return calib_pattern_configurators_[i];
  }
}

CalibrationConfigurator::~CalibrationConfigurator() {
  for (int i = 0; i < calib_pattern_configurators_.size(); ++i) {
    calib_pattern_configurator_factory_.DisposeConfigurator(calib_pattern_configurators_[i]);
  }
}

}
}
}