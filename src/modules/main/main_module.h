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
  virtual QAction *AppendMenuAction(const std::string &path);
  virtual bool CreateMenuBranch(std::string path);
 private:
  MainWindow main_window_;
};

}
}
}
#endif //MAIN_MODULE_H
