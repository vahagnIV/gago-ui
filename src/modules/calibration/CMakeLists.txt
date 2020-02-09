cmake_minimum_required(VERSION 3.10)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB MODULE_SOURCES *.h *.cpp pattern/*.h pattern/*.cpp calibrator/*.h calibrator/*.cpp)

list(REMOVE_ITEM MODULE_SOURCES test.cpp)

qt5_wrap_cpp(QtCPPSources calibration_configurator.h calibrator/mle_calibrator.h calibrator/image_set_view.h calibration_module.h calibrator/rectified_image_view_window.h )
qt5_wrap_ui(QtUISources calibrator/mle_calibration_window.ui calibrator/rectified_image_view_window.ui)

message(${Qt5Multimedia_LIBRARIES})

add_library(calibration_module SHARED ${QtCPPSources} ${MODULE_SOURCES} ${QtUISources})

target_link_libraries(calibration_module ${QT_LIBRARIES} ${Qt5Widgets_LIBRARIES} pthread ${OpenCV_LIBRARIES} gagocommon ${Qt5Multimedia_LIBRARIES})

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS} ${Qt5Multimedia_INCLUDE_DIRS} calibrator)

add_executable(test_calib test.cpp ${QtCPPSources} ${MODULE_SOURCES} ${QtUISources})
target_link_libraries(test_calib ${QT_LIBRARIES} ${Qt5Widgets_LIBRARIES} pthread ${OpenCV_LIBRARIES} gagocommon ${Qt5Multimedia_LIBRARIES})
