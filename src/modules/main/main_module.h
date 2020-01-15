#ifndef MAIN_MODULE_H
#define MAIN_MODULE_H

#include "../../imodule.h"
#include "main_window.h"
#include "view.h"

namespace gago {
namespace gui {
namespace modules {

class MainModule : public IModule {

 public:
  MainModule();
  // IModule
  ModuleInitializationResult Initalize() override;
  unsigned int MajorVersion() const override;
  unsigned int MinorVersion() const override;
  void QRequiredModules(std::vector<RequiredModuleParams> &out_required_modules) override;
  void SetRequiredModules(const std::vector<IModule *> &modules) override;

  // Main Module
  virtual void Show();
  virtual QAction *CreateMenuBranch(std::string path);
  virtual QMainWindow *MainWindow() ;
  virtual ~MainModule() = default;

  virtual void RegisterView(View * view);
 private:
  void SetCurrentView(int idx);
  gago::gui::modules::MainWindow main_window_;
  std::vector<View *> views_;
  int current_view_index_;
};

}
}
}
#endif //MAIN_MODULE_H
