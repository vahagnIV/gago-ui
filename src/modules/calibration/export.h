#ifndef EXPORT_CALIBRATION_MODULE_H
#define EXPORT_CALIBRATION_MODULE_H

#include "calibration_module.h"

extern "C"
gago::gui::modules::IModule * get_module();

extern "C"
void dispose_module(gago::gui::modules::CalibrationModule * module_ptr);

#endif // EXPORT_CALIBRATION_H