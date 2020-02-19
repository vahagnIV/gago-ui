//
// Created by vahagn on 12/29/19.
//

#include <QFile>
#include "configuration.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>

namespace gago {
namespace gui {
namespace configuration {

const QString Configuration::JSON_MODULE_PATHS = "ModulePaths";
const QString Configuration::JSON_MODULE_DIRS = "ModuleDirs";

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