//
// Created by vahagn on 1/16/20.
//

#include "calibration_pattern_type.h"

namespace gago {
namespace gui {
namespace configuration {

QString to_string(CalibrationPatternType type) {
  switch (type) {
    case Checkerboard:
      return "Checkerboard";
  }
  return QString();
}

bool try_parse(const QString & type_str, CalibrationPatternType & out_type) {
  if (0 == QString::compare(type_str, "Checkerboard", Qt::CaseInsensitive)) {
    out_type = Checkerboard;
    return true;
  }
  return false;
}

}
}
}