//
// Created by vahagn on 2/16/20.
//

#include "menu_tree.h"

namespace gago {
namespace gui {
namespace modules {
MenuTreeNode *MenuTreeNode::GetChild(const QString &child_name) {
  return &children_[child_name];
}

bool MenuTreeNode::ContainsChild(const QString &child_name) const {
  return children_.contains(child_name);
}

void MenuTreeNode::Insert(const QString &key, QMenu *menu) {
  if (menu_)
    menu_->addMenu(menu);
  children_.insert(key, MenuTreeNode(menu)).value();
}

QAction *MenuTreeNode::GetAction(const QString &child_name) {
  return actions_.contains(child_name) ? actions_[child_name] : nullptr;
}
void MenuTreeNode::InsertAction(const QString &child_name, QAction *action) {
  menu_->addAction(action);
  actions_.insert(child_name, action);
}
bool MenuTreeNode::ContainsAction(const QString &child_name) const {
  return actions_.contains(child_name);
}

QList<QAction *> MenuTreeNode::AllActions() const {
  return actions_.values();
}

}
}
}