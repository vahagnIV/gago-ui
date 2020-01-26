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
  explicit ThumbShowItemModel(const QDir &directory, const QStringList &cameras, QObject *parent = nullptr);

  bool AppendByIndex(int idx,  char * format);
  bool AppendByFilenames(const QStringList &filenames);
  // Header:
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

  // Basic functionality:
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;

  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

 private:

  QStringList GetFilenames(int idx, char * format);
  QList<QImage> GetImages(const QStringList &filenames);
  void GetTotalWidthMaxHeight(const QList<QImage> &images, int &out_total_width, int &out_max_height);
  QImage GetThumbnail(const QList<QImage> &images, int max_width);

  QList<QImage> images_;
  QDir directory_;
  QStringList cameras_;
};

}
}
}
#endif // THUMBSHOWITEMMODEL_H