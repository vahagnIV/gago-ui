file(GLOB SOURCES *.h *.cpp config/*.h config/*.cpp)


set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_wrap_ui(GagoUI)

qt5_wrap_cpp(GagoCPP)

INCLUDE_DIRECTORIES(include ${STERIO_INCLUDE_PATH} ${Qt5Widgets_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} external/concurrentqueue)

add_executable(Gago ${SOURCES} ${GagoUI} ${GagoCPP} module_manager.cpp module_manager.h imodule.h config/configuration.cpp config/configuration.h)

target_link_libraries(Gago
        ${QT_LIBRARIES}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        Qt5::Widgets
        pthread
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
add_subdirectory(modules)
