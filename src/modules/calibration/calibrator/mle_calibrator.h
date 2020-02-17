//
// Created by vahagn on 1/18/20.
//

#ifndef GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
#define GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
#include <QDialog>
#include <QDir>
#include <QSoundEffect>
#include <QSharedPointer>
#include <pattern/pattern_estimation_parameters.h>
#include <settings/mle_calibrator_settings.h>
#include "icalibrator.h"
#include "pattern/ipattern.h"
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
                const QSharedPointer<gago::calibration::pattern::IPattern> & pattern,
                const QSharedPointer<gago::gui::configuration::MLECalibratorSettings> & settings);
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
  void PlaySoundFromPath(const QString & path);

signals:
  void DisableControlElements();
  void EnableControlElements();
  void PlaySound(const QString & path);
 private:
  Ui::MLECalibrationWindow *ui_;

  QList<common::VideoPlayer *> players_;
  QList<QLabel *> cam_labels_;
  QSharedPointer<gago::calibration::pattern::IPattern> pattern_;
  int control_disable_bit_mask_;

  QSharedPointer<gago::gui::configuration::MLECalibratorSettings> settings_;
  gago::calibration::CalibrationEstimates estimates_;

  long next_capture_time_;
  int last_capture_index_;
  const char format[12] = "%s_%03d.jpg";
  QMap<QString, QSoundEffect *> sound_effects_;

  // Calibration
  QList<QStringList> files_;
  QDir sound_dir_;

};

}
}
}
#endif //GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATION_WINDOW_H_
