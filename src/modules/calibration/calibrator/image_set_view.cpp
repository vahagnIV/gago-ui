//
// Created by vahagn on 2/1/20.
//

#include "image_set_view.h"
#include <QMenu>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <QPainter>
#include <QtWidgets/QColormap>

namespace gago {
namespace calibration {

ImageSetView::ImageSetView(QWidget *parent) : QTableView(parent) {

  rectifiedImageViewWindow_ = new RectifiedImageViewWindow(this);
  rectifiedImageViewWindow_->hide();

  model_ = new QStandardItemModel();

  connect(model_, &QStandardItemModel::itemChanged, this, &ImageSetView::ItemChanged);

  this->setModel(model_);
  this->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &ImageSetView::customContextMenuRequested, this, &ImageSetView::ShowContextMenu);

  connect(rectifiedImageViewWindow_,
          &RectifiedImageViewWindow::ActiveBatchChanged,
          this,
          &ImageSetView::ActiveBatchChanges);
}

void ImageSetView::ShowContextMenu(const QPoint & pos) {
  QPoint globalPos = this->mapToGlobal(pos);

  // Create menu and insert some actions
  QMenu myMenu;
  QAction *show_rectified__action = myMenu.addAction("Show Rectified", this, &ImageSetView::ShowRectified);
  show_rectified__action->setIcon(QIcon::fromTheme("preview-file"));

  show_rectified__action->setEnabled(rectifiedImageViewWindow_->CanShow());

  myMenu.addSeparator();
  myMenu.addAction("Delete row", this, &ImageSetView::DeleteItem)->setIcon(QIcon::fromTheme("remove"));;

  // Show context menu at handling position
  myMenu.exec(globalPos);
}

void ImageSetView::DeleteItem() {
  for (int i = 0; i < this->selectedIndexes().size(); ++i) {
    BatchCalibrationResult & res = parameters_[selectedIndexes()[i].row()];
    // Get curent item on selected row
    for (const PatternEstimationParameters & params: parameters_[this->selectedIndexes()[i].row()].pattern_params) {
      std::string mm = params.filename.toStdString();
      if (QFile::exists(params.filename))
        QFile::remove(params.filename);
    }
    parameters_.removeAt(this->selectedIndexes()[i].row());
    this->model()->removeRow(this->selectedIndexes()[i].row());
  }
}

ImageSetView::~ImageSetView() {
  delete rectifiedImageViewWindow_;
}

void ImageSetView::ShowRectified() {
  rectifiedImageViewWindow_->SetActiveBatch(selectedIndexes().empty() ? 0 : selectedIndexes()[0].row());
  rectifiedImageViewWindow_->exec();
}

void ImageSetView::Append(const QStringList & filenames) {
  parameters_.append(BatchCalibrationResult(filenames));
  QList<QImage> images = GetImages(parameters_.back());
  QList<QStandardItem *> items;
  for (int cam_idx = 0; cam_idx < images.size(); ++cam_idx) {
    QImage & image = images[cam_idx];
    QPixmap pixmap = QPixmap::fromImage(image);
    QStandardItem *item = new QStandardItem();
    item->setIcon(pixmap);
    item->setCheckable(true);
    item->setCheckState(Qt::CheckState::Checked);
    items.append(item);
    setColumnWidth(cam_idx, 130);
  }
  model_->appendRow(items);

  int height = (int) std::round(100. / images[0].width() * images[0].height());
  setRowHeight(model_->rowCount() - 1, height);
  setIconSize(QSize(130, height - 10));

}

QList<QImage> ImageSetView::GetImages(const BatchCalibrationResult & image_batch) {
  QList<QImage> images;
  for (const PatternEstimationParameters & pattern_params: image_batch.pattern_params)
    images.append(QImage(pattern_params.filename));
  return images;
}

QColor ImageSetView::GetColor(float rms) {
  if (rms < 0.5)
    return QColor(0, 255, 0);
  else if (rms < 1)
    return QColor(0, 0, 255);
  else
    return QColor(255, 0, 0);
}

void ImageSetView::Update() {
  for (int batch_idx = 0; batch_idx < parameters_.size(); ++batch_idx) {
    BatchCalibrationResult & result = parameters_[batch_idx];
    QColor backgroun_color = GetColor(result.rms);

    QList<QImage> images = GetImages(result);
    for (int cam_idx = 0; cam_idx < result.pattern_params.size(); ++cam_idx) {
      if (result.state == PES_Calibrated)
        model_->setData(model_->index(batch_idx, cam_idx), backgroun_color, Qt::BackgroundRole);
      else
        model_->setData(model_->index(batch_idx, cam_idx), QVariant(), Qt::BackgroundRole);
      QStandardItem *item = model_->itemFromIndex(model_->index(batch_idx, cam_idx));
      QImage & image = images[cam_idx];

      QColor color = result.pattern_params[cam_idx].state == PES_Calibrated
                     ? GetColor(result.pattern_params[cam_idx].reprojection_error) : QColor(128, 128, 128);

      QPainter painter(&image);
      QPen pen(color, 50);
      painter.setPen(pen);
      painter.drawRect(0, 0, image.width(), image.height());
      if (PES_Calibrated == result.pattern_params[cam_idx].state) {
        QString tool_tip = QString::asprintf("Calibration error: %.4f",
                                             result.pattern_params[cam_idx].reprojection_error);
        if (result.pattern_params.size() == 2)
          tool_tip += QString::asprintf("\nStereo Calibration Error: %.4f", result.rms);

        item->setToolTip(tool_tip);
        item->setIcon(QPixmap::fromImage(image));
      }
      else
      {
        item->setToolTip("");
      }
    }
  }
}

void gago::calibration::ImageSetView::ItemChanged(QStandardItem *item) {
  int batch_idx = model_->indexFromItem(item).row();
  int column_idx = model_->indexFromItem(item).column();
  if (item->checkState() != Qt::CheckState::Checked)
    parameters_[batch_idx].pattern_params[column_idx].state = PES_Disabled;
}

QList<BatchCalibrationResult> & ImageSetView::GetBatchCalibrationResults() {
  return parameters_;
}

const QList<BatchCalibrationResult> & ImageSetView::GetBatchCalibrationResults() const {
  return parameters_;
}

void ImageSetView::ActiveBatchChanges(int batch_idx) {
  setCurrentIndex(model()->index(batch_idx, 0));
}

void ImageSetView::SetCalibrationEstimates(const gago::calibration::CalibrationEstimates & estimates) {
  rectifiedImageViewWindow_->SetCalibrationEstimates(estimates, GetBatchCalibrationResults());
}

void ImageSetView::SetCameraNames(const QStringList & cam_names) {
  model_->setHorizontalHeaderLabels(cam_names);
}

}
}