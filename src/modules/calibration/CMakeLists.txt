cmake_minimum_required(VERSION 3.10)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MODULE_SOURCES *.h *.cpp)

add_library(calibration_module SHARED ${MODULE_SOURCES} )

target_link_libraries(calibration_module ${QT_LIBRARIES} Qt5::Widgets pthread)