#include "export.h"
#include <iostream>
gago::gui::modules::IModule *get_module() {
  return new gago::gui::modules::SettingsModule();
}

void dispose_module(gago::gui::modules::IModule *module_ptr) {
  delete module_ptr;
}