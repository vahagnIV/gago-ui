#ifndef SETTINGS_MODULE_H
#define SETTINGS_MODULE_H

#include <QMainWindow>
#include "imodule.h"
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
    void QRequiredModules(QList<RequiredModuleParams> & out_required_modules) override;
    void SetRequiredModules(const QList<IModule *> & modules) override;

    //Settings
    virtual void RegisterConfigurable(configuration::IConfigurable *);
    virtual void Configure();
    virtual ~SettingsModule() = default;
  int GetWeight() const override;
 private:
  void Save (std::vector<configuration::IConfigurator *> &configurators);
  QMainWindow *main_window_;
  std::vector<configuration::IConfigurable *> configurables_;

};

}
}
}
#endif //SETTINGS_MODULE_H
