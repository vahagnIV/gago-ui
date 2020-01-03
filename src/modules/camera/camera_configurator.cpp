//
// Created by vahagn on 1/1/20.
//

#include <QListView>
#include <QGroupBox>
#include <QStringListModel>

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

  main_layout_ = new QHBoxLayout(widget);
  widget->setLayout(main_layout_);

  // Left camera list
  QVBoxLayout *camera_list_layout = new QVBoxLayout();
  main_layout_->addLayout(camera_list_layout);

  QListView *camera_list_view = new QListView();
  camera_list_layout->addWidget(camera_list_view);
  camera_list_view->setMaximumWidth(200);
  CameraSettingsQModel *model = new CameraSettingsQModel();

  camera_list_view->setModel(model);
  for (int i = 0; i < devices_.size(); ++i) {
    model->insertRow(CameraProps{&devices_[i], &current_settings_[i]});
  }
  camera_list_view->selectionModel()->select(camera_list_view->model()->index(0, 0), QItemSelectionModel::Select);

  //camera_layouts_.resize(devices_.size());
  for (CameraProps &info: model->GetList()) {
    internal::CameraLayout layout;
    // Right details and settings
    QHBoxLayout *details_and_settings_layout = new QHBoxLayout();

    layout.main_widget = new QWidget();
    layout.main_widget->setLayout(details_and_settings_layout);

    QVBoxLayout *settings_frame_layout = new QVBoxLayout();
    details_and_settings_layout->addLayout(settings_frame_layout);

    // Checkbox
    QGroupBox *settings_group_box = new QGroupBox("Settings");


    QGridLayout *settings_group_box_layout = new QGridLayout();

    settings_group_box->setLayout(settings_group_box_layout);
    layout.enabled_checkbx = new QCheckBox("Enabled");
    layout.enabled_checkbx->setChecked(info.settings->status == io::video::Enabled);
    settings_group_box_layout->addWidget(layout.enabled_checkbx, 0, 0, 1, 3);
    settings_frame_layout->addWidget(settings_group_box);
    settings_frame_layout->addStretch(1);

    QLabel *name_label = new QLabel("Name:");
    layout.name_edit = new QLineEdit();
    layout.name_edit->setText(info.settings->camera_name.c_str());
    settings_group_box_layout->addWidget(name_label,1,0,1,1);
    settings_group_box_layout->addWidget(layout.name_edit,1,1,1,2);

    QLabel *camera_format_label = new QLabel("Format:");
    settings_group_box_layout->addWidget(camera_format_label, 2, 0, 1, 1);
    layout.cam_format_combo = new QComboBox();
    for (int i = 0; i < info.info->formats.size(); ++i) {
      layout.cam_format_combo->insertItem(i, (char *) (info.info->formats[i].description));
    }
    settings_group_box_layout->addWidget(layout.cam_format_combo, 2, 1, 1, 2);
    layout.cam_format_combo->setCurrentIndex(info.settings->format_index);

    QLabel *camera_resolution_label = new QLabel("Resolution:");
    settings_group_box_layout->addWidget(camera_resolution_label, 3, 0, 1, 1);
    layout.cam_resolution_combo = new QComboBox();

    QObject::connect(layout.cam_format_combo, QOverload<int>::of(&QComboBox::activated), [=](int index) {
      layout.cam_resolution_combo->clear();
      for (int i = 0; i < info.info->resolutions[index].size(); ++i) {
        layout.cam_resolution_combo->insertItem(i,
                                                QString::number(info.info->resolutions[index][i].discrete.width)
                                                    + "x"
                                                    + QString::number(info.info->resolutions[index][i].discrete.height));
      }
      layout.cam_resolution_combo->setCurrentIndex(
          info.settings->resolution_index < info.info->resolutions.size() ? info.settings->resolution_index : 0);

    });

    settings_group_box_layout->addWidget(layout.cam_resolution_combo, 3, 1, 1, 2);
    layout.cam_format_combo->setCurrentIndex(info.settings->format_index);
    layout.cam_format_combo->activated(info.settings->format_index);

    // Details
    QVBoxLayout *cam_details_layout = new QVBoxLayout();
    details_and_settings_layout->addLayout(cam_details_layout);

    QGroupBox *details_group_box = new QGroupBox("Details");
    cam_details_layout->addWidget(details_group_box);



    QGridLayout *cam_details_frame_layout = new QGridLayout();
    details_group_box->setLayout(cam_details_frame_layout);

    QLabel *camera_dev_name_label = new QLabel("Device name:");
    cam_details_frame_layout->addWidget(camera_dev_name_label, 0, 0, 1, 1);
    QLabel *cam_dev_name_label = new QLabel(info.info->manufacturer.c_str());
    cam_details_frame_layout->addWidget(cam_dev_name_label, 0, 1, 1, 2);

    QLabel *path_label = new QLabel("Device path:");
    cam_details_frame_layout->addWidget(path_label, 1, 0, 1, 1);
    QLabel *path_name_label = new QLabel(info.info->device_path.c_str());
    cam_details_frame_layout->addWidget(path_name_label, 1, 1, 1, 2);
    cam_details_layout->addStretch(1);

    camera_layouts_.push_back(layout);
    main_layout_->addWidget(layout.main_widget);
    layout.main_widget->hide();
  }

  camera_layouts_[0].main_widget->show();
  camera_list_view->setCurrentIndex(camera_list_view->model()->index(0, 0));
  //main_layout->removeWidget(camera_layouts_[0].main_layout);

  QObject::connect(camera_list_view->selectionModel(),
                   &QItemSelectionModel::selectionChanged,
                   [=](const QItemSelection &selected, const QItemSelection &deselected) {
                     camera_layouts_[deselected.indexes().at(0).row()].main_widget->hide();
                     camera_layouts_[selected.indexes().at(0).row()].main_widget->show();
                   });

}

void CameraConfigurator::Apply() {
  for (int i = 0; i < devices_.size(); ++i) {
    current_settings_[i].resolution_index = camera_layouts_[i].cam_resolution_combo->currentIndex();
    current_settings_[i].format_index = camera_layouts_[i].cam_format_combo->currentIndex();
    current_settings_[i].camera_name = camera_layouts_[i].name_edit->text().toStdString();
    current_settings_[i].status = camera_layouts_[i].enabled_checkbx->checkState() == 0 ? io::video::Disabled : io::video::Enabled;
  }
}

void CameraConfigurator::GetConfiguration(nlohmann::json &out_json) {
  for (int i = 0; i < current_settings_.size(); ++i) {
    nlohmann::json conf;
    conf["name"] = current_settings_[i].camera_name;
    conf["format"] = current_settings_[i].format_index;
    conf["resolution"] = current_settings_[i].resolution_index;
    conf["status"] = current_settings_[i].status == io::video::Disabled ? "Disabled" : "Enabled";
    out_json[devices_[i].device_path] = conf;
  }

}

void CameraConfigurator::SetConfiguration(const nlohmann::json &json) {

}

const std::string &CameraConfigurator::ConfigWindowName() const {
  return window_name;
}

}
}
}