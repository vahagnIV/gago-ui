//
// Created by vahagn on 2/17/20.
//

#include <QApplication>
#include "main_module.h"
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  gago::gui::modules::MainModule mainModule;
  
  return 0;
}