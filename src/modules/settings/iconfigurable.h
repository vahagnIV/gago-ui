//
// Created by vahagn on 12/30/19.
//

#ifndef GAGO_UI_SRC_MODULES_SETTINGS_I_CONFIGURABLE_H_
#define GAGO_UI_SRC_MODULES_SETTINGS_I_CONFIGURABLE_H_
#include <QSettings>
#include "iconfigurator.h"

namespace gago {
namespace gui {
namespace configuration {

class IConfigurable {
 public:
  virtual IConfigurator *GetConfigurator() = 0;
  virtual void DisposeConfigurator(IConfigurator *) = 0;
  virtual void ApplyConfiguration(QSettings & settings, IConfigurator *) = 0;
  virtual void Configure(QSettings & settings) = 0;
  virtual const QString & GetName() const = 0;
  virtual ~IConfigurable() = default;
 private:
  QSettings settings_;
};

}
}
}

#endif //GAGO_UI_SRC_MODULES_SETTINGS_I_CONFIGURABLE_H_
