cmake_minimum_required(VERSION 3.10)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MODULE_SOURCES *.h *.cpp)

list(REMOVE_ITEM ${MODULE_SOURCES} test.cpp)

add_library(camera_view_module SHARED ${MODULE_SOURCES} )

target_link_libraries(camera_view_module gagocommon ${QT_LIBRARIES} Qt5::Widgets pthread)

#add_executable(test_cam_view ${MODULE_SOURCES} test.cpp)
#target_link_libraries(test_cam_view ${Gago_LIBRARIES} ${QT_LIBRARIES} Qt5::Widgets pthread  ${Boost_LIBRARIES})