//
// Created by vahagn on 1/1/20.
//

#ifndef GAGO_UI_SRC_MODULES_CAMERA_CAMERA_CONFIGURATOR_H_
#define GAGO_UI_SRC_MODULES_CAMERA_CAMERA_CONFIGURATOR_H_

#include <modules/settings/iconfigurator.h>
#include <io/video/camera.h>

namespace gago {
namespace gui {
namespace configuration {

class CameraConfigurator : public IConfigurator {
 public:
  CameraConfigurator(const std::vector<io::video::CameraDeviceInfo> &devices,
                     const std::vector<io::video::CameraSettings> &current_settings);
  void DrawConfigurationPage(QWidget *widget) override;
  void Apply() override;
  void GetConfiguration(nlohmann::json &out_json) override;
  void SetConfiguration(const nlohmann::json &json) override;
  const std::string &ConfigWindowName() const override;
 private:
  std::string window_name = "Camera";
  const std::vector<io::video::CameraDeviceInfo> devices_;
  std::vector<io::video::CameraSettings>  current_settings_;

};

}
}
}

#endif //GAGO_UI_SRC_MODULES_CAMERA_CAMERA_CONFIGURATOR_H_
