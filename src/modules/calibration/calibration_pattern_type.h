//
// Created by vahagn on 1/16/20.
//

#ifndef GAGO_UI_CALIBRATION_PATTERN_TYPE_H
#define GAGO_UI_CALIBRATION_PATTERN_TYPE_H
#include <QString>

namespace gago {
namespace gui {
namespace configuration {

enum CalibrationPatternType {
  Checkerboard
};

QString to_string(CalibrationPatternType type);

bool try_parse(const QString & type_str, CalibrationPatternType & out_type);



}
}
}

#endif //GAGO_UI_CALIBRATION_PATTERN_TYPE_H
