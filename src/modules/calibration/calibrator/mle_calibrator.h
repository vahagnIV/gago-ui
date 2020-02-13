//
// Created by vahagn on 1/18/20.
//

#ifndef GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
#define GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
#include <QDialog>
#include <QDir>
#include <QMediaPlayer>
#include <pattern/pattern_estimation_parameters.h>
#include "icalibrator.h"
#include "../pattern/ipattern.h"
#include "common/video_player.h"
#include "mle_configuration_settings.h"
#include "calibration_estimates.h"
#include "rectified_image_view_window.h"

namespace Ui {
class MLECalibrationWindow;
}

namespace gago {
namespace gui {
namespace calibration {

class MLECalibrator : public QDialog, public ICalibrator {
 Q_OBJECT
 public:
  MLECalibrator(QWidget *parent,
                const std::shared_ptr<gago::calibration::pattern::IPattern> & pattern,
                const MLEConfigurationSettings & settings);
  virtual ~MLECalibrator();
  int Calibrate() override;
  void Notify(const std::shared_ptr<std::vector<io::video::Capture>> & ptr) override;
  void SetCameras(const std::vector<const io::video::CameraMeta *> & vector) override;
  const gago::calibration::CalibrationEstimates &GetEstimates() const override;

 private slots:
  void OnCalibrateButtonClicked();
  void Close();
  void CaptureRequested();
  void RestoreFilenames(const char *format, QStringList cameras_);
  void DisableControlElementsSlot();
  void EnableControlElementsSlot();
  void OnSaveButtonClicked();

signals:
  void DisableControlElements();
  void EnableControlElements();
 private:
  Ui::MLECalibrationWindow *ui_;

  QList<common::VideoPlayer *> players_;
  QList<QLabel *> cam_labels_;
  std::shared_ptr<gago::calibration::pattern::IPattern> pattern_;
  int control_disable_bit_mask_;

  MLEConfigurationSettings settings_;
  gago::calibration::CalibrationEstimates estimates_;

  long next_capture_time_;
  int last_capture_index_;
  const char format[12] = "%s_%03d.jpg";
  QMediaPlayer *player;

  // Calibration
  QList<QStringList> files_;

};

}
}
}
#endif //GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
