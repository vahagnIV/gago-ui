//
// Created by vahagn on 2/1/20.
//

#include "image_set_view.h"
#include "thumb_show_item_model.h"
#include <QMenu>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <QPainter>
#include <QtWidgets/QColormap>

namespace gago {
namespace calibration {

ImageSetView::ImageSetView(QWidget *parent) : QListView(parent) {
//  model_ = new gago::gui::calibration::ThumbShowItemModel();
  setResizeMode(QListView::Adjust);
  setFlow(TopToBottom);
  setWordWrap(true);
  setUniformItemSizes(false);
  setIconSize(QSize(160, 200));
  model_ = new QStandardItemModel();
  model_->setColumnCount(1);

  this->setModelColumn(0);

  this->setModel(model_);
  this->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &ImageSetView::customContextMenuRequested, this, &ImageSetView::ShowContextMenu);
  //connect(model_, &gago::gui::calibration::ThumbShowItemModel::RowsRemoved, this, &ImageSetView::RowsRemoved);
}

void ImageSetView::ShowContextMenu(const QPoint &pos) {
  QPoint globalPos = this->mapToGlobal(pos);

  // Create menu and insert some actions
  QMenu myMenu;
  myMenu.addAction("Properties", this, &ImageSetView::ShowProperties)->setIcon(QIcon::fromTheme("properties"));
  myMenu.addSeparator();
  myMenu.addAction("Erase", this, &ImageSetView::DeleteItem)->setIcon(QIcon::fromTheme("remove"));;

  // Show context menu at handling position
  myMenu.exec(globalPos);
}

void ImageSetView::DeleteItem() {
  for (int i = 0; i < this->selectedIndexes().size(); ++i) {
    BatchCalibrationResult &res = parameters_[selectedIndexes()[i].row()];
    // Get curent item on selected row
    for (const PatternEstimationParameters &params: parameters_[this->selectedIndexes()[i].row()].pattern_params) {
      std::string mm = params.filename.toStdString();
      if (QFile::exists(params.filename))
        QFile::remove(params.filename);
    }
    parameters_.removeAt(this->selectedIndexes()[i].row());
    this->model()->removeRow(this->selectedIndexes()[i].row());
  }
}

void ImageSetView::ShowProperties() {
  if (this->selectedIndexes().size())
    ProperiesShowRequested(this->selectedIndexes()[0].row());
}

void ImageSetView::Append(const QImage &image) {

  QStandardItem *item = new QStandardItem();
//  item->setSizeHint(QSize(200, 100));
  QPixmap pm = QPixmap::fromImage(image);
  item->setIcon(pm);
  item->setCheckable(true);
  item->setText("");
  QFont f;
  f.setPointSize(1);
  item->setFont(f);
  item->setCheckState(Qt::CheckState::Checked);
  item->setEditable(false);
  model_->appendRow(item);
}

void ImageSetView::Replace(int idx, const QImage &new_image) {
  //model_->ReplaceThumbnail(idx, new_image);

}

void ImageSetView::Append(const QStringList &filenames, bool use) {
  parameters_.append(BatchCalibrationResult(filenames));
  parameters_.back().enabled = use;
  QList < QImage > images = GetImages(parameters_.back());

  QImage icon = GetThumbnail(images, 300);
  QPixmap pm = QPixmap::fromImage(icon);
  QStandardItem *item = new QStandardItem();
  item->setIcon(pm);
  item->setCheckable(true);
  item->setText("");
  QFont f;
  f.setPointSize(1);
  item->setFont(f);
  item->setCheckState(use ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
  item->setEditable(false);
  model_->appendRow(item);
}

QList<QImage> ImageSetView::GetImages(const QStringList &image_paths) {
  QList < QImage > images;
  for (const QString &image_path: image_paths)
    images.append(QImage(image_path));
  return images;
}

QList<QImage> ImageSetView::GetImages(const BatchCalibrationResult &image_batch) {
  QList < QImage > images;
  for (const PatternEstimationParameters &pattern_params: image_batch.pattern_params)
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
    QStandardItem *item = model_->itemFromIndex(model_->index(batch_idx, 0));
    BatchCalibrationResult &result = parameters_[batch_idx];
    QList < QImage > images = GetImages(result);

    for (int cam_idx = 0; cam_idx < images.size(); ++cam_idx) {
      QColor color = GetColor(result.pattern_params[cam_idx].reprojection_error);
      QImage &image = images[cam_idx];
      QPainter painter(&image);
      QPen pen(color, 50);
      painter.setPen(pen);
      painter.drawRect(0, 0, image.width(), image.height());
    }
    item->setIcon(QPixmap::fromImage(GetThumbnail(images, 300)));
  }
}

QImage ImageSetView::GetThumbnail(const QList<QImage> &images, int max_width) {

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

void ImageSetView::GetTotalWidthMaxHeight(const QList<QImage> &images,
                                          int &out_total_width,
                                          int &out_max_height) {
  out_max_height = 0;
  out_total_width = 0;
  for (const QImage &image: images) {
    out_max_height = std::max(image.height(), out_max_height);
    out_total_width += image.width();
  }
}

void ImageSetView::GetAllowedList(QList<BatchCalibrationResult *> &out_list) {
  for (int i = 0; i < model_->rowCount(); ++i) {
    if (model_->item(i, 0)->checkState() == Qt::CheckState::Checked)
      out_list.append(&parameters_[i]);
  }
}

void gago::calibration::ImageSetView::ItemChanged(QStandardItem *item) {
  int idx = model_->indexFromItem(item).row();
  parameters_[idx].enabled = item->checkState() == Qt::CheckState::Checked;
}

QList<BatchCalibrationResult> &ImageSetView::GetBatchCalibrationResults() {
  return parameters_;
}

const QList<BatchCalibrationResult> &ImageSetView::GetBatchCalibrationResults() const {
  return parameters_;
}

}
}