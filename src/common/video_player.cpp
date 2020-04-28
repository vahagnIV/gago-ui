//
// Created by vahagn on 1/13/20.
//

#include "video_player.h"
#include <QPainter>

namespace gago {
namespace gui {
namespace common {

VideoPlayer::VideoPlayer() : QLabel("") {
  this->setMinimumSize(1, 1);

  qRegisterMetaType<cv::Mat>("cv::Mat");

  connect(this, &VideoPlayer::NewImageReceived, this, &VideoPlayer::DrawImage);
}

void VideoPlayer::ShowImage(const cv::Mat & image) {
  emit NewImageReceived(image);
}

void VideoPlayer::setText(const std::string & text) {
  text_ = QString::fromStdString(text);
}

void VideoPlayer::DrawImage(const cv::Mat & image) {
  QSize size = this->size();
  float scale = 1;

  scale = std::min(scale, (float) size.width() / image.cols);
  scale = std::min(scale, (float) size.height() / image.rows);
  qimage_ =
      QImage(image.data, image.cols, image.rows, image.cols * image.channels(), QImage::Format_RGB888);

  qimage_ = qimage_.scaled(scale * qimage_.width(), scale * qimage_.height()).copy();

  QPainter painter;
  if (!painter.begin(&qimage_)) return;
  for (int i = 16; i < qimage_.height(); i += 16) {
    painter.setPen(QPen(Qt::green));
    painter.drawLine(QPoint(0, i), QPoint(qimage_.width(), i));
  }
  painter.end();

  if (!text_.isEmpty()) {
    QPainter p;
    if (!p.begin(&qimage_)) return;

    p.setPen(QPen(Qt::yellow));
    p.setFont(QFont("Times", 12, QFont::Bold));
    p.drawText(qimage_.rect(), Qt::AlignCenter, text_);
    p.end();
  }

  setPixmap(QPixmap::fromImage(qimage_));
}

}
}
}