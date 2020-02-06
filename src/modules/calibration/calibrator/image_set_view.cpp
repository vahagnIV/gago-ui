//
// Created by vahagn on 2/1/20.
//

#include "image_set_view.h"
#include "thumb_show_item_model.h"
#include <QMenu>
#include <QListWidgetItem>
#include <QStandardItemModel>
#include <QPainter>

namespace gago {
namespace calibration {

ImageSetView::ImageSetView(QWidget *parent) : QListView(parent) {
//  model_ = new gago::gui::calibration::ThumbShowItemModel();
  setResizeMode(QListView::Adjust);
  setFlow(TopToBottom);
  setWordWrap(true);
  setUniformItemSizes(false);
  setIconSize(QSize(180, 200));
  model_ = new QStandardItemModel();
  model_->setColumnCount(1);

  this->setModelColumn(0);

  this->setModel(model_);
  this->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &ImageSetView::customContextMenuRequested, this, &ImageSetView::ShowContextMenu);
  //connect(model_, &gago::gui::calibration::ThumbShowItemModel::RowsRemoved, this, &ImageSetView::RowsRemoved);
}

void ImageSetView::ShowContextMenu(const QPoint & pos) {
  QPoint globalPos = this->mapToGlobal(pos);

  // Create menu and insert some actions
  QMenu myMenu;
  myMenu.addAction("Properties", this, &ImageSetView::ShowProperties);
  myMenu.addAction("Erase", this, &ImageSetView::DeleteItem);

  // Show context menu at handling position
  myMenu.exec(globalPos);
}

void ImageSetView::DeleteItem() {
  for (int i = 0; i < this->selectedIndexes().size(); ++i) {
    // Get curent item on selected row
    this->model()->removeRow(this->selectedIndexes()[i].row());
  }
}

void ImageSetView::RowsRemoved(int row_idx, int count) {
  BatchRemoved(row_idx, count);
}

void ImageSetView::ShowProperties() {
  if (this->selectedIndexes().size())
    ProperiesShowRequested(this->selectedIndexes()[0].row());
}

void ImageSetView::Append(const QImage & image) {

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

void ImageSetView::Replace(int idx, const QImage & new_image) {
  //model_->ReplaceThumbnail(idx, new_image);

}

void ImageSetView::Append(const QStringList & filenames, bool use) {
  QImage icon = GetThumbnail(filenames, 300);
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
  parameters_.append(BatchCalibrationResult(filenames));
  parameters_.back().enabled = use;
}

QImage ImageSetView::GetThumbnail(const QStringList & image_paths, int max_width) {
  QList<QImage> images;
  for (const QString & image_path: image_paths)
    images.append(QImage(image_path));

  int total_width, max_height;
  GetTotalWidthMaxHeight(images, total_width, max_height);

  QImage thumbnail(total_width, max_height, QImage::Format_RGB888);
  QPainter painter;
  int offset = 0;
  painter.begin(&thumbnail);
  for (const QImage & image: images) {
    painter.drawImage(offset, 0, image);
    offset += image.width();
  }
  painter.end();
  return thumbnail.scaledToWidth(max_width);
}

void ImageSetView::GetTotalWidthMaxHeight(const QList<QImage> & images,
                                          int & out_total_width,
                                          int & out_max_height) {
  out_max_height = 0;
  out_total_width = 0;
  for (const QImage & image: images) {
    out_max_height = std::max(image.height(), out_max_height);
    out_total_width += image.width();
  }
}

void ImageSetView::GetAllowedList(QList<BatchCalibrationResult *> & out_list) {
  for (int i = 0; i < model_->rowCount(); ++i) {
    if (model_->item(i, 0)->checkState() == Qt::CheckState::Checked)
      out_list.append(&parameters_[i]);
  }
}

void gago::calibration::ImageSetView::ItemChanged(QStandardItem *item) {
  int idx = model_->indexFromItem(item).row();
  parameters_[idx].enabled = item->checkState() == Qt::CheckState::Checked;
}

}
}