//
// Created by vahagn on 2/1/20.
//

#include <QMenu>
#include <QDebug>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <QPainter>
#include <QtWidgets/QColormap>
#include "image_set_view.h"

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
    PatternBatch & res = parameters_[selectedIndexes()[i].row()];
    // Get curent item on selected row
    for (const Pattern & params: parameters_[this->selectedIndexes()[i].row()].pattern_params) {
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

void ImageSetView::Append(const PatternBatch & result) {
  parameters_.append(result);
  QList<QImage> images = GetImages(parameters_.back());
  QList<QStandardItem *> items;
  for (int cam_idx = 0; cam_idx < images.size(); ++cam_idx) {
    QImage & image = images[cam_idx];
    QPixmap pixmap = QPixmap::fromImage(image);
    QStandardItem *item = new QStandardItem();
    item->setIcon(pixmap);
    item->setCheckable(true);
    item->setCheckState(result.pattern_params[cam_idx].enabled ? Qt::Checked : Qt::Unchecked);
    items.append(item);
    setColumnWidth(cam_idx, 130);
  }
  model_->appendRow(items);

  int height = (int) std::round(100. / images[0].width() * images[0].height());
  setRowHeight(model_->rowCount() - 1, height);
  setIconSize(QSize(130, height - 10));
}

void ImageSetView::Append(const QStringList & filenames) {
  PatternBatch batch(filenames);
  Append(batch);
}

QList<QImage> ImageSetView::GetImages(const PatternBatch & image_batch) {
  QList<QImage> images;
  for (const Pattern & pattern_params: image_batch.pattern_params)
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
    PatternBatch & image_batch = parameters_[batch_idx];
    if (PES_Unestimated == image_batch.state)
      continue;

    QColor background_color = PES_Broken == image_batch.state ? QColor(128, 128, 128) : GetColor(image_batch.rms);

    QList<QImage> images = GetImages(image_batch);

    for (int cam_idx = 0; cam_idx < image_batch.pattern_params.size(); ++cam_idx) {
      Pattern & pattern = image_batch.pattern_params[cam_idx];
      QStandardItem *item = model_->itemFromIndex(model_->index(batch_idx, cam_idx));

      model_->setData(model_->index(batch_idx, cam_idx), background_color, Qt::BackgroundRole);
      QImage & image = images[cam_idx];

      QColor color = PES_Broken == pattern.state ?
                     QColor(128, 128, 128) : GetColor(pattern.reprojection_error);

      QPainter painter(&image);
      QPen pen(color, 50);
      painter.setPen(pen);
      painter.drawLine(0, 0, 0, image.height());
      item->setIcon(QPixmap::fromImage(image));
      QString tool_tip;
      if (PES_Calibrated == image_batch.pattern_params[cam_idx].state) {
        tool_tip = QString::asprintf("Calibration error: %.4f",
                                     pattern.reprojection_error);

      } else {
        tool_tip = "Not calibrated: " + to_string(pattern.state);
      }
      if (image_batch.pattern_params.size() == 2)
        if (PES_Calibrated == image_batch.state)
          tool_tip += QString::asprintf("\nStereo Calibration Error: %.4f", image_batch.rms);
        else
          tool_tip += "\nStereo not calibrated: " + to_string(image_batch.state);
      if (!pattern.enabled)
        tool_tip += "\nNot included in the calibration process.";
      item->setToolTip(tool_tip);
    }
  }
}

void gago::calibration::ImageSetView::ItemChanged(QStandardItem *item) {
  int batch_idx = model_->indexFromItem(item).row();
  int column_idx = model_->indexFromItem(item).column();
  parameters_[batch_idx].pattern_params[column_idx].enabled = item->checkState() == Qt::Checked;
}

QList<PatternBatch> & ImageSetView::GetBatchCalibrationResults() {
  return parameters_;
}

const QList<PatternBatch> & ImageSetView::GetBatchCalibrationResults() const {
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