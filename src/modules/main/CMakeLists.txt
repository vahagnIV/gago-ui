cmake_minimum_required(VERSION 3.10)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MODULE_SOURCES *.h *.cpp)

list(REMOVE_ITEM MODULE_SOURCES test.cpp export.h export.cpp)

qt5_wrap_ui(QtUiSources main_window.ui )
qt5_wrap_cpp(QtCPPSources main_module.h)

add_library(main_module SHARED ${MODULE_SOURCES} ${QtUiSources} ${QtCPPSources})

add_executable(test_main ${MODULE_SOURCES} ${QtCPPSources} ${QtUiSources} test.cpp)


target_link_libraries(main_module ${QT_LIBRARIES} Qt5::Widgets pthread)
target_link_libraries(test_main ${QT_LIBRARIES} Qt5::Widgets pthread)