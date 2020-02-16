#include "main_module.h"
#include <QMenuBar>
#include <QAction>
#include <QDebug>
#include "ui_main_window.h"

namespace gago {
namespace gui {
namespace modules {

MainModule::MainModule() : QMainWindow(nullptr),
                           IModule("Main", "main"),
                           ui(new Ui::MainWindow),
                           menus_(nullptr),
                           current_view_index_(-1) {
  ui->setupUi(this);

  GetMenu({QString("File")}, 1);
  GetMenu({"Edit"}, 1);
  GetMenu({"View"}, 1);
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

void MainModule::GetRequiredModules(QList<RequiredModuleParams> &out_required_modules) {
}

void MainModule::SetRequiredModules(const QList<IModule *> &modules) {

}

QAction *MainModule::CreateMenuBranch(const QString &path) {

  QStringList split = path.split('/', QString::SkipEmptyParts);
  MenuTreeNode *node = GetMenu(split, split.size() - 1);

  QAction *result = node->Menu()->addAction(split.back());
  node->InsertAction(split.back(), result);
  return result;

}

QMainWindow *MainModule::MainWindow() {
  return (QMainWindow *) this;
}

void MainModule::RegisterView(View *view) {
  views_.push_back(view);
  QAction *start_view = CreateMenuBranch("/View/" + view->GetViewName());
  MenuTreeNode * node = GetMenu("View");
  start_view->setCheckable(true);
  start_view->setChecked(false);
  int idx = views_.size() - 1;
  node->InsertAction(view->GetViewName(), start_view);

  QObject::connect(start_view, &QAction::triggered, [this, idx, start_view]() {
    SetCurrentView(idx);
  });
}

void MainModule::SetCurrentView(int idx) {
  if (views_.empty())
    return;
  if (idx == this->current_view_index_)
    return;

  MenuTreeNode *node = GetMenu("View");
  if (-1 != this->current_view_index_) {
    View *current_view = this->views_[idx];
    current_view->StopDrawing();
    node->GetAction(current_view->GetViewName())->setChecked(false);
  }

  View *view = this->views_[idx];
  node->GetAction(view->GetViewName())->setChecked(true);
  view->StartDrawing(this->centralWidget());
  this->current_view_index_ = idx;
}

int MainModule::GetWeight() const {
  return 1;
}
void MainModule::Start() {
  GetMenu("File")->Menu()->addSeparator();
  QAction *quit = CreateMenuBranch("/File/Quit");
  quit->setIcon(QIcon::fromTheme("application-exit"));

  QObject::connect(quit, &QAction::triggered, [&]() { close(); });
  SetCurrentView(0);

  show();
}

MainModule::~MainModule() {
  if(current_view_index_!=-1)
    views_[current_view_index_]->StopDrawing();
  delete ui;

}

MenuTreeNode *MainModule::GetMenu(const QStringList &path, int count) {
  MenuTreeNode *current_node = &menus_;

  for (int i = 0; i < std::min(count, path.size()); ++i) {
    //Create if not exists
    qInfo() << path[i];
    bool contains = current_node->Children().contains(path[i]);
    int cnt = current_node->Children().size();
    if (!current_node->ContainsChild(path[i])) {
      QMenu *menu;
      if (i == 0)
        menu = menuBar()->addMenu(path[i]);
      else
        menu = current_node->Menu()->addMenu(path[i]);
      current_node->Insert(path[i], menu);
    }

    current_node = current_node->GetChild(path[i]);
  }
  return current_node;
}

MenuTreeNode *MainModule::GetMenu(const QString &name) {
  return GetMenu({name}, 1);
}

}
}
}
