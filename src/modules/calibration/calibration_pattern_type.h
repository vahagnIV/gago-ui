//
// Created by vahagn on 1/16/20.
//

#ifndef GAGO_UI_CALIBRATION_PATTERN_TYPE_H
#define GAGO_UI_CALIBRATION_PATTERN_TYPE_H
#include <string>

namespace gago {
namespace gui {
namespace configuration {

enum CalibrationPatternType {
  Checkerboard
};

std::string to_string(CalibrationPatternType type);

bool try_parse(const std::string & type_str, CalibrationPatternType & out_type);



}
}
}

#endif //GAGO_UI_CALIBRATION_PATTERN_TYPE_H
