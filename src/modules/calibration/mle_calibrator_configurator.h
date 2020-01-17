//
// Created by vahagn on 1/17/20.
//

#ifndef GAGO_UI_MLE_CALIBRATOR_CONFIGURATOR_H
#define GAGO_UI_MLE_CALIBRATOR_CONFIGURATOR_H

#include "modules/settings/iconfigurator.h"
#include <QCheckBox>

namespace gago {
namespace gui {
namespace configuration {

struct MLEConfigurationSettings{
  bool calibrate_camera_first;
};

class MLECalibratorConfigurator : public IConfigurator {
 public:
  MLECalibratorConfigurator();
  void DrawConfigurationPage(QWidget *widget) override;
  void Apply() override;
  void GetConfiguration(nlohmann::json & out_json) override;
  void SetConfiguration(const nlohmann::json & json) override;
  const std::string & ConfigWindowName() const override;
 protected:
  MLEConfigurationSettings current_settings_;

  QCheckBox * camera_first_chkbx_;

  const std::string window_name_;

};

}}}
#endif //GAGO_UI_MLE_CALIBRATOR_H
