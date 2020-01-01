//
// Created by vahagn on 12/29/19.
//

#include "main_window.h"
#include "ui_main_window.h"
namespace gago {
namespace gui {
namespace modules {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  delete ui;
}
}
}
}