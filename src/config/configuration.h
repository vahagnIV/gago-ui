//
// Created by vahagn on 12/29/19.
//

#ifndef GAGO_UI_SRC_CONFIG_CONFIGURATION_H_
#define GAGO_UI_SRC_CONFIG_CONFIGURATION_H_
#include "external/nlohmann/json.hpp"


namespace gago {
namespace gui {
namespace configuration {

class Configuration {
 public:

  bool Load(const std::string & filename);
  void GetModulePaths(std::vector<std::string> & out_paths);
  void GetModuleDirs(std::vector<std::string> & out_paths);
 private:
  void GetArray(std::vector<std::string> & out_array, const std::string & json_name);
  nlohmann::json json_;
  static const std::string JSON_MODULE_PATHS;
  static const std::string JSON_MODULE_DIRS;
};

}
}
}

#endif //GAGO_UI_SRC_CONFIG_CONFIGURATION_H_
