//
// Created by vahagn on 1/17/20.
//

#ifndef GAGO_UI_CALIBRATOR_TYPE_H
#define GAGO_UI_CALIBRATOR_TYPE_H

#include <QString>

namespace gago {
namespace gui {
namespace configuration {

enum CalibratorType {
  MLE_Calibrator
};

QString to_string(CalibratorType type);

bool try_parse(const QString & type_str, CalibratorType & out_type);
}
}
}

#endif //GAGO_UI_CALIBRATOR_TYPE_H
