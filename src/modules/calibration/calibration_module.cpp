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
  QDir cache_folder = GetParamSaveFolder(cameras);
  QSharedPointer<calibration::ICalibrator>
      window = calibration::CalibratorFactory::Create(&settings_, main_window_, cache_folder);
  camera_module_->RegisterWatcher(window.data());
  if (QDialog::Accepted == window->Calibrate()) {
    estimates_ = window->GetEstimates();
    SaveEstimatesAsOpenCvYml(cache_folder);
    save_action_->setEnabled(true);
  }
  camera_module_->UnRegisterWatcher(window.data());
}

void CalibrationModule::GetRequiredModules(QList<RequiredModuleParams> & out_required_modules) {
  out_required_modules = {RequiredModuleParams{Name: "main", MinMajorVersion: 1, MinMinorVersion: 0},
                          RequiredModuleParams{Name: "settings", MinMajorVersion: 1, MinMinorVersion: 0},
                          RequiredModuleParams{Name: "camera", MinMajorVersion: 1, MinMinorVersion: 0}};
}

void CalibrationModule::SetRequiredModules(const QList<IModule *> & modules) {
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
  for (const QString & subfolder: calibration_dir.entryList(QStringList{"calib_*"}, QDir::Dirs)) {
    QDir folder(subfolder);
    if (!folder.exists("info.txt"))
      continue;

    int idx = subfolder.right(3).toInt();
    if (idx > last_folder)
      last_folder = idx;

    QFile file(folder.filePath("info.txt"));
    QTextStream fstream(&file);
    QString content = fstream.readAll();
    if (content == id_string) {
      correct_folder = subfolder;
      break;
    }
  }
  if (correct_folder == QDir("")) {
    QString folder_name = QString("calib_") + QString::number(last_folder + 1).leftJustified(3, '0');
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
  cv::FileStorage
      extrinsics_file_storage(folder.filePath("extrinsics.yml").toStdString(), cv::FileStorage::WRITE);
  extrinsics_file_storage << "R" << estimates_.R << "F" << estimates_.F << "T" << estimates_.T << "E" << estimates_.E
                          << "RMS" << estimates_.rms;
  cv::FileStorage
      intrinsics_file_storage(folder.filePath("intrinsics.yml").toStdString(), cv::FileStorage::WRITE);
  intrinsics_file_storage << "P1" << estimates_.intrinsic_parameters[0].camera_matrix << "D1"
                          << estimates_.intrinsic_parameters[0].distortion_coefficients << "RPE1"
                          << estimates_.intrinsic_parameters[0].rms << "Size1"
                          << estimates_.intrinsic_parameters[0].image_size << "P2"
                          << estimates_.intrinsic_parameters[1].camera_matrix << "D2"
                          << estimates_.intrinsic_parameters[1].distortion_coefficients << "RPE2"
                          << estimates_.intrinsic_parameters[0].rms << "Size2"
                          << estimates_.intrinsic_parameters[0].image_size;

}

int CalibrationModule::GetDestructorIndex() const {
  return -1;
}

void CalibrationModule::LoadEstimatesFromOpenCvYml(QDir folder) {
  if (!folder.exists("extrinsics.yml"))
    return;
  cv::FileStorage
      extrinsics_file_storage(folder.filePath("extrinsics.yml").toStdString(), cv::FileStorage::READ);
  estimates_.R = extrinsics_file_storage["R"].mat();
  estimates_.T = extrinsics_file_storage["T"].mat();
  estimates_.F = extrinsics_file_storage["F"].mat();
  estimates_.E = extrinsics_file_storage["E"].mat();
  estimates_.rms = extrinsics_file_storage["RMS"];

  cv::FileStorage
      intrinsics_file_storage(folder.filePath("intrinsics.yml").toStdString(), cv::FileStorage::READ);

  estimates_.intrinsic_parameters[0].camera_matrix = intrinsics_file_storage["P1"].mat();
  estimates_.intrinsic_parameters[0].distortion_coefficients = intrinsics_file_storage["D1"].mat();
  estimates_.intrinsic_parameters[0].rms = intrinsics_file_storage["RPE1"];
  estimates_.intrinsic_parameters[0].image_size.width = intrinsics_file_storage["Size1"][0];
  estimates_.intrinsic_parameters[0].image_size.height = intrinsics_file_storage["Size1"][1];

  estimates_.intrinsic_parameters[1].camera_matrix = intrinsics_file_storage["P2"].mat();
  estimates_.intrinsic_parameters[1].distortion_coefficients = intrinsics_file_storage["P2"].mat();
  estimates_.intrinsic_parameters[1].rms = intrinsics_file_storage["RPE2"];
  estimates_.intrinsic_parameters[1].image_size.width = intrinsics_file_storage["Size2"][0];
  estimates_.intrinsic_parameters[1].image_size.height = intrinsics_file_storage["Size2"][1];

}

void CalibrationModule::Start() {

  LoadEstimatesFromOpenCvYml(GetParamSaveFolder(camera_module_->GetCameras()));
  if (!estimates_.R.empty())
    save_action_->setEnabled(true);
}

}
}
}
