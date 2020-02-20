//
// Created by vahagn on 12/29/19.
//

#ifndef GAGO_UI_SRC_CONFIG_CONFIGURATION_H_
#define GAGO_UI_SRC_CONFIG_CONFIGURATION_H_

#include <QString>
#include <QDir>

namespace gago {
namespace gui {
namespace configuration {

class Configuration {
 public:
  static Configuration & Instance() {
    static Configuration instance;
    return instance;
  }
  bool Load(const QString & filename);
  QStringList & GetModulePaths();
  QStringList & GetModuleDirs();
  const QDir & GetCacheFolderPath() const;
 private:
  Configuration();
  QStringList module_paths_;
  QStringList module_dirs_;
  QDir cache_folder_;
  static const QString JSON_MODULE_PATHS;
  static const QString JSON_MODULE_DIRS;
};

}
}
}

#endif //GAGO_UI_SRC_CONFIG_CONFIGURATION_H_
