//
// Created by vahagn on 1/17/20.
//

#include "calibrator_type.h"

namespace gago {
namespace gui {
namespace configuration {

QString to_string(CalibratorType type) {
  switch (type) {
    case CalibratorType::MLE_Calibrator:
      return "MLE Calibrator";
  }
  return QString();
}

bool try_parse(const QString & type_str, CalibratorType & out_type) {
  if (0 == QString::compare("MLE Calibrator", type_str, Qt::CaseInsensitive)) {
    out_type = CalibratorType::MLE_Calibrator;
    return true;
  }
  return false;
}
}
}
}
