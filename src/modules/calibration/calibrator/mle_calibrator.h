//
// Created by vahagn on 1/18/20.
//

#ifndef GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
#define GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
#include <QDialog>
#include "icalibrator.h"
#include "../pattern/ipattern.h"
#include "common/video_player.h"
#include "mle_configuration_settings.h"

namespace Ui {
class MLECalibrationWindow;
}

namespace gago {
namespace gui {
namespace calibration {

class MLECalibrator : public QDialog, public ICalibrator {
 Q_OBJECT
 public:
  MLECalibrator(QWidget *parent ,
                const std::shared_ptr<gago::calibration::pattern::IPattern> &pattern, const MLEConfigurationSettings & settings);
  ~MLECalibrator();
  void Calibrate() override;
  void Notify(const std::shared_ptr<std::vector<io::video::Capture>> &ptr) override;
  void SetCameras(const std::vector<const io::video::CameraMeta *> &vector) override;
 private slots:
  void Close();
 private:
  std::vector<common::VideoPlayer *> players_;
  std::shared_ptr<gago::calibration::pattern::IPattern> pattern_;
  MLEConfigurationSettings settings_;
  Ui::MLECalibrationWindow *ui_;

};

}
}
}
#endif //GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
