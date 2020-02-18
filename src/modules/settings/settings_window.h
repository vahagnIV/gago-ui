//
// Created by vahagn on 12/30/19.
//

#ifndef GAGO_UI_SRC_MODULES_SETTINGS_SETTINGS_WINDOW_H_
#define GAGO_UI_SRC_MODULES_SETTINGS_SETTINGS_WINDOW_H_

#include <QDialog>
#include <QAbstractButton>
#include <QMainWindow>
#include "iconfigurator.h"

namespace Ui {
class SettingsWindow;
}

namespace gago {
namespace gui {

namespace modules {
class SettingsModule;
}

namespace modules {

class SettingsWindow : public QDialog {
  friend class modules::SettingsModule;
 Q_OBJECT
 public:
  explicit SettingsWindow(QList<configuration::IConfigurator *> & configurators, QMainWindow *parent = 0);
  virtual ~SettingsWindow();
 public slots:
  void clicked(QAbstractButton *btn);
 signals:
  void applied();
 private:
  void apply();
  Ui::SettingsWindow *ui;
  QList<configuration::IConfigurator *> configurators_;
};

}
}
}

#endif //GAGO_UI_SRC_MODULES_SETTINGS_SETTINGS_WINDOW_H_
