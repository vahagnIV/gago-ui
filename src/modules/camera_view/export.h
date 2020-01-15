#ifndef EXPORT_CAMERA_VIEW_MODULE_H
#define EXPORT_CAMERA_VIEW_MODULE_H

#include "camera_view_module.h"

extern "C"
gago::gui::modules::IModule * get_module();

extern "C"
void dispose_module(gago::gui::modules::Camera_viewModule * module_ptr);

#endif // EXPORT_CAMERA_VIEW_H