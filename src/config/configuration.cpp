//
// Created by vahagn on 12/29/19.
//

#include "configuration.h"
#include <boost/filesystem.hpp>
#include <fstream>

namespace gago {
namespace gui {
namespace configuration {

const std::string Configuration::JSON_MODULE_PATHS = "ModulePaths";
const std::string Configuration::JSON_MODULE_DIRS = "ModuleDirs";

bool Configuration::Load(const std::string &filename) {
  if (!boost::filesystem::exists(filename))
    return false;
  std::ifstream file(filename);
  json_ = nlohmann::json::parse(file);
  return true;
}

void Configuration::GetModulePaths(std::vector<std::string> &out_paths) {
  GetArray(out_paths, JSON_MODULE_PATHS);
}
void Configuration::GetModuleDirs(std::vector<std::string> &out_paths) {
  GetArray(out_paths, JSON_MODULE_DIRS);
}

void Configuration::GetArray(std::vector<std::string> &out_array, const std::string &json_name) {
  if (json_.find(json_name) != json_.end())
    for (auto &json_p: json_[json_name]) {
      out_array.push_back(json_p);
    }
}

}
}
}