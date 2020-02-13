//
// Created by vahagn on 12/29/19.
//

#ifndef GAGO_UI_SRC_IMODULE_H_
#define GAGO_UI_SRC_IMODULE_H_
#include <QString>
#include <QList>

namespace gago {
namespace gui {
namespace modules {

enum ModuleInitializationResult {
  OK = 1,
  ERROR = 2
};

struct RequiredModuleParams {
  QString Name;
  unsigned int MinMajorVersion;
  unsigned int MinMinorVersion;
};

class IModule {
 public:
  IModule(const QString &name, const QString &system_name) : name_(name), system_name_(system_name) {}
  virtual ModuleInitializationResult Initalize() = 0;
  const QString &Name() const { return name_; }
  const QString &SystemName() const { return system_name_; }
  virtual unsigned int MajorVersion() const = 0;
  virtual unsigned int MinorVersion() const = 0;
  virtual void QRequiredModules(QList<RequiredModuleParams> &out_required_modules) = 0;
  virtual void SetRequiredModules(const QList<IModule *> &modules) = 0;
  virtual int GetWeight() const = 0;
  virtual void Start() {};
 protected:
  virtual ~IModule() = default;
  QString name_;
  QString system_name_;

};

}
}
}

#endif //GAGO_UI_SRC_IMODULE_H_
