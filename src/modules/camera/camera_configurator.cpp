//
// Created by vahagn on 1/1/20.
//

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>

#include "camera_settings_q_model.h"
#include "camera_configurator.h"

namespace gago {
namespace gui {
namespace configuration {

CameraConfigurator::CameraConfigurator(const std::vector<gago::io::video::CameraDeviceInfo> &devices,
                                       const std::vector<gago::io::video::CameraSettings> &current_settings) : devices_(
    devices), current_settings_(current_settings) {
}

void CameraConfigurator::DrawConfigurationPage(QWidget *widget) {
  QHBoxLayout *main_layout = new QHBoxLayout(widget);
  widget->setLayout(main_layout);

  QVBoxLayout *camera_list_layout = new QVBoxLayout();
  main_layout->addLayout(camera_list_layout);

  QListView *camera_list_view = new QListView();
  camera_list_layout->addWidget(camera_list_view);
  camera_list_view->setMaximumWidth(200);
  CameraSettingsQModel * model = new CameraSettingsQModel();

  camera_list_view->setModel(model);
  for (int i = 0; i < devices_.size(); ++i) {
    model->insertRow(CameraProps{&devices_[i], &current_settings_[i]});
  }

  QHBoxLayout *details_and_settings_layout = new QHBoxLayout();
  main_layout->addLayout(details_and_settings_layout);
  QVBoxLayout *config_layout = new QVBoxLayout();
  details_and_settings_layout->addLayout(config_layout);
  QFrame *settings_frame = new QFrame();
  config_layout->addWidget(settings_frame);
  config_layout->addStretch();

}

void CameraConfigurator::Apply() {

}

void CameraConfigurator::GetConfiguration(nlohmann::json &out_json) {

}

void CameraConfigurator::SetConfiguration(const nlohmann::json &json) {

}

const std::string &CameraConfigurator::ConfigWindowName() const {
  return window_name;
}

}
}
}