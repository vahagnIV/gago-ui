//
// Created by vahagn on 1/16/20.
//

#include "calib_pattern_configurator_factory.h"
#include "checkerboard_configurator.h"

namespace gago {
namespace gui {
namespace configuration {

IConfigurator *CalibPatternConfiguratorFactory::CreateConfigurator(CalibrationPatternType type) {
  switch (type) {
    case CalibrationPatternType::Checkerboard:
      return (IConfigurator *) new CheckerboardConfigurator();
  }
}

void CalibPatternConfiguratorFactory::DisposeConfigurator(IConfigurator *configurator) {
  CalibrationPatternType type;
  if (try_parse(configurator->ConfigWindowName(), type)) {
    switch (type) {
      case CalibrationPatternType::Checkerboard: {
        CheckerboardConfigurator *cfg = (CheckerboardConfigurator *) configurator;
        delete cfg;
        break;
      }
    }
  }
}


}
}
}