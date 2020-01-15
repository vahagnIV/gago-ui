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
 public:
  VideoPlayer();
  void ShowImage(const cv::Mat &image);
  void setText(const std::string & text);
 private:
  QImage qimage_;
  QString text_;
};

}
}
}

#endif //GAGO_UI_VIDEO_PLAYER_H
