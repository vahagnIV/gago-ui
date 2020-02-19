#ifndef SETTINGS_MODULE_H
#define SETTINGS_MODULE_H

#include <QMainWindow>
#include <QList>
#include <QSettings>
#include "imodule.h"
#include "iconfigurable.h"
#include "settings_window.h"

namespace gago {
namespace gui {
namespace modules {

class SettingsModule : public QObject, public IModule {
 Q_OBJECT

 public:
  SettingsModule();

  // IModule
  ModuleInitializationResult Initalize() override;
  unsigned int MajorVersion() const override;
  unsigned int MinorVersion() const override;
  void GetRequiredModules(QList<RequiredModuleParams> & out_required_modules) override;
  void SetRequiredModules(const QList<IModule *> & modules) override;
  void Start() override ;

  //Settings
  virtual void RegisterConfigurable(configuration::IConfigurable *);
  virtual void Configure();
  virtual ~SettingsModule() {};
  int GetDestructorIndex() const override;
  int GetStartIndex() const override ;
 private slots:
  void ShowSettings();
 private:
  void Save();
  QMainWindow *main_window_;
  QList<configuration::IConfigurable *> configurables_;
  QSettings settings_;

};

}
}
}
#endif //SETTINGS_MODULE_H
