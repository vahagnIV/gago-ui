//
// Created by vahagn on 1/2/20.
//

#ifndef GAGO_UI_SRC_MODULES_CAMERA_CAMERA_SETTINGS_Q_MODEL_H_
#define GAGO_UI_SRC_MODULES_CAMERA_CAMERA_SETTINGS_Q_MODEL_H_
#include <QAbstractListModel>
#include <QList>

#include <io/video/camera.h>

namespace gago {
namespace gui {
namespace configuration  {

struct CameraProps {
  const io::video::CameraDeviceInfo *info;
  io::video::CameraSettings *settings;
};
class CameraSettingsQModel : public QAbstractListModel {
 public:
  CameraSettingsQModel();
  int rowCount(const QModelIndex & parent = QModelIndex()) const override;
  QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
  virtual bool insertRow(CameraProps setting) ;
 private:
  QList<CameraProps> list_;

};

}
}
}

#endif //GAGO_UI_SRC_MODULES_CAMERA_CAMERA_SETTINGS_Q_MODEL_H_
