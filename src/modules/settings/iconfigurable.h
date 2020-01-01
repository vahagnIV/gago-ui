//
// Created by vahagn on 12/30/19.
//

#ifndef GAGO_UI_SRC_MODULES_SETTINGS_I_CONFIGURABLE_H_
#define GAGO_UI_SRC_MODULES_SETTINGS_I_CONFIGURABLE_H_
#include <string>
#include "iconfigurator.h"

namespace gago {
namespace gui {
namespace configuration {

class IConfigurable {
 public:
  virtual IConfigurator *GetConfigurator() = 0;
  virtual void DisposeConfigurator(IConfigurator *) = 0;
  virtual const std::string &ConfigWindowName() const = 0;
  virtual ~IConfigurable() = default;
};

}
}
}

#endif //GAGO_UI_SRC_MODULES_SETTINGS_I_CONFIGURABLE_H_
