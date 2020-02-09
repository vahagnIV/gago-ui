//
// Created by vahagn on 12/29/19.
//

#include "module_manager.h"
#include <dlfcn.h>
#include <iostream>
#include <QVector>
#include<QDebug>

namespace gago {
namespace gui {
namespace modules {

ModuleManager::ModuleManager() = default;

bool ModuleManager::LoadModule(const std::string &path) {
  internal::_ModuleContainer container;
  container.handle = dlopen(path.c_str(), RTLD_NOW);
  if (!container.handle) {
    std::cout << "ERROR loading module " << path << "  " << dlerror() << std::endl;
    return false;
  }

  container.create_function = reinterpret_cast<t_create_function>(dlsym(container.handle, "get_module"));
  if (!container.create_function) {
    std::cout << "Library" << path << "Does not contain a definition of module" << dlerror() << std::endl;
    return false;
  }
  container.delete_function = reinterpret_cast<t_delete_function>(dlsym(container.handle, "dispose_module"));
  if (!container.delete_function) {
    std::cout << "ERROR loading module " << path << "  " << dlerror() << std::endl;
    return false;
  }

  container.module_ptr = container.create_function();
  if (!container.module_ptr) {
    std::cout << "ERROR initlizing module " << path << std::endl;
    return false;
  }

  if (container.module_ptr->Initalize() != modules::ModuleInitializationResult::OK) {
    std::cout << "ERROR initlizing module " << path << std::endl;
    return false;
  }
  if (QLoaded(container.module_ptr->SystemName())) {
    std::cout << "Module " << container.module_ptr->Name() << " is already loaded. Skipping..." << std::endl;
    container.delete_function(container.module_ptr);
    return false;
  }
  container.module_weight = modules_.size();

  modules_[container.module_ptr->SystemName()] = container;
  std::cout << "Successfully loaded module " << container.module_ptr->Name() + " "
            << container.module_ptr->MajorVersion() << "." << container.module_ptr->MinorVersion() << " from " << path
            << std::endl;
  return true;
}

bool ModuleManager::SatisfyRequirements() {
  for (std::pair<std::string, internal::_ModuleContainer> it: modules_) {
    std::vector<modules::RequiredModuleParams> required_module_params;
    it.second.module_ptr->QRequiredModules(required_module_params);
    std::vector<modules::IModule *> required_modules;

    required_modules.reserve(required_module_params.size());

    for (const modules::RequiredModuleParams &req_module_param: required_module_params) {

      if (!QLoaded(req_module_param.Name)) {
        std::cout << "Module " << req_module_param.Name << " required by " << it.second.module_ptr->Name() << "("
                  << it.second.module_ptr->SystemName() << ")"
                  << " is not loaded. " << std::endl;
        return false;
      }

      internal::_ModuleContainer &found_model_container = modules_[req_module_param.Name];

      if (GetCombinedVersion(req_module_param.MinMajorVersion,
                             req_module_param.MinMinorVersion) > GetCombinedVersion(
          found_model_container.module_ptr->MajorVersion(),
          found_model_container.module_ptr->MinorVersion())) {
        std::cout << "Module " << req_module_param.Name << " required by " << it.second.module_ptr->Name()
                  << " is too old. " << std::endl;
        return false;
      }
      found_model_container.module_weight += it.second.module_weight;

      required_modules.push_back(found_model_container.module_ptr);

    }
    it.second.module_ptr->SetRequiredModules(required_modules);
  }
}

uint64_t ModuleManager::GetCombinedVersion(uint32_t major, uint32_t minor) const {
  uint64_t combined = major;
  combined = (combined << 32) | minor;
  return combined;
}

bool ModuleManager::QLoaded(const std::string &module_name) {
  return modules_.find(module_name) != modules_.end();
}

modules::IModule *ModuleManager::GetModule(const std::string &system_name) const {
  return modules_.at(system_name).module_ptr;
}

ModuleManager::~ModuleManager() {

  QVector<internal::_ModuleContainer *> containers;
  for (auto &module : modules_)
    containers.append(&module.second);
  qSort(containers.begin(),
        containers.end(),
        [&](const internal::_ModuleContainer *first, internal::_ModuleContainer *second) {
          return first->module_ptr->GetWeight() < second->module_ptr->GetWeight();
        });

  for (internal::_ModuleContainer *module : containers) {
    if (module->module_ptr->SystemName() == "main")
      continue;
    qInfo() << QString::fromStdString(module->module_ptr->SystemName());
    module->delete_function(module->module_ptr);
    dlclose(module->handle);
  }
}

void ModuleManager::Start() {
  QVector<internal::_ModuleContainer *> containers;
  for (auto &module : modules_)
    containers.append(&module.second);
  qSort(containers.begin(),
        containers.end(),
        [&](const internal::_ModuleContainer *first, internal::_ModuleContainer *second) {
          return first->module_ptr->GetWeight() > second->module_ptr->GetWeight();
        });
  for (auto &module: modules_)
    module.second.module_ptr->Start();

}

}
}
}