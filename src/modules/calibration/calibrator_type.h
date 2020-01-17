//
// Created by vahagn on 1/17/20.
//

#ifndef GAGO_UI_CALIBRATOR_TYPE_H
#define GAGO_UI_CALIBRATOR_TYPE_H

#include <string>

namespace gago {
namespace gui {
namespace configuration {

enum CalibratorType {
  MLE_Calibrator
};

std::string to_string(CalibratorType type);

bool try_parse(const std::string & type_str, CalibratorType & out_type);
}
}
}

#endif //GAGO_UI_CALIBRATOR_TYPE_H
