cmake_minimum_required(VERSION 3.10)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MODULE_SOURCES *.h *.cpp)

qt5_wrap_ui(QtUiSources settings_window.ui)
qt5_wrap_cpp(QtCPPSources settings_window.h settings_module.h)

add_library(settings_module SHARED ${MODULE_SOURCES} ${QtCPPSources} ${QtUiSources})

target_link_libraries(settings_module ${QT_LIBRARIES} Qt5::Widgets pthread)

set(GagoUISettingsModule 1.0.0)

set_property(TARGET settings_module PROPERTY VERSION ${GagoUISettingsModule_VERSION})
set_property(TARGET settings_module PROPERTY SOVERSION 1)
set_property(TARGET settings_module PROPERTY INTERFACE_GagoUISettingsModule_MAJOR_VERSION 1)
set_property(TARGET settings_module APPEND PROPERTY
        COMPATIBLE_INTERFACE_STRING GagoUISettingsModule_MAJOR_VERSION)

install(TARGETS settings_module
        EXPORT gagoSettingsTargets
        LIBRARY
        DESTINATION ${GagoUI_MODULE_PATH}
        PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/gago/ui/modules/settings
        CONFIGURATIONS Release)

install(FILES
        settings_module.h iconfigurable.h iconfigurator.h
        DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/gago/ui/modules/settings)