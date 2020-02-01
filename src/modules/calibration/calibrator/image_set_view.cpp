//
// Created by vahagn on 2/1/20.
//

#include "image_set_view.h"
#include "thumb_show_item_model.h"
#include <QMenu>
#include <QListWidgetItem>

namespace gago {
namespace calibration {

ImageSetView::ImageSetView(QWidget *parent) : QListView(parent) {
  model_ = new gago::gui::calibration::ThumbShowItemModel();
  this->setModel(model_);
  this->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &ImageSetView::customContextMenuRequested, this, &ImageSetView::ShowContextMenu);
  connect(model_, &gago::gui::calibration::ThumbShowItemModel::RowsRemoved, this, &ImageSetView::RowsRemoved);
}

void ImageSetView::ShowContextMenu(const QPoint &pos) {
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

void ImageSetView::Append(const QImage &image) {
  model_->AppendThumbnail(image);
}

void ImageSetView::Replace(int idx, const QImage &new_image) {
  model_->ReplaceThumbnail(idx, new_image);

}

}
}