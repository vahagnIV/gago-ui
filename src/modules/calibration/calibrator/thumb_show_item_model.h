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
  explicit ThumbShowItemModel(const QDir & directory, const QStringList & cameras, QObject *parent = nullptr);

  // Header:
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  // Basic functionality:
  QModelIndex index(int row, int column,
                    const QModelIndex & parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex & index) const override;

  int rowCount(const QModelIndex & parent = QModelIndex()) const override;
  int columnCount(const QModelIndex & parent = QModelIndex()) const override;

  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

 private:
  QList<QImage> images_;
  QDir directory_;
  QStringList cameras_;
};

}
}
}
#endif // THUMBSHOWITEMMODEL_H