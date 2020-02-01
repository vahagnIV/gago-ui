#include "main_module.h"
#include <QMenuBar>
#include <QAction>
#include <boost/algorithm/string.hpp>
#include <iostream>

namespace gago {
namespace gui {
namespace modules {

MainModule::MainModule() : IModule("Main", "main"),
                           main_window_(new gago::gui::modules::MainWindow),
                           current_view_index_(-1) {
  QMenu *menu = main_window_.menuBar()->addMenu("File");
  menu->setObjectName("File");

  menu = main_window_.menuBar()->addMenu("Edit");
  menu->setObjectName("Edit");

  menu = main_window_.menuBar()->addMenu("View");
  menu->setObjectName("View");

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

void MainModule::QRequiredModules(std::vector<RequiredModuleParams> & out_required_modules) {
}

void MainModule::SetRequiredModules(const std::vector<IModule *> & modules) {

}

void MainModule::Show() {
  SetCurrentView(0);

  QMenuBar *menu_bar = main_window_.menuBar();
  menu_bar->findChild<QMenu *>("File")->addSeparator();
  QObject::connect(CreateMenuBranch("/File/Quit"), &QAction::triggered, [&]() { main_window_.close(); });
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
      current_menu = root_menu->findChild<QMenu *>(QString::fromStdString(split[j]), Qt::FindDirectChildrenOnly);
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
  QAction * result = root_menu->addAction(QString::fromStdString(split.back()));
  result->setObjectName(QString::fromStdString(path));
  return result;

}

QMainWindow *MainModule::MainWindow() {
  return &main_window_;
}

void MainModule::RegisterView(View *view) {
  views_.push_back(view);
  QAction *start_view = CreateMenuBranch("/View/" + view->GetName());
  start_view->setCheckable(true);
  start_view->setChecked(false);
  int idx = views_.size() - 1;

  QObject::connect(start_view, &QAction::triggered, [this, idx, start_view]() {
    SetCurrentView(idx);
  });
}

void MainModule::SetCurrentView(int idx) {
  View * view = this->views_[idx];
  QAction * new_action = main_window_.menuBar()->findChild<QAction *>(QString::fromStdString("View/" + view->GetName()));
  new_action->setChecked(true);
  if(idx == this->current_view_index_)
    return;
  if (this->current_view_index_ >= 0) {
    QAction *old_action = main_window_.menuBar()->findChild<QAction *>(QString::fromStdString(
        "View/" + views_[current_view_index_]->GetName()));
    views_[current_view_index_]->StopDrawing();
    old_action->setChecked(false);
  }

  this->current_view_index_ = idx;
  this->views_[idx]->StartDrawing(this->main_window_.centralWidget());
}

}
}
}
