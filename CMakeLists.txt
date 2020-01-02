cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/build)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

add_subdirectory(src)


