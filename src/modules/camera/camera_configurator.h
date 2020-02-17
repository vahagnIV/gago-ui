//
// Created by vahagn on 1/1/20.
//

#ifndef GAGO_UI_SRC_MODULES_CAMERA_CAMERA_CONFIGURATOR_H_
#define GAGO_UI_SRC_MODULES_CAMERA_CAMERA_CONFIGURATOR_H_

#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QWidget>
#include <QListView>

#include <io/video/camera_settings.h>
#include "modules/settings/iconfigurator.h"

namespace gago {
namespace gui {
namespace configuration {

namespace internal {
struct CameraLayout {
  QWidget *main_widget;
  QCheckBox *enabled_checkbx;
  QLineEdit *name_edit;
  QComboBox *cam_format_combo;
  QComboBox *cam_resolution_combo;
  QLabel *cam_dev_name_label;
  QLabel *path_name_label;
};
}

class CameraConfigurator : public IConfigurator {
 public:
  CameraConfigurator(const std::vector<gago::io::video::CameraSettings> &devices);

  //IConfigurator
  void DrawConfigurationPage(QWidget *widget) override;
  void Apply() override;
  const QString &ConfigWindowName() const override;

  //
  const std::vector<io::video::CameraSettings> &GetSettings() const {
    return current_settings_;
  }
 private:
  void InitControlElements();
  void DrawOnWidget(QWidget * widget);
  QString window_name = "Camera";
  const std::vector<gago::io::video::CameraSettings> devices_;
  std::vector<io::video::CameraSettings> current_settings_;
  std::vector<internal::CameraLayout> camera_layouts_;
  QListView * camera_list_view;
  QHBoxLayout *main_layout_;

};

}
}
}

#endif //GAGO_UI_SRC_MODULES_CAMERA_CAMERA_CONFIGURATOR_H_
