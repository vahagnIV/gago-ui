//
// Created by vahagn on 2/1/20.
//

#ifndef GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_IMAGE_SET_VIEW_H_
#define GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_IMAGE_SET_VIEW_H_
#include <QTableView>
#include <QDir>
#include <QStandardItemModel>
#include <QColor>

#include "batch_calibration_result.h"
#include "rectified_image_view_window.h"
namespace gago {
namespace calibration {

class ImageSetView : public QTableView {
 Q_OBJECT
 public:
  ImageSetView(QWidget *parent = nullptr);
  QList<PatternBatch> & GetBatchCalibrationResults();
  const  QList<PatternBatch> & GetBatchCalibrationResults() const;
  void ActiveBatchChanges(int batch_idx);
  void Update();
  virtual ~ImageSetView();
  void SetCalibrationEstimates(const gago::calibration::CalibrationEstimates & estimates);
  void SetCameraNames(const QStringList & cam_names);
 public slots:
  void Append(const QStringList & filenames);
  void Append(const PatternBatch & result);
  void ShowContextMenu(const QPoint &pos);
  void DeleteItem();
 private slots:
  void ShowRectified();
  void ItemChanged(QStandardItem *item);
 private:
  QList<QImage> GetImages(const PatternBatch &image_batch);
  QColor GetColor(float rms);

  QStandardItemModel *model_;
  QList<PatternBatch> parameters_;
  RectifiedImageViewWindow *rectifiedImageViewWindow_;


};

}
}

#endif //GAGO_UI_SRC_MODULES_CALIBRATION_CALIBRATOR_IMAGE_SET_VIEW_H_
