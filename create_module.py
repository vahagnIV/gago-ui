import argparse
import os
import re


def convert(name):
    s1 = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', s1).lower()


def create_module(name: str, system_name: str, class_name: str = None):
    base_dir = os.path.join('src/modules', system_name)
    if os.path.exists(base_dir):
        print('Module {} already exist. Remove before proceeding.')
        return

    os.mkdir(base_dir)

    module_class_name = class_name or system_name.capitalize() + 'Module'

    module_include_filename = convert(module_class_name) + '.h'

    with open(os.path.join(base_dir, module_include_filename), 'w') as include_file:
        include_file.write("""#ifndef {0}_MODULE_H
#define {0}_MODULE_H

#include "../../imodule.h"

namespace gago {{
namespace gui {{
namespace modules {{

class {1}: public IModule{{

public:
    {1}();
    virtual ModuleInitializationResult Initalize () override;
    virtual unsigned int MajorVersion () const override;
    virtual unsigned int MinorVersion () const override;
    virtual void QRequiredModules(std::vector<RequiredModuleParams> & out_required_modules) override;
    virtual void SetRequiredModules(const std::vector<IModule *> & modules) override;        
}};

}}
}}
}}
#endif //{0}_MODULE_H
""".format(system_name.upper(), module_class_name))

    module_cpp_filename = convert(module_class_name) + '.cpp'
    with open(os.path.join(base_dir, module_cpp_filename), 'w') as cpp_file:
        cpp_file.write("""#include "{0}"

namespace gago {{
namespace gui {{
namespace modules {{

{1}::{1}():IModule("{2}", "{3}"){{
}}

ModuleInitializationResult {1}::Initalize () {{
    return ModuleInitializationResult::OK;
}}

unsigned int {1}::MajorVersion () const {{
    return 1;
}}

unsigned int {1}::MinorVersion () const {{
    return 0;
}}

void {1}::QRequiredModules(std::vector<RequiredModuleParams> & out_required_modules) {{    
}}

void {1}::SetRequiredModules(const std::vector<IModule *> & modules) {{
    
}}

}}
}}
}}
""".format(module_include_filename, module_class_name, name, system_name))

    export_include_filename = os.path.join(base_dir, 'export.h')
    with open(export_include_filename, 'w') as export_file:
        export_file.write("""#ifndef EXPORT_{0}_MODULE_H
#define EXPORT_{0}_MODULE_H

#include "{1}"

extern "C"
gago::gui::modules::IModule * get_module();

extern "C"
void dispose_module(gago::gui::modules::{2} * module_ptr);

#endif // EXPORT_{0}_H""".format(system_name.upper(), module_include_filename, module_class_name))

    export_cpp_filename = os.path.join(base_dir, 'export.cpp')
    with open(export_cpp_filename, 'w') as export_cpp_file:
        export_cpp_file.write("""#include "export.h"

gago::gui::modules::IModule * get_module()
{{
    return new gago::gui::modules::{0}();
}}

void dispose_module(gago::gui::modules::{0} * module_ptr)
{{
    delete module_ptr;
}}""".format(module_class_name))

    cmake_file_path = os.path.join(base_dir, 'CopyIfNewer.txt')
    with open(cmake_file_path, 'w') as cmake_file:
        cmake_file.write("""cmake_minimum_required(VERSION 3.10)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MODULE_SOURCES *.h *.cpp)

add_library({0}_module SHARED ${{MODULE_SOURCES}} )

target_link_libraries({0}_module ${{QT_LIBRARIES}} Qt5::Widgets pthread)""".format(system_name))


if __name__ == '__main__':
    create_module('Main', 'main')
