//
// Created by vahagn on 12/29/19.
//

#include <QFile>
#include "configuration.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QStandardPaths>
namespace gago {
namespace gui {
namespace configuration {

const QString Configuration::JSON_MODULE_PATHS = "ModulePaths";
const QString Configuration::JSON_MODULE_DIRS = "ModuleDirs";

Configuration::Configuration() {
  QStringList l = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);

  // TODO: implement cross platform
  QDir home_dir = l.isEmpty() ? QDir("/tmp") : QDir(l[0]);
  if (!home_dir.exists(".gago"))
    home_dir.mkdir(".gago");
  cache_folder_ = QDir(home_dir.filePath(".gago"));

}

const QDir & Configuration::GetCacheFolderPath() const {
  return cache_folder_;
}


bool Configuration::Load(const QString & filename) {
  if (!QFile::exists(filename))
    return false;
  QFile json_file(filename);
  json_file.open(QFile::ReadOnly);

  QJsonDocument document = QJsonDocument::fromJson(json_file.readAll());
  if (document.object().contains(JSON_MODULE_PATHS)) {
    for (QVariant & var: document.object()[JSON_MODULE_PATHS].toArray().toVariantList())
      module_paths_.append(var.toString());
  }

  if (document.object().contains(JSON_MODULE_DIRS)) {
    for (QVariant & var: document.object()[JSON_MODULE_DIRS].toArray().toVariantList())
      module_dirs_.append(var.toString());
  }
  return true;
}

QStringList & Configuration::GetModulePaths() {
  return module_paths_;
}

QStringList & Configuration::GetModuleDirs() {
  return module_dirs_;
}

}
}
}