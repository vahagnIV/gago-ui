#include <QtWidgets/QHBoxLayout>
#include "camera_view_module.h"

#include "video_player.h"

namespace gago {
namespace gui {
namespace modules {

Camera_viewModule::Camera_viewModule() : IModule("CameraView", "camera_view") {
}

ModuleInitializationResult Camera_viewModule::Initalize() {
  return ModuleInitializationResult::OK;
}

unsigned int Camera_viewModule::MajorVersion() const {
  return 1;
}

unsigned int Camera_viewModule::MinorVersion() const {
  return 0;
}

void Camera_viewModule::QRequiredModules(std::vector<RequiredModuleParams> &out_required_modules) {
  out_required_modules.resize(2);
  out_required_modules[0].Name = "main";
  out_required_modules[0].MinMajorVersion = 1;
  out_required_modules[0].MinMinorVersion = 0;

  out_required_modules[1].Name = "camera";
  out_required_modules[1].MinMajorVersion = 1;
  out_required_modules[1].MinMinorVersion = 0;

}

void Camera_viewModule::SetRequiredModules(const std::vector<IModule *> &modules) {
  for (IModule *module: modules) {
    if (module->SystemName() == "main")
      main_module_ = ((MainModule *) modules[0]);

    else if (module->SystemName() == "camera")
      camera_module_ = (CameraModule *) module;
  }
  main_module_->RegisterView(this);
}

void Camera_viewModule::StartDrawing(QWidget *widget) {
  draw_widget_ = widget;
  camera_module_->RegisterWatcher(this);
}

void Camera_viewModule::StopDrawing() {
  camera_module_->UnRegisterWatcher(this);
}

const std::string &Camera_viewModule::GetName() const {
  return view_name_;
}

void Camera_viewModule::Notify(const std::shared_ptr<std::vector<io::video::Capture>> &ptr) {
  for (const io::video::Capture &capture: *ptr) {
    players_[capture.camera->GetUniqueId()]->ShowImage(capture.data);
  }
}

void Camera_viewModule::SetCameras(const std::vector<const io::video::CameraMeta *> &vector) {
  ClearPlayers();

  QHBoxLayout *layout = new QHBoxLayout();

  for (const io::video::CameraMeta *camera : vector) {
    players_[camera->GetUniqueId()] = new common::VideoPlayer();
    layout->addWidget(players_[camera->GetUniqueId()]);
  }
  draw_widget_->setLayout(layout);
}

void Camera_viewModule::ClearPlayers() {
  for (std::pair<std::string, common::VideoPlayer *> player_name_player: players_) {
    delete player_name_player.second;
  }
  players_.clear();
  delete draw_widget_->layout();
}

Camera_viewModule::~Camera_viewModule() {
  //StopDrawing();
  ClearPlayers();
}

int Camera_viewModule::GetWeight() const {
  return main_module_->GetWeight() + camera_module_->GetWeight() + 1;
}

}
}
}
