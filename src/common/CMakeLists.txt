cmake_minimum_required(VERSION 3.10)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MODULE_SOURCES *.h *.cpp)
qt5_wrap_cpp(QtCPPSources video_player.h)

add_library(gagocommon SHARED ${MODULE_SOURCES} ${QtCPPSources})
target_link_libraries(gagocommon ${QT_LIBRARIES} Qt5::Widgets pthread ${Gago_LIBRARIES} ${Boost_LIBRARIES} ${OpenCV_LIBRARIES})

target_include_directories(gagocommon INTERFACE ${CMAKE_CURRENT_LIST_DIR})
