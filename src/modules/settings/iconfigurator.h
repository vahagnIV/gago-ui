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
  virtual void DrawConfigurationPage(QWidget * widget) = 0;
  virtual void ApplyConfigurationAndGet(nlohmann::json & out_json) = 0;
  virtual void Configure(const nlohmann::json & json) = 0;
  virtual ~IConfigurator()= default;
};

}
}
}

#endif //GAGO_UI_SRC_MODULES_SETTINGS_I_CONFIGURATOR_H_
