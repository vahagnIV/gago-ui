#include "main_module.h"
#include <QMenuBar>
#include <boost/algorithm/string.hpp>
#include <iostream>

namespace gago {
namespace gui {
namespace modules {

MainModule::MainModule() : IModule("Main", "main") {
}

ModuleInitializationResult MainModule::Initalize() {
  return ModuleInitializationResult::OK;
}

unsigned int MainModule::MajorVersion() const {
  return 1;
}

unsigned int MainModule::MinorVersion() const {
  return 0;
}

void MainModule::QRequiredModules(std::vector<RequiredModuleParams> &out_required_modules) {
}

void MainModule::SetRequiredModules(const std::vector<IModule *> &modules) {

}

void MainModule::Show() {
  main_window_.show();
}

QAction *MainModule::CreateMenuBranch(std::string path) {
  boost::trim_if(path, boost::is_any_of("/ "));
  std::vector<std::string> split;
  boost::algorithm::split(split, path, boost::algorithm::is_from_range('/', '/'));
  QMenuBar *menu_bar = main_window_.menuBar();

  QMenu *root_menu, *current_menu;
  if (split.size() > 1) {
    root_menu = menu_bar->findChild<QMenu *>(QString::fromStdString(split[0]), Qt::FindDirectChildrenOnly);
    if (nullptr == root_menu) {
      root_menu = menu_bar->addMenu(QString::fromStdString(split[0]));
      root_menu->setObjectName(QString::fromStdString(split[0]));
    }
    for (int j = 1; j < split.size() - 1; ++j) {
      current_menu = root_menu->findChild<QMenu *>(QString::fromStdString(split[0]), Qt::FindDirectChildrenOnly);
      if (nullptr == current_menu) {
        root_menu = root_menu->addMenu(QString::fromStdString(split[j]));
        root_menu->setObjectName(QString::fromStdString(split[j]));
      } else
        root_menu = current_menu;
    }
  } else {
    // TODO: log error
    return nullptr;
  }
  return root_menu->addAction(QString::fromStdString(split.back()));

}

QMainWindow *MainModule::MainWindow()  {
  return &main_window_;
}

}
}
}
