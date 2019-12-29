#ifndef EXPORT_MAIN_MODULE_H
#define EXPORT_MAIN_MODULE_H

#include "main_module.h"

extern "C"
gago::gui::modules::IModule * get_module();

extern "C"
void dispose_module(gago::gui::modules::MainModule * module_ptr);

#endif // EXPORT_MAIN_H