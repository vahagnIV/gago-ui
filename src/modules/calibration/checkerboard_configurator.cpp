//
// Created by vahagn on 1/16/20.
//


#include "checkerboard_configurator.h"
#include <QLabel>
#include <QSpinBox>

namespace gago {
namespace gui {
namespace configuration {

void CheckerboardConfigurator::DrawConfigurationPage(QWidget *widget) {
  layout_ = new QGridLayout();
  widget->setLayout(layout_);
  layout_->addWidget(new QLabel("Nx:"), 0, 1, 1, 1);
  nx_box_ = new QSpinBox();
  layout_->addWidget(nx_box_, 0, 2, 1, 3);
  nx_box_->setValue(current_params_.n_x);
  layout_->addWidget(new QLabel("Ny:"), 1, 1, 1, 1);
  ny_box_ = new QSpinBox();
  ny_box_->setValue(current_params_.n_y);
  layout_->addWidget(ny_box_, 1, 2, 1, 3);
}

void CheckerboardConfigurator::Apply() {
  current_params_.n_x = nx_box_->value();
  current_params_.n_y = ny_box_->value();
}
/*
void CheckerboardConfigurator::GetConfiguration(nlohmann::json & out_json) {
  out_json["Nx"] = current_params_.n_x;
  out_json["Ny"] = current_params_.n_y;
}

void CheckerboardConfigurator::SetConfiguration(const nlohmann::json & json) {
  if (json.find("Nx") != json.end())
    current_params_.n_x = json["Nx"];
  if (json.find("Ny") != json.end())
    current_params_.n_y = json["Ny"];
}
*/
const QString & CheckerboardConfigurator::ConfigWindowName() const {
  return window_name_;
}
CheckerboardConfigurator::~CheckerboardConfigurator() {

}

}
}
}