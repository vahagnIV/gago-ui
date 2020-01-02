#ifndef SETTINGS_MODULE_H
#define SETTINGS_MODULE_H

#include <QtWidgets/QMainWindow>
#include "../../imodule.h"
#include "iconfigurable.h"
#include "settings_window.h"

namespace gago {
namespace gui {
namespace modules {

class SettingsModule: public IModule{

public:
    SettingsModule();

    // IModule
    ModuleInitializationResult Initalize () override;
    unsigned int MajorVersion () const override;
    unsigned int MinorVersion () const override;
    void QRequiredModules(std::vector<RequiredModuleParams> & out_required_modules) override;
    void SetRequiredModules(const std::vector<IModule *> & modules) override;

    //Settings
    virtual void RegisterConfigurable(configuration::IConfigurable *);
    virtual ~SettingsModule() = default;
 private:
  void Save (std::vector<configuration::IConfigurator *> &configurators);
  QMainWindow *main_window_;
  std::vector<configuration::IConfigurable *> configurables_;

};

}
}
}
#endif //SETTINGS_MODULE_H
