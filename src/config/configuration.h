//
// Created by vahagn on 12/29/19.
//

#ifndef GAGO_UI_SRC_CONFIG_CONFIGURATION_H_
#define GAGO_UI_SRC_CONFIG_CONFIGURATION_H_

#include <QString>


namespace gago {
namespace gui {
namespace configuration {

class Configuration {
 public:

  bool Load(const QString & filename);
  QStringList & GetModulePaths();
  QStringList & GetModuleDirs();
 private:
  QStringList  module_paths_;
  QStringList  module_dirs_;
  static const QString JSON_MODULE_PATHS;
  static const QString JSON_MODULE_DIRS;
};

}
}
}

#endif //GAGO_UI_SRC_CONFIG_CONFIGURATION_H_
