#include "export.h"

gago::gui::modules::IModule * get_module()
{
    return new gago::gui::modules::CalibrationModule();
}

void dispose_module(gago::gui::modules::CalibrationModule * module_ptr)
{
    delete module_ptr;
}