#include "export.h"

gago::gui::modules::IModule *get_module() {
  return new gago::gui::modules::CameraModule();
}

void dispose_module(gago::gui::modules::IModule *module_ptr) {
  delete (gago::gui::modules::CameraModule *) module_ptr;
}