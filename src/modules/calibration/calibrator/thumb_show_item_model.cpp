#include "thumb_show_item_model.h"
#include <QPixmap>
namespace gago {
namespace gui {
namespace calibration {

ThumbShowItemModel::ThumbShowItemModel(const QDir & directory, const QStringList & cameras, QObject *parent)
    : QAbstractItemModel(parent),
      cameras_(cameras),
      directory_(directory) {
  directory_.setSorting(QDir::Name);
  for (int i = 0; i < cameras.size(); ++i) {
    cameras_[i] = cameras_[i] + "_*.jpg";
  }
  QStringList l = directory_.entryList({cameras_[0]}, QDir::NoFilter, QDir::Name);
  for (int j = 0; j < l.count(); ++j) {
    int index = l[j].right(7).left(3).toInt();
    for (int i = 1; i < cameras.size(); ++i) {
      bool all_exist = true;
      if (QFile::exists(directory_.filePath(cameras[i]))) {
        all_exist = false;
        break;
      }
    }
    //if(all_exi)
  }
}

QVariant ThumbShowItemModel::headerData(int section, Qt::Orientation orientation, int role) const {
  // FIXME: Implement me!
}

QModelIndex ThumbShowItemModel::index(int row, int column, const QModelIndex & parent) const {
  // FIXME: Implement me!
}

QModelIndex ThumbShowItemModel::parent(const QModelIndex & index) const {
  return QModelIndex();
  // FIXME: Implement me!
}

int ThumbShowItemModel::rowCount(const QModelIndex & parent) const {
  if (!parent.isValid())
    return 0;
  return images_.count();
}

int ThumbShowItemModel::columnCount(const QModelIndex & parent) const {
  if (!parent.isValid())
    return 0;
  return 1;
  // FIXME: Implement me!
}

QVariant ThumbShowItemModel::data(const QModelIndex & index, int role) const {
  if (!index.isValid())
    return QVariant();
  if (role == Qt::DecorationRole) {
    QVariant varinat;

    varinat.setValue(QPixmap::fromImage(images_[index.row()]));
    return varinat;
  }

  /* if(role == Qt::DisplayRole)
     return QImage()*/

  // FIXME: Implement me!
  return QVariant();
}

}
}
}