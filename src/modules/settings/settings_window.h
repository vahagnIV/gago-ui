//
// Created by vahagn on 12/30/19.
//

#ifndef GAGO_UI_SRC_MODULES_SETTINGS_SETTINGS_WINDOW_H_
#define GAGO_UI_SRC_MODULES_SETTINGS_SETTINGS_WINDOW_H_
#include <QDialog>
#include "iconfigurator.h"
namespace Ui {
class SettingsWindow;
}

namespace gago {
namespace gui {
namespace modules {

class SettingsWindow: public QDialog{
  Q_OBJECT
 public:
  explicit SettingsWindow( QWidget *parent = 0);
  virtual ~SettingsWindow() ;
  virtual void AddConfigurator(configuration::IConfigurator * configurator, std::string name);
 public slots:
  void accepted();
 private:
  Ui::SettingsWindow *ui;
  std::vector<configuration::IConfigurator *> configurators_;




};

}
}
}

#endif //GAGO_UI_SRC_MODULES_SETTINGS_SETTINGS_WINDOW_H_
