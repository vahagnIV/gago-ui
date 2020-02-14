//
// Created by vahagn on 2/14/20.
//

#ifndef GAGO_UI_CHECKERBOARD_PATTERN_CONFIGURATOR_H
#define GAGO_UI_CHECKERBOARD_PATTERN_CONFIGURATOR_H
#include <QGridLayout>
#include <QSpinBox>

#include "modules/settings/iconfigurator.h"

namespace gago {
namespace gui {
namespace configuration {

class CheckerboardPatternSettings;

class CheckerboardPatternConfigurator: public IConfigurator {
 public:
  CheckerboardPatternConfigurator(CheckerboardPatternSettings * settings);
  void DrawConfigurationPage(QWidget *widget) override;
  void Apply() override;
  const QString & ConfigWindowName() const override;
  int GetNx() const;
  int GetNy() const;
 private:
  const QString name_ = "Checkerboard";
  QGridLayout *layout_;
  QSpinBox *nx_box_;
  QSpinBox *ny_box_;
  CheckerboardPatternSettings *setting_;

};

}
}
}
#endif //GAGO_UI_CHECKERBOARD_PATTERN_CONFIGURATOR_H
