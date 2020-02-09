#include <QApplication>
#include "module_manager.h"
#include <iostream>
#include <config/configuration.h>
#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include "modules/main/main_module.h"
#include "modules/settings/settings_module.h"

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  gago::gui::configuration::Configuration config;
  if(!config.Load("config.json")) {
    std::cout << "Could not load config.json" << std::endl;
    return -1;
  }
  std::vector<std::string> module_paths, module_dirs;
  config.GetModulePaths(module_paths);
  config.GetModuleDirs(module_dirs);

  for(const std::string & module_path: module_paths)
    gago::gui::modules::ModuleManager::Instance().LoadModule(module_path);

  for (const std::string & module_dir: module_dirs) {
    for(auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(module_dir), {})){
      gago::gui::modules::ModuleManager::Instance().LoadModule(entry.path().string());
    }
  }
  gago::gui::modules::MainModule *main_module =
      static_cast<gago::gui::modules::MainModule * >( gago::gui::modules::ModuleManager::Instance().GetModule("main"));
//  main_module->CreateMenuBranch("/File/New/G/");


  gago::gui::modules::ModuleManager::Instance().SatisfyRequirements();

  gago::gui::modules::SettingsModule *settings_module =
      static_cast<gago::gui::modules::SettingsModule * >( gago::gui::modules::ModuleManager::Instance().GetModule("settings"));

  settings_module->Configure();

  gago::gui::modules::ModuleManager::Instance().Start();
  std::cout << "Main module shown " << std::endl;

  return QApplication::exec();
}