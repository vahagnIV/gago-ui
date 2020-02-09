//
// Created by vahagn on 12/29/19.
//

#ifndef GAGO_UI_SRC_MODULE_MANAGER_H_
#define GAGO_UI_SRC_MODULE_MANAGER_H_

#include <unordered_map>
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
  std::string path;
  void * handle;
  t_create_function create_function;
  t_delete_function delete_function;
};
}

class ModuleManager{
 public:
  bool LoadModule(const std::string & path);
  bool QLoaded(const std::string & module_name);
  void Start();

  // Singletone
  static ModuleManager& Instance() {
    static ModuleManager instance;
    return instance;
  }
  bool SatisfyRequirements();
  modules::IModule * GetModule(const std::string &system_name) const;
  virtual ~ModuleManager();
 private:
  ModuleManager();
  std::unordered_map<std::string, internal::_ModuleContainer> modules_;
  inline uint64_t GetCombinedVersion(uint32_t major, uint32_t minor) const;

};
}
}
}
#endif //GAGO_UI_SRC_MODULE_MANAGER_H_
