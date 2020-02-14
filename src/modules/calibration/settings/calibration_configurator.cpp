//
// Created by vahagn on 1/15/20.
//

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include "calibration_configurator.h"
#include "calibration_settings.h"
#include "consts.h"

namespace gago {
namespace gui {
namespace configuration {

CalibrationConfigurator::CalibrationConfigurator(const QList<IConfigurator *> & calib_pattern_configurators,
                                                 const QList<IConfigurator *> & calibrator_configurators,
                                                 CalibrationSettings *settings)
    : QObject(nullptr), calib_pattern_configurators_(calib_pattern_configurators),
      calibrator_configurators_(calibrator_configurators),
      settings_(settings) {

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

  int selected_index = 0;
  for (IConfigurator *calibrator_configurator: calibrator_configurators_) {
    calibrator_combo_->insertItem(calibrator_combo_->model()->rowCount(), calibrator_configurator->ConfigWindowName());
    calibrator_configurator_frames_.push_back(new QFrame());
    if (SupportedCalibrators[calibrator_configurator_frames_.size() - 1] == settings_->GetCalibratorType())
      selected_index = calibrator_configurator_frames_.size() - 1;
    calibrator_configurator_frames_.back()->hide();
    calibrator_group_box_->layout()->addWidget(calibrator_configurator_frames_.back());
    calibrator_configurator->DrawConfigurationPage(calibrator_configurator_frames_.back());
  }

  QObject::connect(calibrator_combo_,
                   QOverload<int>::of(&QComboBox::activated),
                   this,
                   &CalibrationConfigurator::CalibratorChanged);

  calibrator_combo_->setCurrentIndex(selected_index);
  calibrator_combo_->activated(selected_index);

}

void CalibrationConfigurator::DrawPatternSide() {
  patterns_combo = new QComboBox();
  pattern_group_box_->setLayout(new QVBoxLayout());
  pattern_group_box_->layout()->addWidget(patterns_combo);

  int selected_index = 0;
  for (IConfigurator *pattern_configurator: calib_pattern_configurators_) {
    patterns_combo->insertItem(patterns_combo->model()->rowCount(), pattern_configurator->ConfigWindowName());
    calib_pattern_configurator_frames_.push_back(new QFrame());
    calib_pattern_configurator_frames_.back()->hide();
    if (SupportedPatterns[calibrator_configurator_frames_.size() - 1] == settings_->GetCalibrationPatternType())
      selected_index = calibrator_configurator_frames_.size() - 1;

    pattern_group_box_->layout()->addWidget(calib_pattern_configurator_frames_.back());
    pattern_configurator->DrawConfigurationPage(calib_pattern_configurator_frames_.back());
  }
  QObject::connect(patterns_combo,
                   QOverload<int>::of(&QComboBox::activated),
                   this,
                   &CalibrationConfigurator::CalibrationPatternChanged);
  patterns_combo->setCurrentIndex(selected_index);
  patterns_combo->activated(selected_index);

}

void CalibrationConfigurator::Apply() {
  for (IConfigurator *configurator: calib_pattern_configurators_)
    configurator->Apply();
  for (IConfigurator *configurator: calibrator_configurators_)
    configurator->Apply();
}

const QString & CalibrationConfigurator::ConfigWindowName() const {
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

CalibrationConfigurator::~CalibrationConfigurator() {
}

CalibrationPatternType CalibrationConfigurator::GetSelectedPatternType() const {
  return SupportedPatterns[patterns_combo->currentIndex()];;
}
CalibratorType CalibrationConfigurator::GetSelectedCalibratorType() const {
  return SupportedCalibrators[calibrator_combo_->currentIndex()];
}

const QList<IConfigurator *> & CalibrationConfigurator::GetPatternConfigurators() const {
  return calib_pattern_configurators_;
}

const QList<IConfigurator *> & CalibrationConfigurator::GetCalibratorConfigurators() const {
  return calibrator_configurators_;
}

QList<IConfigurator *> & CalibrationConfigurator::GetPatternConfigurators() {
  return calib_pattern_configurators_;
}

QList<IConfigurator *> & CalibrationConfigurator::GetCalibratorConfigurators() {
  return calibrator_configurators_;
}

}
}
}