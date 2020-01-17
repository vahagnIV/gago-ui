//
// Created by vahagn on 1/17/20.
//

#include "calibrator_configurator_factory.h"
#include "mle_calibrator_configurator.h"
namespace gago {
namespace gui {
namespace configuration {

IConfigurator *CalibratorConfiguratorFactory::Create(CalibratorType type) {
  switch (type) {
    case CalibratorType::MLE_Calibrator:
      return new MLECalibratorConfigurator();
  }
  return nullptr;
}

void CalibratorConfiguratorFactory::DisposeConfigurator(IConfigurator *configurator) {
  CalibratorType type;
  if (try_parse(configurator->ConfigWindowName(), type)) {
    switch (type) {
      case CalibratorType::MLE_Calibrator:
        delete (MLECalibratorConfigurator *) configurator;
    }
  }
}

}
}
}