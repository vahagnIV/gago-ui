#include "export.h"

gago::gui::modules::IModule * get_module()
{
    return new gago::gui::modules::Camera_viewModule();
}

void dispose_module(gago::gui::modules::IModule * module_ptr)
{

    delete (gago::gui::modules::Camera_viewModule *)module_ptr;
}