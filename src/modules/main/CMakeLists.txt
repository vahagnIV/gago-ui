cmake_minimum_required(VERSION 3.10)
find_package(Qt5Widgets CONFIG REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MODULE_SOURCES *.h *.cpp)

qt5_wrap_ui(QtUiSources main_window.ui)
qt5_wrap_cpp(QtCPPSources main_module.h)

add_library(main_module SHARED ${MODULE_SOURCES} ${QtUiSources} ${QtCPPSources} ${UI_RESOURCES_RCC})

target_link_libraries(main_module ${QT_LIBRARIES} Qt5::Widgets pthread)


set(GagoUIMainModule 1.0.0)

set_property(TARGET main_module PROPERTY VERSION ${GagoUIMainModule_VERSION})
set_property(TARGET main_module PROPERTY SOVERSION 1)
set_property(TARGET main_module PROPERTY INTERFACE_GagoUIMainModule_MAJOR_VERSION 1)
set_property(TARGET main_module APPEND PROPERTY
        COMPATIBLE_INTERFACE_STRING GagoUIMainModule_MAJOR_VERSION)

install(TARGETS main_module
        EXPORT gagoMainTargets
        LIBRARY
        DESTINATION ${GagoUI_MODULE_PATH}
        PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/gago/ui/modules/main
        CONFIGURATIONS Release)

install(FILES
        view.h main_module.h
        DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/gago/ui/modules/main)