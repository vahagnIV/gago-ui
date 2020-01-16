//
// Created by vahagn on 1/16/20.
//

#ifndef GAGO_UI_CALIB_PATTERN_CONFIGURATOR_FACTORY_H
#define GAGO_UI_CALIB_PATTERN_CONFIGURATOR_FACTORY_H
#include "modules/settings/iconfigurator.h"
#include "calibration_pattern_type.h"

namespace gago {
namespace gui {
namespace configuration {

class CalibPatternConfiguratorFactory {
 public:
  IConfigurator *CreateConfigurator(CalibrationPatternType type);
  void DisposeConfigurator(IConfigurator *configurator);
 private:
  nlohmann::json calib_pattern_setting_json_;
};

}
}
}
#endif //GAGO_UI_CALIB_PATTERN_CONFIGURATOR_FACTORY_H
