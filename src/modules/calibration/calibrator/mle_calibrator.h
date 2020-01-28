//
// Created by vahagn on 1/18/20.
//

#ifndef GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
#define GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
#include <QDialog>
#include <QDir>
#include <QMediaPlayer>
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
  MLECalibrator(QWidget *parent,
                const std::shared_ptr<gago::calibration::pattern::IPattern> & pattern,
                const MLEConfigurationSettings & settings);
  ~MLECalibrator();
  void Calibrate() override;
  void Notify(const std::shared_ptr<std::vector<io::video::Capture>> & ptr) override;
  void SetCameras(const std::vector<const io::video::CameraMeta *> & vector) override;

 private slots:
  void on_pushButton_2_clicked();

 private slots:
  void Close();
  void CaptureRequested();
  void RestoreFilenames(const char *format, QStringList cameras_);
 private:
  Ui::MLECalibrationWindow *ui_;

  QList<common::VideoPlayer *> players_;
  std::shared_ptr<gago::calibration::pattern::IPattern> pattern_;

  MLEConfigurationSettings settings_;

  long next_capture_time_;
  int last_image_index;
  const char format[12] = "%s_%03d.jpg";
  QMediaPlayer *player;

  // Calibration
  std::vector<std::vector<std::string>> files_;

};

}
}
}
#endif //GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
