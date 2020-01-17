//
// Created by vahagn on 1/17/20.
//

#ifndef GAGO_UI_CALIBRATOR_CONFIGURATOR_FACTORY_H
#define GAGO_UI_CALIBRATOR_CONFIGURATOR_FACTORY_H

#include "calibrator_type.h"
#include "modules/settings/iconfigurator.h"

namespace gago {
namespace gui {
namespace configuration {

class CalibratorConfiguratorFactory {
 public:
  static IConfigurator * Create(CalibratorType type);
  static void DisposeConfigurator(IConfigurator * configurator);
};

}
}
}

#endif //GAGO_UI_CALIBRATOR_CONFIGURATOR_FACTORY_H
