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
  static IConfigurator *CreateConfigurator(CalibrationPatternType type);
  static void DisposeConfigurator(IConfigurator *configurator);
};

}
}
}
#endif //GAGO_UI_CALIB_PATTERN_CONFIGURATOR_FACTORY_H
