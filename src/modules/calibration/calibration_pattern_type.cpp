//
// Created by vahagn on 1/16/20.
//

#include "calibration_pattern_type.h"
#include <boost/algorithm/string.hpp>

namespace gago {
namespace gui {
namespace configuration {

std::string to_string(CalibrationPatternType type) {
  switch (type) {
    case Checkerboard:
      return "Checkerboard";
  }
  return std::__cxx11::string();
}

bool try_parse(const std::string & type_str, CalibrationPatternType & out_type) {
  if (boost::algorithm::iequals(type_str, "Checkerboard")) {
    out_type = Checkerboard;
    return true;
  }
  return false;
}

}
}
}