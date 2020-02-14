//
// Created by vahagn on 2/14/20.
//

#ifndef GAGO_UI_CHECKERBOARD_PATTERN_SETTINGS_H
#define GAGO_UI_CHECKERBOARD_PATTERN_SETTINGS_H

#include <modules/settings/iconfigurable.h>

namespace gago {
namespace gui {
namespace configuration {

class CheckerboardPatternSettings: public IConfigurable {
 public:

  unsigned int GetNx() const;
  unsigned int GetNy() const;

  // IConfigurable
  IConfigurator *GetConfigurator() override;
  void DisposeConfigurator(IConfigurator *configurator) override;
  void ApplyConfiguration(QSettings & settings, IConfigurator *configurator) override;
  void Configure(QSettings & settings) override;
  const QString & GetName() const override;

 private:
  unsigned int nx_;
  unsigned int ny_;
  QString name_= "Checkerboard";

};

}
}
}
#endif //GAGO_UI_CHECKERBOARD_PATTERN_SETTINGS_H
