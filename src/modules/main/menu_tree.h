//
// Created by vahagn on 2/16/20.
//

#ifndef PROJECT_MENU_TREE_H
#define PROJECT_MENU_TREE_H

#include <QString>
#include <QMenu>
#include <QAction>
namespace gago {
namespace gui {
namespace modules {

class MenuTreeNode {
 public:
  MenuTreeNode(QMenu *menu) : menu_(menu){}
  MenuTreeNode() : menu_(nullptr) {}

  MenuTreeNode * GetChild(const QString &child_name);
  QAction * GetAction(const QString &child_name);
  void InsertAction(const QString &child_name, QAction * action);
  QList<QAction *> AllActions() const;

  QMap<QString, MenuTreeNode> &Children() { return children_; };
  bool ContainsChild(const QString &child_name) const ;
  bool ContainsAction(const QString &child_name) const ;
  QMenu *Menu() const { return menu_; }
  void Insert(const QString &key, QMenu * menu);

 private:
  QMenu *menu_;
  QMap<QString, MenuTreeNode> children_;
  QMap<QString, QAction *> actions_;

};
}
}
}

#endif //PROJECT_MENU_TREE_H
