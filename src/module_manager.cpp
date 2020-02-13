//
// Created by vahagn on 12/29/19.
//

#include <dlfcn.h>

#include <QVector>
#include<QDebug>

#include "module_manager.h"

namespace gago {
namespace gui {
namespace modules {

ModuleManager::ModuleManager() = default;

bool ModuleManager::LoadModule(const QString & path) {
  internal::_ModuleContainer container;
  container.handle = dlopen(path.toStdString().c_str(), RTLD_NOW);
  if (!container.handle) {
    qWarning() << "ERROR loading module " << path << "  " << dlerror();
    return false;
  }

  container.create_function = reinterpret_cast<t_create_function>(dlsym(container.handle, "get_module"));
  if (!container.create_function) {
    qWarning() << "Library" << path << "Does not contain a definition of module " << dlerror();
    return false;
  }
  container.delete_function = reinterpret_cast<t_delete_function>(dlsym(container.handle, "dispose_module"));
  if (!container.delete_function) {
    qWarning() << "ERROR loading module " << path << "  " << dlerror();
    return false;
  }

  container.module_ptr = container.create_function();
  if (!container.module_ptr) {
    qWarning() << "ERROR initlizing module " << path;
    return false;
  }

  if (container.module_ptr->Initalize() != modules::ModuleInitializationResult::OK) {
    qWarning() << "ERROR initlizing module " << path;
    return false;
  }
  if (QLoaded(container.module_ptr->SystemName())) {
    qWarning() << "Module " << container.module_ptr->Name() << " is already loaded. Skipping...";
    container.delete_function(container.module_ptr);
    return false;
  }
  container.module_weight = modules_.size();

  modules_[container.module_ptr->SystemName()] = container;
  qWarning() << "Successfully loaded module " << container.module_ptr->Name() + " "
             << container.module_ptr->MajorVersion() << "." << container.module_ptr->MinorVersion() << " from " << path;
  return true;
}

bool ModuleManager::SatisfyRequirements() {

  for (internal::_ModuleContainer & container: modules_.values()) {

    QList<modules::RequiredModuleParams> required_module_params;
    container.module_ptr->GetRequiredModules(required_module_params);
    QList<modules::IModule *> required_modules;

    required_modules.reserve(required_module_params.size());

    for (const modules::RequiredModuleParams & req_module_param: required_module_params) {

      if (!QLoaded(req_module_param.Name)) {
        qWarning() << "Module " << req_module_param.Name << " required by " << container.module_ptr->Name() << "("
                   << container.module_ptr->SystemName() << ")"
                   << " is not loaded. ";
        return false;
      }

      internal::_ModuleContainer & found_model_container = modules_[req_module_param.Name];

      if (GetCombinedVersion(req_module_param.MinMajorVersion,
                             req_module_param.MinMinorVersion) > GetCombinedVersion(
          found_model_container.module_ptr->MajorVersion(),
          found_model_container.module_ptr->MinorVersion())) {
        qWarning() << "Module " << req_module_param.Name << " required by " << container.module_ptr->Name()
                   << " is too old. ";
        return false;
      }
      found_model_container.module_weight += container.module_weight;

      required_modules.push_back(found_model_container.module_ptr);

    }
    container.module_ptr->SetRequiredModules(required_modules);
  }
}

uint64_t ModuleManager::GetCombinedVersion(uint32_t major, uint32_t minor) const {
  uint64_t combined = major;
  combined = (combined << 32) | minor;
  return combined;
}

bool ModuleManager::QLoaded(const QString & module_name) {
  return modules_.find(module_name) != modules_.end();
}

modules::IModule *ModuleManager::GetModule(const QString & system_name) const {
  return modules_[system_name].module_ptr;
}

ModuleManager::~ModuleManager() {

  QList<internal::_ModuleContainer *> containers;
  for (auto & module : modules_.values())
    containers.append(&module);
  qSort(containers.begin(),
        containers.end(),
        [&](const internal::_ModuleContainer *first, internal::_ModuleContainer *second) {
          return first->module_ptr->GetWeight() < second->module_ptr->GetWeight();
        });

  for (internal::_ModuleContainer *module : containers) {
    if (module->module_ptr->SystemName() == "main")
      continue;
    qInfo() << module->module_ptr->SystemName();
    module->delete_function(module->module_ptr);
    dlclose(module->handle);
  }
}

void ModuleManager::Start() {
  QVector<internal::_ModuleContainer *> containers;
  for (auto & module : modules_.values())
    containers.append(&module);
  qSort(containers.begin(),
        containers.end(),
        [&](const internal::_ModuleContainer *first, internal::_ModuleContainer *second) {
          return first->module_ptr->GetWeight() > second->module_ptr->GetWeight();
        });
  for (auto & module: modules_)
    module.module_ptr->Start();
}

}
}
}