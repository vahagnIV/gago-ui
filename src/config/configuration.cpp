//
// Created by vahagn on 12/29/19.
//

#include <QFile>
#include "configuration.h"
#include <fstream>

namespace gago {
namespace gui {
namespace configuration {

const QString Configuration::JSON_MODULE_PATHS = "ModulePaths";
const QString Configuration::JSON_MODULE_DIRS = "ModuleDirs";

bool Configuration::Load(const QString &filename) {
  if (!QFile::exists(filename))
    return false;
  std::ifstream file(filename.toStdString());
  json_ = nlohmann::json::parse(file);
  return true;
}

void Configuration::GetModulePaths(QStringList &out_paths) {
  GetArray(out_paths, JSON_MODULE_PATHS);
}
void Configuration::GetModuleDirs(QStringList &out_paths) {
  GetArray(out_paths, JSON_MODULE_DIRS);
}

void Configuration::GetArray(QStringList &out_array, const QString &json_name) {
  if (json_.find(json_name.toStdString()) != json_.end())
    for (auto &json_p: json_[json_name.toStdString()]) {
      out_array.push_back(QString::fromStdString(json_p));
    }
}

}
}
}