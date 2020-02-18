#ifndef MAIN_MODULE_H
#define MAIN_MODULE_H

#include <QMainWindow>

#include "imodule.h"
#include "view.h"
#include "menu_tree.h"

namespace Ui {
class MainWindow;
}

namespace gago {
namespace gui {
namespace modules {

class MainModule : public QMainWindow, public IModule {
 Q_OBJECT

 public:
  MainModule();
  // IModule
  ModuleInitializationResult Initalize() override;
  unsigned int MajorVersion() const override;
  unsigned int MinorVersion() const override;
  void GetRequiredModules(QList<RequiredModuleParams> &out_required_modules) override;
  void SetRequiredModules(const QList<IModule *> &modules) override;
  int GetDestructorIndex() const override;
  void Start() override;

  // Main Module
  virtual QMainWindow *MainWindow();
  virtual void RegisterView(View *view);
  virtual QAction * GetAction(const QString &path);
  virtual void DisableAllViews();

  virtual ~MainModule();

 private:

  MenuTreeNode * GetMenu(const QStringList &name, int count);
  MenuTreeNode * GetMenu(const QString &path);
  Ui::MainWindow *ui;
  void SetCurrentView(int idx);
  QList<View *> views_;
  int current_view_index_;
  MenuTreeNode menus_;
};

}
}
}
#endif //MAIN_MODULE_H
