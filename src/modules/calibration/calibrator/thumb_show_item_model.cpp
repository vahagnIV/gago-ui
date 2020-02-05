#include "thumb_show_item_model.h"
#include <QPixmap>
#include <QtGui/QPainter>
namespace gago {
namespace gui {
namespace calibration {

ThumbShowItemModel::ThumbShowItemModel(QObject *parent)
    : QAbstractItemModel(parent) {
}

QVariant ThumbShowItemModel::headerData(int section, Qt::Orientation orientation, int role) const {
  // FIXME: Implement me!
}

QModelIndex ThumbShowItemModel::index(int row, int column, const QModelIndex &parent) const {
  return createIndex(row, column);
  // FIXME: Implement me!
}

QModelIndex ThumbShowItemModel::parent(const QModelIndex &index) const {
  return QModelIndex();
  // FIXME: Implement me!
}

int ThumbShowItemModel::rowCount(const QModelIndex &parent) const {
  /*if (!parent.isValid())
    return 0;*/
  return images_.count();
}

int ThumbShowItemModel::columnCount(const QModelIndex &parent) const {
  /*if (!parent.isValid())
    return 0;*/
  return 1;
  // FIXME: Implement me!
}

QVariant ThumbShowItemModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();
  if (role == Qt::DecorationRole) {
    QVariant varinat;

    varinat.setValue(QPixmap::fromImage(images_[index.row()]));
    return varinat;
  }
  return QVariant();
}

bool ThumbShowItemModel::removeRows(int row, int count, const QModelIndex &parent) {
  if (row < 0 || row > images_.size())
    return false;
  beginRemoveRows(parent, row, row + count);
  images_.removeAt(row);
  endRemoveRows();
  emit RowsRemoved(row, count);
  return true;
}

void ThumbShowItemModel::AppendThumbnail(const QImage &thumbnail) {
  beginInsertRows(QModelIndex(), images_.size(), images_.size() + 1);
  images_.append(thumbnail);

  endInsertRows();
}

void ThumbShowItemModel::ReplaceThumbnail(int idx, const QImage &thumbnail) {
  images_[idx] = thumbnail;
}

}
}
}