//
// Created by vahagn on 1/1/20.
//

#ifndef GAGO_UI_SRC_MODULES_CAMERA_CAMERA_CONFIGURATOR_H_
#define GAGO_UI_SRC_MODULES_CAMERA_CAMERA_CONFIGURATOR_H_

#include <modules/settings/iconfigurator.h>
#include <io/video/camera.h>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QWidget>

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
  CameraConfigurator(const std::vector<io::video::CameraDeviceInfo> &devices,
                     const std::vector<io::video::CameraSettings> &current_settings);

  //IConfigurator
  void DrawConfigurationPage(QWidget *widget) override;
  void Apply() override;
  void GetConfiguration(nlohmann::json &out_json) override;
  void SetConfiguration(const nlohmann::json &json) override;
  const std::string &ConfigWindowName() const override;

  //
  const std::vector<io::video::CameraSettings> &GetSettings() const {
    return current_settings_;
  }
 private:
  void InitControlElements();
  void DrawOnWidget(QWidget * widget);
  std::string window_name = "Camera";
  const std::vector<io::video::CameraDeviceInfo> devices_;
  std::vector<io::video::CameraSettings> current_settings_;
  std::vector<internal::CameraLayout> camera_layouts_;
  QHBoxLayout *main_layout_;

};

}
}
}

#endif //GAGO_UI_SRC_MODULES_CAMERA_CAMERA_CONFIGURATOR_H_
