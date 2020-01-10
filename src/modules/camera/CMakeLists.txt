cmake_minimum_required(VERSION 3.10)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MODULE_SOURCES *.h *.cpp)

list(REMOVE_ITEM ${MODULE_SOURCES} test.cpp)

add_library(camera_module SHARED ${MODULE_SOURCES} )

target_link_libraries(camera_module ${QT_LIBRARIES} Qt5::Widgets pthread ${Gago_LIBRARIES} ${Boost_LIBRARIES})

add_executable(test_cam ${MODULE_SOURCES} test.cpp)
target_link_libraries(test_cam ${Gago_LIBRARIES} ${QT_LIBRARIES} Qt5::Widgets pthread  ${Boost_LIBRARIES})
