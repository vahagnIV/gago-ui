//
// Created by vahagn on 1/18/20.
//

#include <calibration_pattern_type.h>
#include "calibration_pattern_factory.h"
#include "checkerboard_pattern.h"
#include "../checkerboard_configurator.h"

namespace gago {
using namespace gui::configuration;
namespace calibration {
namespace pattern {

std::shared_ptr<IPattern> CalibrationPatternFactory::Create(gui::configuration::IConfigurator *configurator) {
  CalibrationPatternType type;
  if (try_parse(configurator->ConfigWindowName(), type)) {
    switch (type) {
      case CalibrationPatternType::Checkerboard:
        CheckerboardConfigurator *cnf = (CheckerboardConfigurator *) configurator;
        return std::make_shared<CheckerboardPattern>(cnf->GetSettings().n_x, cnf->GetSettings().n_y);
    }
  }
  return nullptr;
}
}
}
}