#include <QApplication>
#include <QDir>
#include <QDebug>
#include "module_manager.h"
#include "config/configuration.h"
#include "modules/settings/settings_module.h"

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  gago::gui::configuration::Configuration config;
  if (!config.Load("config.json")) {
    qWarning() << "Could not load config.json";
    return -1;
  }
  QStringList module_paths, module_dirs;
  config.GetModulePaths(module_paths);
  config.GetModuleDirs(module_dirs);

  for (const QString & module_path: module_paths)
    gago::gui::modules::ModuleManager::Instance().LoadModule(module_path);

  for (const QDir module_dir: module_dirs) {
    for (QString & module_name : module_dir.entryList()) {
      gago::gui::modules::ModuleManager::Instance().LoadModule(module_dir.filePath(module_name));
    }
  }

  gago::gui::modules::ModuleManager::Instance().SatisfyRequirements();

  static_cast<gago::gui::modules::SettingsModule * >( gago::gui::modules::ModuleManager::Instance().GetModule(
      "settings"))->Configure();

  gago::gui::modules::ModuleManager::Instance().Start();

  return QApplication::exec();
}