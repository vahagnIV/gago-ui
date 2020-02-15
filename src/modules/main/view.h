//
// Created by vahagn on 1/13/20.
//

#ifndef GAGO_UI_VIEW_H
#define GAGO_UI_VIEW_H

#include <QWidget>
#include <QString>

namespace gago {
namespace gui {
namespace modules {

class View {
 public:
  View() = default;
  virtual void StartDrawing(QWidget *widget) = 0;
  virtual void StopDrawing() = 0;
  virtual const QString & GetViewName() const = 0;
  virtual ~View() = default;
};

}
}
}

#endif //GAGO_UI_VIEW_H
