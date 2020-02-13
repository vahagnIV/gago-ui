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