//
// Created by vahagn on 1/19/20.
//

#ifndef GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_MLE_CONFIGURATION_SETTINGS_H_
#define GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_MLE_CONFIGURATION_SETTINGS_H_
namespace gago {
namespace gui {
namespace calibration {

struct MLEConfigurationSettings{
  bool calibrate_camera_first;
  QString image_save_folder;
};

}}}

#endif //GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_MLE_CONFIGURATION_SETTINGS_H_
