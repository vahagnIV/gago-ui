//
// Created by vahagn on 1/16/20.
//

#ifndef GAGO_UI_CHECKERBOARD_CONFIGURATOR_H
#define GAGO_UI_CHECKERBOARD_CONFIGURATOR_H
#include <QGridLayout>
#include <QSpinBox>

#include "modules/settings/iconfigurator.h"

namespace gago {
namespace gui {
namespace configuration {

struct CheckerboardParams {
  int n_x;
  int n_y;
};

class CheckerboardConfigurator : IConfigurator {
 public:
  CheckerboardConfigurator() = default;
  void DrawConfigurationPage(QWidget *widget) override;
  void Apply() override;
  const QString &ConfigWindowName() const override;
  const CheckerboardParams &GetSettings() const { return current_params_; }
  virtual ~CheckerboardConfigurator();
 private:
  const QString window_name_ = "Checkerboard";
  QGridLayout *layout_;
  CheckerboardParams current_params_;
  QSpinBox *nx_box_;
  QSpinBox *ny_box_;

};

}
}
}
#endif //GAGO_UI_CHECKERBOARD_CONFIGURATOR_H
