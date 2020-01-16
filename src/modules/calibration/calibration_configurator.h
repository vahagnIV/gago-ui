//
// Created by vahagn on 1/15/20.
//

#ifndef GAGO_UI_CALIBRATION_CONFIGURATOR_H
#define GAGO_UI_CALIBRATION_CONFIGURATOR_H
#include <QObject>
#include <QFrame>
#include <QComboBox>
#include <QGroupBox>
#include "modules/settings/iconfigurator.h"
#include "calib_pattern_configurator_factory.h"

namespace gago {
namespace gui {
namespace configuration {

struct CalibrationConfiguration {
  CalibrationPatternType calib_pattern_type;
};

class CalibrationConfigurator : public QObject, public IConfigurator {
 Q_OBJECT
 public:
  CalibrationConfigurator(QObject *parent = nullptr);

  void DrawConfigurationPage(QWidget *widget) override;
  void Apply() override;
  void GetConfiguration(nlohmann::json & out_json) override;
  void SetConfiguration(const nlohmann::json & json) override;
  const std::string & ConfigWindowName() const override;
  virtual ~CalibrationConfigurator();

  IConfigurator *GetActivePatternConfigurator();
 private slots:
  void CalibrationPatternChanged(int idx);

 private:
  void DrawPatternSide();
  void DrawCalibratorSide();
  std::string window_name = "Calibration";
  CalibrationConfiguration current_calibration_settings_;

  QGroupBox *calibrator_group_box_;

  // Calib pattern
  gago::gui::configuration::CalibPatternConfiguratorFactory calib_pattern_configurator_factory_;
  std::vector<IConfigurator *> calib_pattern_configurators_;
  std::vector<QFrame *> calib_pattern_configurator_frames_;
  QGroupBox *pattern_group_box_ = nullptr;
  QComboBox *patterns_combo;

};

}
}
}
#endif //GAGO_UI_CALIBRATION_CONFIGURATOR_H
