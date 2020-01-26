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

bool ThumbShowItemModel::AppendByIndex(int idx, char *format) {
  QStringList filenames = GetFilenames(idx, format);
  return AppendByFilenames(filenames);
}

QStringList ThumbShowItemModel::GetFilenames(int idx, char *format) {
  QStringList filenames;
  for (const QString &cam_forma: cameras_) {
    QString filename = QString::asprintf(format, cam_forma.toStdString().c_str(), idx);
    if (!directory_.exists(filename))
      break;
    else
      filenames.append(filename);
  }
  return filenames;
}

QList<QImage> ThumbShowItemModel::GetImages(const QStringList &filenames) {
  QList<QImage> images;
  for (const QString &filename: filenames)
    images.append(QImage(directory_.filePath(filename)));
  return images;
}

void ThumbShowItemModel::GetTotalWidthMaxHeight(const QList<QImage> &images,
                                                int &out_total_width,
                                                int &out_max_height) {
  out_max_height = 0;
  out_total_width = 0;
  for (const QImage &image: images) {
    out_max_height = std::max(image.height(), out_max_height);
    out_total_width += image.width();
  }

}

QImage ThumbShowItemModel::GetThumbnail(const QList<QImage> &images, int max_width) {
  int total_width, max_height;
  GetTotalWidthMaxHeight(images, total_width, max_height);

  QImage thumbnail(total_width, max_height, QImage::Format_RGB888);
  QPainter painter;
  int offset = 0;
  painter.begin(&thumbnail);
  for (const QImage &image: images) {
    painter.drawImage(offset, 0, image);
    offset += image.width();
  }
  painter.end();
  return thumbnail.scaledToWidth(max_width);
}

bool ThumbShowItemModel::AppendByFilenames(const QStringList &filenames) {
  if (cameras_.size() != filenames.size())
    return false;
  for (const QString &filename: filenames)
    if (!directory_.exists(filename))
      return false;

  QList<QImage> images = GetImages(filenames);
  QImage thumbnail = GetThumbnail(images, 150);
  beginInsertRows(QModelIndex(), images_.size(), images_.size() + 1);
  images_.append(thumbnail);
  endInsertRows();
  return true;
}

}
}
}