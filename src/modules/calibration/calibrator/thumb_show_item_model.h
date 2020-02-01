#ifndef THUMBSHOWITEMMODEL_H
#define THUMBSHOWITEMMODEL_H

#include <QAbstractItemModel>
#include <QImage>
#include <QDir>

namespace gago {
namespace gui {
namespace calibration {

class ThumbShowItemModel : public QAbstractItemModel {
 Q_OBJECT

 public:
  explicit ThumbShowItemModel(QObject *parent = nullptr);

  void AppendThumbnail(const QImage & thumbnail);
  void ReplaceThumbnail(int idx, const QImage & thumbnail);
  // Header:
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  // Basic functionality:
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override ;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

 signals:
  void RowsRemoved(int start, int count);

 private:



  QList<QImage> images_;
};

}
}
}
#endif // THUMBSHOWITEMMODEL_H