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
}

void VideoPlayer::ShowImage(const cv::Mat & image) {

  QSize size = this->size();
  float scale = 1;

  scale = std::min(scale, (float) size.width() / image.cols);

  scale = std::min(scale, (float) size.height() / image.rows);
  qimage_ =
      QImage(image.data, image.cols, image.rows, image.cols * image.channels(), QImage::Format_RGB888);

  qimage_ = qimage_.scaled(scale * qimage_.width(), scale * qimage_.height());

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

void VideoPlayer::setText(const std::string & text) {
  text_ = QString::fromStdString(text);
}

}
}
}