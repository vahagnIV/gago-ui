cmake_minimum_required(VERSION 3.10)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MODULE_SOURCES *.h *.cpp pattern/*.h pattern/*.cpp calibrator/*.h calibrator/*.cpp settings/*.cpp settings/*.h)

qt5_wrap_cpp(QtCPPSources settings/calibration_configurator.h
        calibrator/mle_calibrator.h
        calibrator/image_set_view.h
        calibration_module.h
        calibrator/rectified_image_view_window.h
        settings/mle_calibrator_configurator.h )
qt5_wrap_ui(QtUISources calibrator/mle_calibration_window.ui calibrator/rectified_image_view_window.ui)

message(${Qt5Multimedia_LIBRARIES})

add_library(calibration_module SHARED ${QtCPPSources} ${MODULE_SOURCES} ${QtUISources})

target_link_libraries(calibration_module ${QT_LIBRARIES} ${Qt5Widgets_LIBRARIES} pthread ${OpenCV_LIBRARIES} gagocommon ${Qt5Multimedia_LIBRARIES})

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS} ${Qt5Multimedia_INCLUDE_DIRS} calibrator)
set(GagoUICalibrationModule 1.0.0)

set_property(TARGET calibration_module PROPERTY VERSION ${GagoUICalibrationModule_VERSION})
set_property(TARGET calibration_module PROPERTY SOVERSION 1)
set_property(TARGET calibration_module PROPERTY INTERFACE_GagoUICalibrationModule_MAJOR_VERSION 1)
set_property(TARGET calibration_module APPEND PROPERTY
        COMPATIBLE_INTERFACE_STRING GagoUICalibrationModule_MAJOR_VERSION)

install(TARGETS calibration_module
        EXPORT gagoCameraViewTargets
        LIBRARY
        DESTINATION ${GagoUI_MODULE_PATH}
        PUBLIC_HEADER
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/gago/ui/modules/calibration
        CONFIGURATIONS Release)

install(FILES
        calibration_module.h
        DESTINATION
        ${CMAKE_INSTALL_PREFIX}/include/gago/ui/modules/calibration)

