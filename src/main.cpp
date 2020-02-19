#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QIcon>
#include <QJsonDocument>

#include "module_manager.h"
#include "config/configuration.h"

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  a.setWindowIcon(QIcon(":/icons/icon-large.png"));
  gago::gui::configuration::Configuration config;
  if (!config.Load("config.json")) {
    qWarning() << "Could not load config.json";
    QDir dir(GagoUI_MODULE_PATH);
    for (const QString & module_path: dir.entryList({"*.so"})) {
      gago::gui::modules::ModuleManager::Instance().LoadModule(dir.filePath(module_path));
    }
  } else {
    QStringList module_paths, module_dirs;
    module_paths = config.GetModulePaths();
    module_dirs = config.GetModuleDirs();

    for (const QString & module_path: module_paths)
      gago::gui::modules::ModuleManager::Instance().LoadModule(module_path);

    for (const QDir module_dir: module_dirs) {
      for (QString & module_name : module_dir.entryList()) {
        gago::gui::modules::ModuleManager::Instance().LoadModule(module_dir.filePath(module_name));
      }
    }
  }

  gago::gui::modules::ModuleManager::Instance().SatisfyRequirements();

  gago::gui::modules::ModuleManager::Instance().Start();

  return QApplication::exec();
}