#include <QApplication>
#include "module_manager.h"
#include <iostream>
#include <config/configuration.h>
#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include "modules/main/main_module.h"

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

  /*sterio::gui::settings::SettingsModule *settings_module =
      static_cast<sterio::gui::settings::SettingsModule * >( sterio::gui::ModuleManager::Instance().GetModule("Settings"));

  settings_module->Configure();
  std::cout << "Configuring done" << std::endl;

   */
//  main_module->CreateMenuBranch("/File/Close");

  main_module->Show();
  std::cout << "Main module shown " << std::endl;

  return QApplication::exec();
}