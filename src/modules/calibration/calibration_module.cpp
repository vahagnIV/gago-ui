#include <QAction>
#include <QStandardPaths>
#include <QTextStream>
#include <QFileDialog>

#include "modules/settings/settings_module.h"
#include "calibration_module.h"
#include "modules/main/main_module.h"
#include "calibration_configurator.h"
#include "pattern/calibration_pattern_factory.h"
#include "calibrator/calibrator_factory.h"

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
  configuration::CalibrationConfigurator *cnf = new configuration::CalibrationConfigurator();
  /*if (!settings_.empty())
    cnf->SetConfiguration(settings_);*/

  // Create calibrator
  std::shared_ptr<calibration::ICalibrator> window = calibration::CalibratorFactory::Create(cnf, main_window_);
  camera_module_->RegisterWatcher(window.get());
  if (QDialog::Accepted == window->Calibrate()) {
    estimates_ = window->GetEstimates();
    QVector<const io::video::CameraMeta *> cameras = camera_module_->GetCameras();
    QDir param__save_folder = GetParamSaveFolder(cameras);
    SaveEstimatesAsOpenCvYml(param__save_folder);
    save_action_->setEnabled(true);
  }
  camera_module_->UnRegisterWatcher(window.get());

}

void CalibrationModule::GetRequiredModules(QList<RequiredModuleParams> & out_required_modules) {
  out_required_modules = {RequiredModuleParams{Name: "main", MinMajorVersion: 1, MinMinorVersion: 0},
                          RequiredModuleParams{Name: "settings", MinMajorVersion: 1, MinMinorVersion: 0},
                          RequiredModuleParams{Name: "camera", MinMajorVersion: 1, MinMinorVersion: 0}};
}

void CalibrationModule::SetRequiredModules(const QList<IModule *> &modules) {
  for (IModule *module: modules) {
    if (module->SystemName() == "main") {
      main_window_ = ((MainModule *) module)->MainWindow();
      QAction *action = ((MainModule *) module)->CreateMenuBranch("/File/Calibration/Calibrate");
      connect(action, &QAction::triggered, this, &CalibrationModule::Calibrate);
      save_action_ = ((MainModule *) module)->CreateMenuBranch("/File/Calibration/Save As/Opencv.yml");
      connect(save_action_, &QAction::triggered, this, &CalibrationModule::SaveParamsToFolder);
      save_action_->setEnabled(false);

    } else if (module->SystemName() == "settings") {
      ((SettingsModule *) module)->RegisterConfigurable(this);
    } else if (module->SystemName() == "camera")
      camera_module_ = (CameraModule *) module;
  }
}

configuration::IConfigurator *CalibrationModule::GetConfigurator() {
  configuration::IConfigurator *cnf = new configuration::CalibrationConfigurator();
  return cnf;
}

void CalibrationModule::DisposeConfigurator(configuration::IConfigurator *configurator) {
  configuration::CalibrationConfigurator *cnf = (configuration::CalibrationConfigurator *) configurator;
  delete cnf;
}

void CalibrationModule::ApplyConfiguration(QSettings & settings, configuration::IConfigurator *configurator) {
  //configurator->GetConfiguration(settings_);
}

QDir CalibrationModule::GetParamSaveFolder(QVector<const io::video::CameraMeta *> cameras) {
  QStringList l = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);

  // TODO: implement cross platform
  QDir home_dir = l.isEmpty() ? QDir("/tmp") : QDir(l[0]);
  // TODO: implement global
  if (!home_dir.exists(".gago"))
    home_dir.mkdir(".gago");
  QDir gago_dir(home_dir.filePath(".gago"));

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
  extrinsics_file_storage << "R" << estimates_.R << "F" << estimates_.F << "T" << estimates_.T << "E" << estimates_.E;
  cv::FileStorage
      intrinsics_file_storage(folder.filePath("intrinsics.yml").toStdString(), cv::FileStorage::WRITE);
  intrinsics_file_storage << "P1" << estimates_.intrinsic_parameters[0].camera_matrix << "D1"
                          << estimates_.intrinsic_parameters[0].distortion_coefficients << "P2"
                          << estimates_.intrinsic_parameters[1].camera_matrix << "D2"
                          << estimates_.intrinsic_parameters[1].distortion_coefficients;

}

int CalibrationModule::GetWeight() const {
  return camera_module_->GetWeight() + 1;
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

  cv::FileStorage
      intrinsics_file_storage(folder.filePath("intrinsics.yml").toStdString(), cv::FileStorage::READ);

  estimates_.intrinsic_parameters[0].camera_matrix = intrinsics_file_storage["P1"].mat();
  estimates_.intrinsic_parameters[0].distortion_coefficients = intrinsics_file_storage["D1"].mat();
  estimates_.intrinsic_parameters[1].camera_matrix = intrinsics_file_storage["P2"].mat();
  estimates_.intrinsic_parameters[1].distortion_coefficients = intrinsics_file_storage["P2"].mat();

}

void CalibrationModule::Start() {

  LoadEstimatesFromOpenCvYml(GetParamSaveFolder(camera_module_->GetCameras()));
  if(!estimates_.R.empty())
    save_action_->setEnabled(true);
}

void CalibrationModule::Configure(QSettings & settings) {

}

const QString & CalibrationModule::GetName() const {
  return Name();
}

}
}
}
