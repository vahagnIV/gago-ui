#ifndef MAIN_MODULE_H
#define MAIN_MODULE_H

#include "../../imodule.h"
#include "main_window.h"


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
 private:
  gago::gui::modules::MainWindow main_window_;
};

}
}
}
#endif //MAIN_MODULE_H
