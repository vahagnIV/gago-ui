cmake_minimum_required(VERSION 3.10)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MODULE_SOURCES *.h *.cpp)
qt5_wrap_cpp(QtCPPSources video_player.h)

add_library(gagocommon STATIC ${MODULE_SOURCES} ${QtCPPSources})
target_link_libraries(gagocommon ${QT_LIBRARIES} Qt5::Widgets pthread ${Gago_LIBRARIES} ${Boost_LIBRARIES} ${OpenCV_LIBRARIES})

target_include_directories(gagocommon INTERFACE ${CMAKE_CURRENT_LIST_DIR})

set(GagoUICommon 1.0.0)

set_property(TARGET gagocommon PROPERTY VERSION ${GagoUICommon_VERSION})
set_property(TARGET gagocommon PROPERTY SOVERSION 1)
set_property(TARGET gagocommon PROPERTY INTERFACE_GagoUICommon_MAJOR_VERSION 1)
set_property(TARGET gagocommon APPEND PROPERTY
        COMPATIBLE_INTERFACE_STRING GagoUICommon_MAJOR_VERSION)

install(TARGETS gagocommon
        EXPORT gagoCommonViewTargets
        ARCHIVE
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/gago/ui/common
        CONFIGURATIONS Release)

install(FILES
        video_player.h
        DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/gago/ui/common)
