//
// Created by vahagn on 12/29/19.
//

#ifndef GAGO_UI_SRC_MODULES_MAIN_MAIN_WINDOW_H_
#define GAGO_UI_SRC_MODULES_MAIN_MAIN_WINDOW_H_


#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace gago {
namespace gui {
namespace modules {

class MainWindow : public QMainWindow {

 Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow();
 private:
  Ui::MainWindow *ui;

};

}}}
#endif //GAGO_UI_SRC_MODULES_MAIN_MAIN_WINDOW_H_
