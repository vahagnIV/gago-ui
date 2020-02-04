#ifndef RECTIFIED_IMAGE_VIEW_WINDOW_H
#define RECTIFIED_IMAGE_VIEW_WINDOW_H

#include <QDialog>
#include <calibration_estimates.h>

namespace Ui {
class RectifiedImageViewWindow;
}

class RectifiedImageViewWindow : public QDialog {
 Q_OBJECT

 public:
  explicit RectifiedImageViewWindow(QWidget *parent = 0);
  void SetCalibrationEstimates(const gago::calibration::CalibrationEstimates & estimates,
                               const cv::Size & image_size,
                               const QList<QStringList> & files,
                               const QList<int> & valid_map);

  ~RectifiedImageViewWindow();
 signals:
  void ActiveBatchChanged(int batch_idx);
 public slots:
  void ShowImage(const QImage & image);
  void SetActiveBatch(int batch_idx);
 private slots:
  void NextButtonPressedSlot();
  void PreviousButtonPressedSlot();
 private:
  QList<cv::Mat> LoadFiles(int batch_idx);
  QImage CreateRectifiedImage(const QList<cv::Mat> & images);
  Ui::RectifiedImageViewWindow *ui;
  gago::calibration::CalibrationEstimates estimates_;
  QList<int> valid_batch_map_;
  QList<QStringList> files_;
  QList<int> valid_map_;
  int current_selected_idx_;

  cv::Mat rmap[2][2];
  cv::Mat R1, R2, P1, P2, Q;
  cv::Rect validRoi[2];
  cv::Size image_size;

};

#endif // RECTIFIED_IMAGE_VIEW_WINDOW_H
