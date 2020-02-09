//
// Created by vahagn on 2/1/20.
//

#ifndef GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_IMAGE_SET_VIEW_H_
#define GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_IMAGE_SET_VIEW_H_
#include <QListView>
#include <QDir>
#include <QStandardItemModel>
#include <QColor>

#include "batch_calibration_result.h"
namespace gago {
namespace calibration {

class ImageSetView : public QListView {
 Q_OBJECT
 public:
  ImageSetView(QWidget *parent = nullptr);
  void Append(const QImage &image);
  void Replace(int idx, const QImage & new_image);
  void Append(const QStringList & filenames, bool use);
  void GetAllowedList(QList<BatchCalibrationResult *> & out_list);
  QList<BatchCalibrationResult> & GetBatchCalibrationResults();
  const  QList<BatchCalibrationResult> & GetBatchCalibrationResults() const;
  void Update();
 public slots:
  void ShowContextMenu(const QPoint &pos);
  void DeleteItem();
 private slots:
  void ShowProperties();
  void ItemChanged(QStandardItem *item);
 signals:
  void ProperiesShowRequested(int batch_idx);
 private:
  QImage GetThumbnail(const QList<QImage> & images, int max_width);
  QList<QImage> GetImages(const QStringList & image_paths);
  QList<QImage> GetImages(const BatchCalibrationResult &image_batch);
  void GetTotalWidthMaxHeight(const QList<QImage> & images, int & out_total_width, int & out_max_height);
  QColor GetColor(float rms);

  QStandardItemModel *model_;
  QList<BatchCalibrationResult> parameters_;


};

}
}

#endif //GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_IMAGE_SET_VIEW_H_
