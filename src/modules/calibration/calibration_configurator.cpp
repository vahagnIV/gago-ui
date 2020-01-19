//
// Created by vahagn on 1/15/20.
//

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include "calibration_configurator.h"
#include "consts.h"
#include "calibrator_configurator_factory.h"

namespace gago {
namespace gui {
namespace configuration {

CalibrationConfigurator::CalibrationConfigurator(QObject *parent) : QObject(parent) {
  for (int i = 0; i < sizeof(SupportedPatterns) / sizeof(CalibrationPatternType); ++i) {
    calib_pattern_configurators_.push_back(CalibPatternConfiguratorFactory::CreateConfigurator(SupportedPatterns[i]));
  }
  for (int i = 0; i < sizeof(SupportedCalibrators) / sizeof(CalibratorType); ++i) {
    calibrator_configurators_.push_back(CalibratorConfiguratorFactory::Create(SupportedCalibrators[i]));
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

void CalibrationConfigurator::DrawCalibratorSide() {
  calibrator_group_box_->setLayout(new QVBoxLayout());
  calibrator_combo_ = new QComboBox();
  calibrator_group_box_->layout()->addWidget(calibrator_combo_);

  for (int i = 0; i < sizeof(SupportedCalibrators) / sizeof(CalibratorType); ++i) {
    calibrator_combo_->insertItem(i, calibrator_configurators_[i]->ConfigWindowName().c_str());
    calibrator_configurator_frames_.push_back(new QFrame());
    calibrator_configurator_frames_[i]->hide();
    calibrator_group_box_->layout()->addWidget(calibrator_configurator_frames_[i]);
    calibrator_configurators_[i]->DrawConfigurationPage(calibrator_configurator_frames_[i]);
  }

  QObject::connect(calibrator_combo_,
                   QOverload<int>::of(&QComboBox::activated),
                   this,
                   &CalibrationConfigurator::CalibratorChanged);

  calibrator_combo_->setCurrentIndex(0);
  calibrator_combo_->activated(0);

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
  for (IConfigurator *configurator: calibrator_configurators_)
    configurator->Apply();
  current_calibration_settings_.calib_pattern_type = SupportedPatterns[patterns_combo->currentIndex()];
}

void CalibrationConfigurator::GetConfiguration(nlohmann::json &out_json) {
  for (IConfigurator *configurator: calib_pattern_configurators_)
    configurator->GetConfiguration(out_json["Pattern"][configurator->ConfigWindowName()]);
  for (IConfigurator *configurator: calibrator_configurators_)
    configurator->GetConfiguration(out_json["Calibrator"][configurator->ConfigWindowName()]);
}

void CalibrationConfigurator::SetConfiguration(const nlohmann::json &json) {
  if (json.find("Pattern") != json.end())
    for (IConfigurator *configurator: calib_pattern_configurators_) {
      if (json["Pattern"].find(configurator->ConfigWindowName()) != json["Pattern"].end()) {
        configurator->SetConfiguration(json["Pattern"][configurator->ConfigWindowName()]);
      }
    }

  if (json.find("Calibrator") != json.end())
    for (IConfigurator *configurator: calibrator_configurators_) {
      if (json["Calibrator"].find(configurator->ConfigWindowName()) != json["Calibrator"].end()) {
        configurator->SetConfiguration(json["Calibrator"][configurator->ConfigWindowName()]);
      }
    }
  if (!(json.find("SelectedPatternType") != json.end()
      && try_parse(json["SelectedPatternType"], current_calibration_settings_.calib_pattern_type)))
    current_calibration_settings_.calib_pattern_type = SupportedPatterns[0];

  if (!(json.find("SelectedCalibratorType") != json.end()
      && try_parse(json["SelectedCalibratorType"], current_calibration_settings_.calibrator_type)))
    current_calibration_settings_.calibrator_type = SupportedCalibrators[0];

}

const std::string &CalibrationConfigurator::ConfigWindowName() const {
  return window_name;
}

void CalibrationConfigurator::CalibrationPatternChanged(int idx) {
  for (int i = 0; i < calib_pattern_configurator_frames_.size(); ++i)
    idx == i ? calib_pattern_configurator_frames_[i]->show() : calib_pattern_configurator_frames_[i]->hide();
}

void CalibrationConfigurator::CalibratorChanged(int idx) {
  for (int i = 0; i < calibrator_configurator_frames_.size(); ++i)
    idx == i ? calibrator_configurator_frames_[i]->show() : calibrator_configurator_frames_[i]->hide();
}

IConfigurator *CalibrationConfigurator::GetActivePatternConfigurator() {
  for (int i = 0; i < calib_pattern_configurators_.size(); ++i) {
    if (current_calibration_settings_.calib_pattern_type == SupportedPatterns[i])
      return calib_pattern_configurators_[i];
  }
}

IConfigurator *CalibrationConfigurator::GetActiveCalibratorConfigurator() {
  for (int i = 0; i < calibrator_configurators_.size(); ++i) {
    if (current_calibration_settings_.calibrator_type == SupportedCalibrators[i])
      return calibrator_configurators_[i];
  }
}

CalibrationConfigurator::~CalibrationConfigurator() {
  for (int i = 0; i < calib_pattern_configurators_.size(); ++i) {
    CalibPatternConfiguratorFactory::DisposeConfigurator(calib_pattern_configurators_[i]);
  }
  for (int i = 0; i < calibrator_configurators_.size(); ++i) {
    CalibratorConfiguratorFactory::DisposeConfigurator(calibrator_configurators_[i]);
  }
}

}
}
}