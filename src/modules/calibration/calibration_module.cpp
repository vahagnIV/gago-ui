#include <QAction>
#include <QStandardPaths>
#include <QTextStream>
#include <QFileDialog>

#include "modules/settings/settings_module.h"
#include "calibration_module.h"
#include "modules/main/main_module.h"
#include "settings/calibration_configurator.h"
#include "calibrator/calibrator_factory.h"
#include "configuration.h"

namespace gago {
namespace gui {

namespace modules {

CalibrationModule::CalibrationModule() : QObject(nullptr), IModule("Calibration", "calibration") {
}

ModuleInitializationResult CalibrationModule::Initalize() {
  return ModuleInitializationResult::OK;
}

unsigned int CalibrationModule::MajorVersion() const {
  return 1;
}

unsigned int CalibrationModule::MinorVersion() const {
  return 0;
}

void CalibrationModule::Calibrate() {
  QVector<const io::video::CameraMeta *> cameras = camera_module_->GetCameras();
  LoadEstimatesFromOpenCvYml(GetParamSaveFolder(cameras));
  QDir cache_folder = GetParamSaveFolder(cameras);
  QSharedPointer<calibration::ICalibrator>
      window = calibration::CalibratorFactory::Create(settings_, estimates_, main_window_, cache_folder);
  camera_module_->RegisterWatcher(window.data());
  if (QDialog::Accepted == window->Calibrate()) {
    estimates_ = window->GetEstimates();
    SaveEstimatesAsOpenCvYml(cache_folder);
    save_action_->setEnabled(true);
  }
  camera_module_->UnRegisterWatcher(window.data());
}

void CalibrationModule::GetRequiredModules(QList<RequiredModuleParams> &out_required_modules) {
  out_required_modules = {RequiredModuleParams{Name: "main", MinMajorVersion: 1, MinMinorVersion: 0},
                          RequiredModuleParams{Name: "settings", MinMajorVersion: 1, MinMinorVersion: 0},
                          RequiredModuleParams{Name: "camera", MinMajorVersion: 1, MinMinorVersion: 0}};
}

void CalibrationModule::SetRequiredModules(const QList<IModule *> &modules) {
  for (IModule *module: modules) {
    if (module->SystemName() == "main") {
      main_window_ = ((MainModule *) module)->MainWindow();
      QAction *action = ((MainModule *) module)->GetAction("/File/Calibration/Calibrate");
      QMenu *menu = (QMenu *) action->associatedWidgets()[0];
      menu->setIcon(QIcon::fromTheme("calibrate"));
      connect(action, &QAction::triggered, this, &CalibrationModule::Calibrate);

      save_action_ = ((MainModule *) module)->GetAction("/File/Calibration/Save As/Opencv.yml");
      connect(save_action_, &QAction::triggered, this, &CalibrationModule::SaveParamsToFolder);
      ((QMenu *) save_action_->associatedWidgets()[0])->setIcon(QIcon::fromTheme("document-save-as"));
      save_action_->setEnabled(false);

    } else if (module->SystemName() == "settings") {
      ((SettingsModule *) module)->RegisterConfigurable(&settings_);
    } else if (module->SystemName() == "camera")
      camera_module_ = (CameraModule *) module;
  }
}

QDir CalibrationModule::GetParamSaveFolder(QVector<const io::video::CameraMeta *> cameras) {
  QDir gago_dir = configuration::Configuration::Instance().GetCacheFolderPath();

  if (!gago_dir.exists("calibration"))
    gago_dir.mkdir("calibration");
  QDir calibration_dir(gago_dir.filePath("calibration"));

  QString id_string = "";
  for (const io::video::CameraMeta *camera: cameras)
    id_string +=
        (QString::fromStdString(camera->GetName()) + ": " + QString::fromStdString(camera->GetHardwareDetails())
            + "\n");
  QDir correct_folder("");
  int last_folder = 0;
  for (const QString &subfolder: calibration_dir.entryList(QStringList{"calib_*"}, QDir::Dirs)) {
    QDir folder(calibration_dir.filePath(subfolder));
    if (!folder.exists("info.txt"))
      continue;

    int idx = subfolder.right(3).toInt();
    if (idx > last_folder)
      last_folder = idx;

    QFile file(folder.filePath("info.txt"));
    file.open(QFile::ReadOnly);
    QTextStream fstream(&file);
    QString content = fstream.readAll();
    if (content == id_string) {
      correct_folder = folder;
      break;
    }
  }
  if (correct_folder == QDir("")) {
    QString folder_name = QString("calib_") + QString::number(last_folder + 1).rightJustified(3, '0');
    calibration_dir.mkdir(folder_name);
    correct_folder.setPath(calibration_dir.filePath(folder_name));
    QFile file(correct_folder.filePath("info.txt"));
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream << id_string;
    file.close();
  }

  return correct_folder;
}

void CalibrationModule::SaveParamsToFolder() {
  QFileDialog *save_to_dlg = new QFileDialog(main_window_);
  save_to_dlg->setFileMode(QFileDialog::FileMode::Directory);
  if (save_to_dlg->exec() == QFileDialog::Accepted)
    SaveEstimatesAsOpenCvYml(save_to_dlg->directory());;
  save_to_dlg->deleteLater();

}

void CalibrationModule::SaveEstimatesAsOpenCvYml(QDir folder) {
  if (estimates_.intrinsic_parameters.size() == 2) {
    cv::FileStorage
        extrinsics_file_storage(folder.filePath("extrinsics.yml").toStdString(), cv::FileStorage::WRITE);
    extrinsics_file_storage << "R" << estimates_.R << "F" << estimates_.F << "T" << estimates_.T << "E" << estimates_.E
                            << "RMS" << estimates_.rms;
  }
  cv::FileStorage
      intrinsics_file_storage(folder.filePath("intrinsics.yml").toStdString(), cv::FileStorage::WRITE);
  for (int cam_idx = 0; cam_idx < estimates_.intrinsic_parameters.size(); ++cam_idx) {
    gago::calibration::IntrinsicParameters &intrinsics = estimates_.intrinsic_parameters[cam_idx];
    std::string camera_matrix_name = "P" + std::to_string(cam_idx);
    std::string distortion_coefficients_name = "D" + std::to_string(cam_idx);
    std::string rpe_name = "RPE" + std::to_string(cam_idx);
    std::string size_name = "Size" + std::to_string(cam_idx);
    intrinsics_file_storage << camera_matrix_name << estimates_.intrinsic_parameters[cam_idx].camera_matrix
                            << distortion_coefficients_name
                            << estimates_.intrinsic_parameters[cam_idx].distortion_coefficients << rpe_name
                            << estimates_.intrinsic_parameters[cam_idx].rms << size_name
                            << estimates_.intrinsic_parameters[cam_idx].image_size;

  }
}

int CalibrationModule::GetDestructorIndex() const {
  return -1;
}

int CalibrationModule::GetStartIndex() const {
  return camera_module_->GetStartIndex() + 100;
}

void CalibrationModule::LoadEstimatesFromOpenCvYml(QDir folder) {
  const int camera_count = camera_module_->GetCameras().size();

  if (camera_count == 2) {
    if (!folder.exists("extrinsics.yml"))
      return;
    cv::FileStorage
        extrinsics_file_storage(folder.filePath("extrinsics.yml").toStdString(), cv::FileStorage::READ);
    estimates_.R = extrinsics_file_storage["R"].mat();
    estimates_.T = extrinsics_file_storage["T"].mat();
    estimates_.F = extrinsics_file_storage["F"].mat();
    estimates_.E = extrinsics_file_storage["E"].mat();
    estimates_.rms = extrinsics_file_storage["RMS"];
  }
  else{
    estimates_.R = cv::Mat();
    estimates_.T = cv::Mat();
    estimates_.E = cv::Mat();
    estimates_.F = cv::Mat();
  }

  estimates_.intrinsic_parameters.clear();
  cv::FileStorage
      intrinsics_file_storage(folder.filePath("intrinsics.yml").toStdString(), cv::FileStorage::READ);
  for (int cam_idx = 0; cam_idx < camera_count; ++cam_idx) {
    gago::calibration::IntrinsicParameters intrinsics;

    std::string camera_matrix_name = "P" + std::to_string(cam_idx);
    std::string distortion_coefficients_name = "D" + std::to_string(cam_idx);
    std::string rpe_name = "RPE" + std::to_string(cam_idx);
    std::string size_name = "Size" + std::to_string(cam_idx);

    intrinsics.camera_matrix = intrinsics_file_storage[camera_matrix_name].mat();
    intrinsics.distortion_coefficients = intrinsics_file_storage[distortion_coefficients_name].mat();
    intrinsics.rms = intrinsics_file_storage[rpe_name];
    intrinsics.image_size.width = intrinsics_file_storage[size_name][0];
    intrinsics.image_size.height = intrinsics_file_storage[size_name][1];
    estimates_.intrinsic_parameters.push_back(intrinsics);
  }

}

void CalibrationModule::Start() {

  LoadEstimatesFromOpenCvYml(GetParamSaveFolder(camera_module_->GetCameras()));
  if (!estimates_.R.empty())
    save_action_->setEnabled(true);
}

}
}
}
