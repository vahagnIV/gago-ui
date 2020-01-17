//
// Created by vahagn on 1/17/20.
//

#include "calibrator_type.h"
#include <boost/algorithm/string.hpp>
namespace gago {
namespace gui {
namespace configuration {

std::string to_string(CalibratorType type) {
  switch (type) {
    case CalibratorType::MLE_Calibrator:
      return "MLE Calibrator";
  }
  return std::__cxx11::string();
}

bool try_parse(const std::string & type_str, CalibratorType & out_type) {
  if (boost::algorithm::iequals("MLE Calibrator", type_str)) {
    out_type = CalibratorType::MLE_Calibrator;
    return true;
  }
  return false;
}
}
}
}
