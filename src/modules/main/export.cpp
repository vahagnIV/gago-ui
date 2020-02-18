#include "export.h"
#include "main_module.h"

gago::gui::modules::IModule *get_module() {
  return new gago::gui::modules::MainModule();
}

void dispose_module(gago::gui::modules::IModule *module_ptr) {
  delete module_ptr;
}