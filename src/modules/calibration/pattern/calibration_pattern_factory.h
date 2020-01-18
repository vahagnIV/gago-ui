//
// Created by vahagn on 1/18/20.
//

#ifndef GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_PATTERN_FACTORY_H_
#define GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_PATTERN_FACTORY_H_

#include <memory>
#include "ipattern.h"
#include "modules/settings/iconfigurator.h"
namespace gago {
namespace calibration {
namespace pattern {

class CalibrationPatternFactory {
 public:
  static std::shared_ptr<IPattern> Create(gui::configuration::IConfigurator * configurator);
};

}
}
}

#endif //GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_PATTERN_FACTORY_H_
