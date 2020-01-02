//
// Created by vahagn on 1/2/20.
//

#include "camera_settings_q_model.h"

namespace gago {
namespace gui {
namespace configuration {

CameraSettingsQModel::CameraSettingsQModel():list_() {

}

int CameraSettingsQModel::rowCount(const QModelIndex &parent) const {
  return list_.size();
}

QVariant CameraSettingsQModel::data(const QModelIndex &index, int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();
  int ind = index.row();
  if (ind < 0 || ind > list_.size())
    return QVariant();
  QString cam_name = QString::fromStdString(list_.at(ind).info->device_path);
  if (list_.at(ind).settings->status == io::video::CameraStatus::Disabled)
    cam_name += " (Disabled)";
  return cam_name;
}

bool CameraSettingsQModel::insertRow(CameraProps setting) {
  beginInsertRows(QModelIndex(), list_.size(), list_.size() + 1);
  list_.append(setting);
  endInsertRows();
  return true;
}


}
}
}