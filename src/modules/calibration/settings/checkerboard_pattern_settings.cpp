//
// Created by vahagn on 2/14/20.
//

#include "checkerboard_pattern_settings.h"
#include "checkerboard_pattern_configurator.h"

namespace gago {
namespace gui {
namespace configuration {

IConfigurator *CheckerboardPatternSettings::GetConfigurator() {
  return new CheckerboardPatternConfigurator(this);
}

void CheckerboardPatternSettings::DisposeConfigurator(IConfigurator *configurator) {
  delete (CheckerboardPatternConfigurator *) configurator;
}

void CheckerboardPatternSettings::ApplyConfiguration(QSettings & settings, IConfigurator *configurator) {
  CheckerboardPatternConfigurator *cfg = (CheckerboardPatternConfigurator *) configurator;
  nx_ = cfg->GetNx();
  ny_ = cfg->GetNy();
  settings.setValue("nx", nx_);
  settings.setValue("ny", ny_);
}

void CheckerboardPatternSettings::Configure(QSettings & settings) {
  if (settings.contains("nx"))
    nx_ = settings.value("nx").toInt();
  if (settings.contains("ny"))
    ny_ = settings.value("ny").toInt();
}

const QString & CheckerboardPatternSettings::GetName() const {
  return name_;
}

unsigned int CheckerboardPatternSettings::GetNx() const {
  return nx_;
}

unsigned int CheckerboardPatternSettings::GetNy() const {
  return ny_;
}

}
}
}