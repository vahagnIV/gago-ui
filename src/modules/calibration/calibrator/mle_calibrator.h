//
// Created by vahagn on 1/18/20.
//

#ifndef GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
#define GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
#include <QDialog>
#include <QDir>
#include <QMediaPlayer>
#include <QSharedPointer>
#include <QTimer>
#include <pattern/pattern.h>
#include <settings/mle_calibrator_settings.h>
#include "icalibrator.h"
#include "pattern/ipattern_extractor.h"
#include "common/video_player.h"
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
                const QSharedPointer<gago::calibration::pattern::IPatternExtractor> & pattern,
                const QSharedPointer<gago::gui::configuration::MLECalibratorSettings> & settings,
                const QDir & cache_folder);
  virtual ~MLECalibrator();
  int Calibrate() override;
  void Notify(const std::shared_ptr<std::vector<io::video::Capture>> & ptr) override;
  void SetCameras(const std::vector<const io::video::CameraMeta *> & vector) override;
  const gago::calibration::CalibrationEstimates & GetEstimates() const override;

 private slots:
  void OnCalibrateButtonClicked();
  void Close();
  void OnCaptureButtonClicked();
  void RestoreFilenames(const char *format, QStringList cameras_);
  void DisableControlElementsSlot();
  void EnableControlElementsSlot();
  void OnSaveButtonClicked();
  void PlaySoundFromPath(const QString & path);
  void TimerElapsed();
  void SetNextCaptureTime();


 signals:
  void DisableControlElements();
  void EnableControlElements();
  void PlaySound(const QString & path);
  void PictureTaken();
  void NewImagesArrived(const QStringList & filenames);
 private:

  void ResetNextCaptureTime();

  Ui::MLECalibrationWindow *ui_;

  QList<common::VideoPlayer *> players_;
  QList<QLabel *> cam_labels_;
  QSharedPointer<gago::calibration::pattern::IPatternExtractor> pattern_;
  int control_disable_bit_mask_;

  QSharedPointer<gago::gui::configuration::MLECalibratorSettings> settings_;
  gago::calibration::CalibrationEstimates estimates_;

  std::chrono::system_clock::time_point next_capture_time_;
  int last_capture_index_;
  const char format[12] = "%s_%03d.jpg";
  QMap<QString, QMediaPlayer *> sound_effects_;

  // Calibration
  QDir sound_dir_;
  QDir cache_folder_;
  QTimer * timer_;
  const char * capture_button_format_;
  bool capture_in_progress_ = false;

};

}
}
}
#endif //GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
