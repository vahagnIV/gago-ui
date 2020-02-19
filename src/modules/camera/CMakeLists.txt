cmake_minimum_required(VERSION 3.10)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MODULE_SOURCES *.h *.cpp)

list(REMOVE_ITEM MODULE_SOURCES test.cpp)

add_library(camera_module SHARED ${MODULE_SOURCES} )

message(${LibGago_LIBRARIES})

target_link_libraries(camera_module ${QT_LIBRARIES} Qt5::Widgets pthread ${LibGago_LIBRARIES} ${Boost_LIBRARIES})

add_executable(test_cam ${MODULE_SOURCES} test.cpp)
target_link_libraries(test_cam ${LibGago_LIBRARIES} ${QT_LIBRARIES} Qt5::Widgets pthread  ${Boost_LIBRARIES})


set(GagoUICameraModule 1.0.0)

set_property(TARGET camera_module PROPERTY VERSION ${GagoUICameraModule_VERSION})
set_property(TARGET camera_module PROPERTY SOVERSION 1)
set_property(TARGET camera_module PROPERTY INTERFACE_GagoUICameraModule_MAJOR_VERSION 1)
set_property(TARGET camera_module APPEND PROPERTY
        COMPATIBLE_INTERFACE_STRING GagoUICameraModule_MAJOR_VERSION)

install(TARGETS camera_module
        EXPORT gagoCameraViewTargets
        LIBRARY
        DESTINATION ${GagoUI_MODULE_PATH}
        PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/gago/ui/modules/camera
        CONFIGURATIONS Release)

install(FILES
        camera_module.h
        DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/gago/ui/modules/camera)