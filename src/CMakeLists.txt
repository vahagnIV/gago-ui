find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(OpenCV REQUIRED)
find_package(LibGago REQUIRED)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTORCC ON)

file(GLOB SOURCES *.h *.cpp config/*.h)

INCLUDE_DIRECTORIES(include ${Qt5Widgets_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR} ${LibGago_INCLUDE_DIRS})


add_subdirectory(common)
add_subdirectory(modules)

add_executable(Gago ${SOURCES} ${GagoUI} ${GagoCPP} resources.qrc module_manager.cpp module_manager.h imodule.h common/configuration.cpp common/configuration.h)

target_link_libraries(Gago
        ${QT_LIBRARIES}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Gago_LIBRARIES}
        Qt5::Widgets
        gagocommon
        dl)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/config.json
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/config.json
        COMMAND "${CMAKE_COMMAND}" -D SOURCE_FILE=${CMAKE_CURRENT_SOURCE_DIR}/config.json
        -D DESTINATION_FILE=${CMAKE_CURRENT_BINARY_DIR}/config.json
        -P "${CMAKE_SOURCE_DIR}/build/CopyIfNewer.cmake"
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/config.json
        COMMENT "Copying  ${CMAKE_CURRENT_SOURCE_DIR}/config.json to ${CMAKE_CURRENT_BINARY_DIR}/config.json...")

add_custom_target(gago_config DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/config.json)

add_dependencies(Gago gago_config)


# Installation

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,${CMAKE_INSTALL_PREFIX}/lib")


set(GagoUI_VERSION 1.0.0)

include(GenerateExportHeader)

set_property(TARGET Gago PROPERTY VERSION ${GagoUI_VERSION})
set_property(TARGET Gago PROPERTY SOVERSION 1)
set_property(TARGET Gago PROPERTY INTERFACE_Gago_MAJOR_VERSION 1)
set_property(TARGET Gago APPEND PROPERTY
        COMPATIBLE_INTERFACE_STRING gago_MAJOR_VERSION)

install(TARGETS Gago
        EXPORT GagoTargets
        RUNTIME
        DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/
        PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/gago/ui
        CONFIGURATIONS Release)

install(FILES imodule.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/ui)

set(ConfigPackageLocation ${CMAKE_INSTALL_PREFIX}/lib/cmake/GagoUI)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/GagoUI/GagoUIConfigVersion.cmake"
        VERSION ${GagoUI_VERSION}
        COMPATIBILITY AnyNewerVersion)

install(FILES ${CMAKE_MODULE_PATH}/GagoUIConfig.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/GagoUI/GagoUIConfigVersion.cmake"
        DESTINATION ${ConfigPackageLocation})
add_definitions(-DGagoUI_MODULE_PATH="${GagoUI_MODULE_PATH}")



