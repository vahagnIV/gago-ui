//
// Created by vahagn on 2/14/20.
//
#include <QLabel>
#include "checkerboard_pattern_configurator.h"
#include "checkerboard_pattern_settings.h"

namespace gago {
namespace gui {
namespace configuration {

CheckerboardPatternConfigurator::CheckerboardPatternConfigurator(CheckerboardPatternSettings *settings) : setting_(
    settings) {

}

void CheckerboardPatternConfigurator::DrawConfigurationPage(QWidget *widget) {
  layout_ = new QGridLayout();
  widget->setLayout(layout_);
  layout_->addWidget(new QLabel("Nx:"), 0, 1, 1, 1);
  nx_box_ = new QSpinBox();
  layout_->addWidget(nx_box_, 0, 2, 1, 3);
  nx_box_->setValue(setting_->GetNx());
  layout_->addWidget(new QLabel("Ny:"), 1, 1, 1, 1);
  ny_box_ = new QSpinBox();
  ny_box_->setValue(setting_->GetNy());
  layout_->addWidget(ny_box_, 1, 2, 1, 3);
}

void CheckerboardPatternConfigurator::Apply() {

}

const QString & CheckerboardPatternConfigurator::ConfigWindowName() const {
  return name_;
}

int CheckerboardPatternConfigurator::GetNx() const {
  return nx_box_->value();
}
int CheckerboardPatternConfigurator::GetNy() const {
  return ny_box_->value();
}

}
}
}