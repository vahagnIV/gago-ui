#ifndef EXPORT_SETTINGS_MODULE_H
#define EXPORT_SETTINGS_MODULE_H

#include "settings_module.h"

extern "C"
gago::gui::modules::IModule * get_module();

extern "C"
void dispose_module(gago::gui::modules::IModule * module_ptr);

#endif // EXPORT_SETTINGS_H