//
// Created by vahagn on 12/29/19.
//

#ifndef GAGO_UI_SRC_MODULE_MANAGER_H_
#define GAGO_UI_SRC_MODULE_MANAGER_H_

#include <QString>
#include <QMap>

#include "imodule.h"

namespace gago {
namespace gui {
namespace modules {


typedef modules::IModule*(*t_create_function)();
typedef void(*t_delete_function)(modules::IModule*);

namespace internal {
struct _ModuleContainer{
  int module_weight;
  modules::IModule * module_ptr;
  void * handle;
  t_create_function create_function;
  t_delete_function delete_function;
};
}

class ModuleManager{
 public:
  bool LoadModule(const QString & path);
  bool QLoaded(const QString & module_name);
  void Start();

  // Singletone
  static ModuleManager& Instance() {
    static ModuleManager instance;
    return instance;
  }
  bool SatisfyRequirements();
  modules::IModule * GetModule(const QString &system_name) const;
  virtual ~ModuleManager();
 private:
  ModuleManager();
  QMap<QString, internal::_ModuleContainer> modules_;
  inline uint64_t GetCombinedVersion(uint32_t major, uint32_t minor) const;

};
}
}
}
#endif //GAGO_UI_SRC_MODULE_MANAGER_H_
