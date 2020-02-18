//
// Created by vahagn on 1/13/20.
//

#ifndef GAGO_UI_VIDEO_PLAYER_H
#define GAGO_UI_VIDEO_PLAYER_H

#include <QLabel>
#include <opencv2/opencv.hpp>

namespace gago {
namespace gui {
namespace common {

class VideoPlayer : public QLabel {
  Q_OBJECT
 public:
  VideoPlayer();
  void setText(const std::string & text);
  void ShowImage(const cv::Mat & image);
 signals:
  void NewImageReceived(const cv::Mat & image);

 public slots:
  void DrawImage(const cv::Mat & image);
 private:
  QImage qimage_;
  QString text_;
};

}
}
}

#endif //GAGO_UI_VIDEO_PLAYER_H
