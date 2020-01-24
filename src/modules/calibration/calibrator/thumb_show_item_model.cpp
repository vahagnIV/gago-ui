#include "thumb_show_item_model.h"
#include <QPixmap>
#include <QtGui/QPainter>
namespace gago {
namespace gui {
namespace calibration {

ThumbShowItemModel::ThumbShowItemModel(const QDir &directory, const QStringList &cameras, QObject *parent)
    : QAbstractItemModel(parent),
      cameras_(cameras),
      directory_(directory) {
  directory_.setSorting(QDir::Name);

  /*for (int i = 0; i < cameras.size(); ++i) {
    cameras_[i] = cameras_[i] + "_*.jpg";
  }*/
  QStringList filter = {cameras[0] + "_*.jpg"};
  QStringList first_camera_files = directory_.entryList(filter, QDir::NoFilter, QDir::Name);

  for (int j = 0; j < first_camera_files.size(); ++j) {

    int index = first_camera_files[j].right(7).left(3).toInt();
    QStringList filenames = {first_camera_files[j]};

    for (int i = 1; i < cameras.size(); ++i) {
      QString filename = QString("%1_%2.jpg").arg(cameras[i]).arg(QString::number(index).rightJustified(3, '0'));
      if (QFile::exists(directory_.filePath(filename))) {
        filenames.append(filename);
      }
    }
    if (filenames.count() != cameras.size())
      continue;

    QList<QImage> images;
    int total_length = 0;
    int max_height = 0;
    for (int k = 0; k < filenames.size(); ++k) {
      QImage image(directory.filePath(filenames[k]));
      total_length += image.width();
      max_height = std::max(max_height, image.height());
      images.append(image);
    }

    QImage thumbnail(total_length, max_height, QImage::Format_RGB888);
    QPainter painter;
    int offset = 0;
    painter.begin(&thumbnail);
    for (const QImage &image: images) {
      painter.drawImage(offset, 0, image);
      offset += image.width();
    }
    painter.end();

    images_.append(thumbnail.scaledToWidth(150, Qt::TransformationMode::SmoothTransformation));
  }
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

  /* if(role == Qt::DisplayRole)
     return QImage()*/

  // FIXME: Implement me!
  return QVariant();
}

}
}
}