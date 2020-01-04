//
// Created by vahagn on 12/30/19.
//

#ifndef GAGO_UI_SRC_MODULES_SETTINGS_I_CONFIGURATOR_H_
#define GAGO_UI_SRC_MODULES_SETTINGS_I_CONFIGURATOR_H_

#include <nlohmann/json.hpp>
#include <QWidget>

namespace gago {
namespace gui {
namespace configuration {

class IConfigurator {
 public:
  IConfigurator() = default;
  virtual void DrawConfigurationPage(QWidget * widget) = 0;
  virtual void Apply() = 0;
  virtual void GetConfiguration(nlohmann::json & out_json) = 0;
  virtual void SetConfiguration(const nlohmann::json & json) = 0;
  virtual const std::string &ConfigWindowName() const = 0;
  virtual ~IConfigurator()= default;
};

}
}
}

#endif //GAGO_UI_SRC_MODULES_SETTINGS_I_CONFIGURATOR_H_