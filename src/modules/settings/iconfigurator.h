//
// Created by vahagn on 12/30/19.
//

#ifndef GAGO_UI_SRC_MODULES_SETTINGS_I_CONFIGURATOR_H_
#define GAGO_UI_SRC_MODULES_SETTINGS_I_CONFIGURATOR_H_

#include <QJsonObject>
#include <QWidget>

namespace gago {
namespace gui {
namespace configuration {

class IConfigurator {
 public:
  IConfigurator() = default;
  virtual void DrawConfigurationPage(QWidget * widget) = 0;
  virtual void Apply() = 0;
  /*virtual void GetConfiguration(QJsonObject & out_json) = 0;
  virtual void SetConfiguration(const QJsonObject & json) = 0;*/
  virtual const QString &ConfigWindowName() const = 0;
  virtual ~IConfigurator()= default;
};

}
}
}

#endif //GAGO_UI_SRC_MODULES_SETTINGS_I_CONFIGURATOR_H_
