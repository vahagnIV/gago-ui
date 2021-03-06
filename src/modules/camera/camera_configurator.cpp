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

CameraConfigurator::CameraConfigurator(const std::vector<gago::io::video::CameraSettings> & devices)
    : devices_(devices), current_settings_(devices) {
}

void CameraConfigurator::DrawConfigurationPage(QWidget *widget) {
  InitControlElements();
  DrawOnWidget(widget);
}

void CameraConfigurator::Apply() {
  for (int i = 0; i < devices_.size(); ++i) {
    current_settings_[i].config.resolution_index = camera_layouts_[i].cam_resolution_combo->currentIndex();
    current_settings_[i].config.format_index = camera_layouts_[i].cam_format_combo->currentIndex();
    current_settings_[i].config.name = camera_layouts_[i].name_edit->text().toStdString();
    current_settings_[i].config.vertical_flip = camera_layouts_[i].vflip_checkbx->isChecked();
    current_settings_[i].config.status =
        camera_layouts_[i].enabled_checkbx->checkState() == 0 ? io::video::Disabled : io::video::Enabled;
  }
  camera_list_view->setFocus();
}

const QString & CameraConfigurator::ConfigWindowName() const {
  return window_name;
}

void CameraConfigurator::InitControlElements() {
  for (int i = 0; i < devices_.size(); ++i) {
    const io::video::CameraSettings & device_info = devices_[i];

    internal::CameraLayout layout;

    layout.main_widget = new QWidget();

    // Create elements
    layout.enabled_checkbx = new QCheckBox("Enabled");
    layout.vflip_checkbx = new QCheckBox("Vertical Flip");
    layout.name_edit = new QLineEdit();
    layout.cam_format_combo = new QComboBox();
    layout.cam_resolution_combo = new QComboBox();
    layout.cam_dev_name_label = new QLabel(device_info.camera->GetHardwareDetails().c_str());
    layout.path_name_label = new QLabel(device_info.camera->GetUniqueId().c_str());


    // ====== Set up initial values and bindings ====

    // Name
    layout.name_edit->setText(device_info.camera->GetName().c_str());

    //Format
    for (int i = 0; i < device_info.camera->GetFormats().size(); ++i) {
      layout.cam_format_combo->insertItem(i, (char *) (device_info.camera->GetFormats()[i].c_str()));
    }
    layout.cam_format_combo->setCurrentIndex(device_info.config.format_index);

    // Resolution
    QObject::connect(layout.cam_format_combo, QOverload<int>::of(&QComboBox::activated), [=](int index) {
      layout.cam_resolution_combo->clear();
      for (int i = 0; i < device_info.camera->GetResolutions()[index].size(); ++i) {
        layout.cam_resolution_combo->insertItem(i,
                                                QString::number(device_info.camera->GetResolutions()[index][i].width)
                                                    + "x"
                                                    + QString::number(device_info.camera->GetResolutions()[index][i].height));
      }
      layout.cam_resolution_combo->setCurrentIndex(
          device_info.config.resolution_index
              < device_info.camera->GetResolutions()[device_info.config.format_index].size()
          ? device_info.config.resolution_index : 0);

    });

    layout.cam_format_combo->setCurrentIndex(device_info.config.format_index);
    layout.cam_format_combo->activated(device_info.config.format_index);
    // Manufacturer
    //Path

    QObject::connect(layout.enabled_checkbx, &QCheckBox::stateChanged, [=](int state) {
      layout.name_edit->setEnabled(state);
      layout.cam_format_combo->setEnabled(state);
      layout.cam_resolution_combo->setEnabled(state);

    });

    layout.enabled_checkbx->setChecked(io::video::Enabled == device_info.config.status);
    layout.enabled_checkbx->stateChanged(io::video::Enabled == device_info.config.status);
    layout.vflip_checkbx->setChecked(device_info.config.vertical_flip);

    camera_layouts_.push_back(layout);
  }

}

