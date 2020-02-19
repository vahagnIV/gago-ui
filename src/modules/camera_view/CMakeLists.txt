cmake_minimum_required(VERSION 3.10)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MODULE_SOURCES *.h *.cpp)

list(REMOVE_ITEM MODULE_SOURCES test.cpp)

add_library(camera_view_module SHARED ${MODULE_SOURCES} )

target_link_libraries(camera_view_module gagocommon ${QT_LIBRARIES} Qt5::Widgets pthread)

set(GagoUICameraViewModule 1.0.0)

set_property(TARGET camera_view_module PROPERTY VERSION ${GagoUICameraViewModule_VERSION})
set_property(TARGET camera_view_module PROPERTY SOVERSION 1)
set_property(TARGET camera_view_module PROPERTY INTERFACE_GagoUICameraViewModule_MAJOR_VERSION 1)
set_property(TARGET camera_view_module APPEND PROPERTY
        COMPATIBLE_INTERFACE_STRING GagoUICameraViewModule_MAJOR_VERSION)

install(TARGETS camera_view_module
        EXPORT gagoCameraViewTargets
        LIBRARY
        DESTINATION ${GagoUI_MODULE_PATH}
        PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/gago/ui/modules/camera_view
        CONFIGURATIONS Release)

install(FILES
        camera_view_module.h
        DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/gago/ui/modules/camera_view)
