//
// Created by vahagn on 12/29/19.
//

#ifndef GAGO_UI_SRC_CONFIG_CONFIGURATION_H_
#define GAGO_UI_SRC_CONFIG_CONFIGURATION_H_

#include <QString>
#include "external/nlohmann/json.hpp"


namespace gago {
namespace gui {
namespace configuration {

class Configuration {
 public:

  bool Load(const QString & filename);
  void GetModulePaths(QStringList & out_paths);
  void GetModuleDirs(QStringList & out_paths);
 private:
  void GetArray(QStringList & out_array, const QString & json_name);
  nlohmann::json json_;
  static const QString JSON_MODULE_PATHS;
  static const QString JSON_MODULE_DIRS;
};

}
}
}

#endif //GAGO_UI_SRC_CONFIG_CONFIGURATION_H_