void CameraConfigurator::DrawOnWidget(QWidget *widget) {
  main_layout_ = new QHBoxLayout(widget);
  widget->setLayout(main_layout_);

  // Left camera list
  QVBoxLayout *camera_list_layout = new QVBoxLayout();
  main_layout_->addLayout(camera_list_layout);

  camera_list_view = new QListView();
  camera_list_layout->addWidget(camera_list_view);
  camera_list_view->setMaximumWidth(200);
  CameraSettingsQModel *model = new CameraSettingsQModel();

  camera_list_view->setModel(model);
  for (int i = 0; i < devices_.size(); ++i) {
    model->insertRow(&current_settings_[i]);
  }
  camera_list_view->selectionModel()->select(camera_list_view->model()->index(0, 0), QItemSelectionModel::Select);
  camera_list_view->setCurrentIndex(camera_list_view->model()->index(0, 0));

  QObject::connect(camera_list_view->selectionModel(),
                   &QItemSelectionModel::selectionChanged,
                   [=](const QItemSelection & selected, const QItemSelection & deselected) {
                     camera_layouts_[deselected.indexes().at(0).row()].main_widget->hide();
                     camera_layouts_[selected.indexes().at(0).row()].main_widget->show();
                   });

  for (internal::CameraLayout & layout: camera_layouts_) {

    // Right details and settings
    QHBoxLayout *details_and_settings_layout = new QHBoxLayout();

    layout.main_widget->setLayout(details_and_settings_layout);

    QVBoxLayout *settings_frame_layout = new QVBoxLayout();
    details_and_settings_layout->addLayout(settings_frame_layout);

    // Checkbox
    QGroupBox *settings_group_box = new QGroupBox("Settings");

    QGridLayout *settings_group_box_layout = new QGridLayout();

    settings_group_box->setLayout(settings_group_box_layout);

    settings_group_box_layout->addWidget(layout.enabled_checkbx, 0, 0, 1, 3);
    settings_frame_layout->addWidget(settings_group_box);
    settings_frame_layout->addStretch(1);

    //Name
    QLabel *name_label = new QLabel("Name:");

    settings_group_box_layout->addWidget(name_label, 1, 0, 1, 1);
    settings_group_box_layout->addWidget(layout.name_edit, 1, 1, 1, 2);

    // Format
    QLabel *camera_format_label = new QLabel("Format:");
    settings_group_box_layout->addWidget(camera_format_label, 2, 0, 1, 1);
    settings_group_box_layout->addWidget(layout.cam_format_combo, 2, 1, 1, 2);

    QLabel *camera_resolution_label = new QLabel("Resolution:");
    settings_group_box_layout->addWidget(camera_resolution_label, 3, 0, 1, 1);

    settings_group_box_layout->addWidget(layout.cam_resolution_combo, 3, 1, 1, 2);

    settings_group_box_layout->addWidget(layout.vflip_checkbx, 4, 0, 1, 2);


    // Details
    QVBoxLayout *cam_details_layout = new QVBoxLayout();
    details_and_settings_layout->addLayout(cam_details_layout);

    QGroupBox *details_group_box = new QGroupBox("Details");
    cam_details_layout->addWidget(details_group_box);

    QGridLayout *cam_details_frame_layout = new QGridLayout();
    details_group_box->setLayout(cam_details_frame_layout);

    QLabel *camera_dev_name_label = new QLabel("Device name:");
    cam_details_frame_layout->addWidget(camera_dev_name_label, 0, 0, 1, 1);

    cam_details_frame_layout->addWidget(layout.cam_dev_name_label, 0, 1, 1, 2);

    QLabel *path_label = new QLabel("Device path:");
    cam_details_frame_layout->addWidget(path_label, 1, 0, 1, 1);

    cam_details_frame_layout->addWidget(layout.path_name_label, 1, 1, 1, 2);
    cam_details_layout->addStretch(1);

    main_layout_->addWidget(layout.main_widget);
    layout.main_widget->hide();
  }

  camera_layouts_[0].main_widget->show();

}

}
}
}