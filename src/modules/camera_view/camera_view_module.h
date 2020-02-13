#ifndef CAMERA_VIEW_MODULE_H
#define CAMERA_VIEW_MODULE_H

#include "imodule.h"
#include "modules/main/view.h"
#include "modules/camera/camera_module.h"
#include "modules/main/main_module.h"
#include "video_player.h"

namespace gago {
namespace gui {
namespace modules {

class Camera_viewModule : public IModule, public View, public CameraWatcher {

 public:
  Camera_viewModule();

  // IModule
  ModuleInitializationResult Initalize() override;
  unsigned int MajorVersion() const override;
  unsigned int MinorVersion() const override;
  void QRequiredModules(QList<RequiredModuleParams> & out_required_modules) override;
  void SetRequiredModules(const QList<IModule *> & modules) override;

  // View
  void StartDrawing(QWidget *widget) override;
  void StopDrawing() override;
  const std::string & GetName() const override;
  void Notify(const std::shared_ptr<std::vector<io::video::Capture>> & ptr) override;
  void SetCameras(const std::vector<const io::video::CameraMeta *> & vector) override;
  virtual ~Camera_viewModule();
  int GetWeight() const override;
 private:
  void ClearPlayers();
  std::string view_name_ = "Raw Cameras";
  CameraModule * camera_module_;
  MainModule * main_module_;
  std::unordered_map<std::string, gago::gui::common::VideoPlayer*> players_;
  QWidget * draw_widget_;

  // Camera_viewModule
};

}
}
}
#endif //CAMERA_VIEW_MODULE_H
