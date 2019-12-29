cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/build)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(OpenCV REQUIRED)

add_subdirectory(src)


