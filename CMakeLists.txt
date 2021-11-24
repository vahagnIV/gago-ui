cmake_minimum_required(VERSION 3.10)
project(GagoUI)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/Modules)
include(GagoUIConfig)
add_subdirectory(src)


