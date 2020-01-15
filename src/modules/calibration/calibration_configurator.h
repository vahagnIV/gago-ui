//
// Created by vahagn on 1/15/20.
//

#ifndef GAGO_UI_CALIBRATION_CONFIGURATOR_H
#define GAGO_UI_CALIBRATION_CONFIGURATOR_H
#include "modules/settings/iconfigurator.h"

namespace gago {
namespace gui {
namespace configuration {

class CalibrationConfigurator: public IConfigurator {
 public:
  void DrawConfigurationPage(QWidget *widget) override;
  void Apply() override;
  void GetConfiguration(nlohmann::json & out_json) override;
  void SetConfiguration(const nlohmann::json & json) override;
  const std::string & ConfigWindowName() const override;
 private:
  std::string window_name = "Calibration";

};

}}}
#endif //GAGO_UI_CALIBRATION_CONFIGURATOR_H
