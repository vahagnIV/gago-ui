#include "rectified_image_view_window.h"
#include "ui_rectified_image_view_window.h"

RectifiedImageViewWindow::RectifiedImageViewWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RectifiedImageViewWindow) {
  ui->setupUi(this);
  connect(ui->actionBack, &QAction::triggered, this, &RectifiedImageViewWindow::PreviousButtonPressedSlot);
  connect(ui->actionForward, &QAction::triggered, this, &RectifiedImageViewWindow::NextButtonPressedSlot);
  //ui->horizontalLayout->setAlignment(Qt::AlignTop | ui->horizontalLayout->alignment());
}

RectifiedImageViewWindow::~RectifiedImageViewWindow() {
  delete ui;
}

void RectifiedImageViewWindow::ShowImage(const QImage & image) {
  QSize size = this->size();
  float scale = 1;

  scale = std::min(scale, (float) (size.width() - 30) / image.width());
  scale = std::min(scale, (float) (size.height() - 30) / image.height());

  QImage qimage_ = image.scaled(scale * image.width(), scale * image.height());

  ui->label->setPixmap(QPixmap::fromImage(qimage_));

}

void RectifiedImageViewWindow::NextButtonPressedSlot() {
  int new_id = (current_selected_idx_ + 1) % batch_calib_results_.size();
  SetActiveBatch(new_id);
}

void RectifiedImageViewWindow::PreviousButtonPressedSlot() {
  int new_id = current_selected_idx_ == 0 ? batch_calib_results_.size() - 1 : current_selected_idx_ - 1;
  SetActiveBatch(new_id);
}

QImage RectifiedImageViewWindow::CreateRectifiedImage(const QList<cv::Mat> & images) {

  cv::Mat canvas, img;
  double sf;
  int w, h;
  sf = 600. / MAX(image_size.width, image_size.height);
  w = cvRound(image_size.width * sf);
  h = cvRound(image_size.height * sf);
  canvas.create(h, w * 2, CV_8UC3);

  for (int k = 0; k < 2; k++) {

    cv::remap(images[k], img, rmap[k][0], rmap[k][1], cv::INTER_LINEAR);

//      cv::Mat canvasPart = !isVerticalStereo ? canvas(Rect(w * k, 0, w, h)) : canvas(Rect(0, h * k, w, h));
    cv::Mat canvasPart = canvas(cv::Rect(w * k, 0, w, h));
    cv::resize(img, canvasPart, canvasPart.size(), 0, 0, cv::INTER_AREA);

    cv::Rect vroi(cvRound(validRoi[k].x * sf), cvRound(validRoi[k].y * sf),
                  cvRound(validRoi[k].width * sf), cvRound(validRoi[k].height * sf));
    rectangle(canvasPart, vroi, cv::Scalar(0, 0, 255), 3, 8);

  }

  for (int j = 0; j < canvas.rows; j += 16)
    cv::line(canvas, cv::Point(0, j), cv::Point(canvas.cols, j), cv::Scalar(0, 255, 0), 2, 8);

  cv::cvtColor(canvas, canvas, cv::COLOR_RGB2BGR);

  QImage output_image(canvas.data, canvas.cols, canvas.rows, canvas.cols * canvas.channels(), QImage::Format_RGB888);
  return output_image.copy();
}

QList<cv::Mat> RectifiedImageViewWindow::LoadFiles(int batch_idx) {
  QList<cv::Mat> result;
  for (const gago::calibration::Pattern & params : batch_calib_results_[batch_idx].pattern_params) {
    result.append(cv::imread(params.filename.toStdString()));
  }
  return result;
}

void RectifiedImageViewWindow::SetActiveBatch(int batch_idx) {
  current_selected_idx_ = batch_idx;
  QImage image = CreateRectifiedImage(LoadFiles(batch_idx));
  ShowImage(image);
  ui->label_2->setText(batch_calib_results_[batch_idx].pattern_params[0].filename);
  ui->label_3->setText(batch_calib_results_[batch_idx].pattern_params[1].filename);
  emit ActiveBatchChanged(batch_idx);
}

void RectifiedImageViewWindow::SetCalibrationEstimates(const gago::calibration::CalibrationEstimates & estimates,
                                                       const QList<gago::calibration::PatternBatch> & results) {
  estimates_ = estimates;
  batch_calib_results_ = results;
  if (results.isEmpty())
    return;
  int first_valid_idx = 0;
  while (results[first_valid_idx].pattern_params.isEmpty())
    ++first_valid_idx;

  // TODO: remove this line in future
  image_size = estimates_.intrinsic_parameters[0].image_size;

  stereoRectify(estimates_.intrinsic_parameters[0].camera_matrix,
                estimates_.intrinsic_parameters[0].distortion_coefficients,
                estimates_.intrinsic_parameters[1].camera_matrix,
                estimates_.intrinsic_parameters[1].distortion_coefficients,
                image_size,
                estimates_.R,
                estimates_.T,
                R1,
                R2,
                P1,
                P2,
                Q,
                cv::CALIB_ZERO_DISPARITY,
                1,
                image_size ,
                &validRoi[0],
                &validRoi[1]);

  initUndistortRectifyMap(estimates_.intrinsic_parameters[0].camera_matrix,
                          estimates_.intrinsic_parameters[0].distortion_coefficients,
                          R1,
                          P1,
                          image_size,
                          CV_16SC2,
                          rmap[0][0],
                          rmap[0][1]);
  initUndistortRectifyMap(estimates_.intrinsic_parameters[1].camera_matrix,
                          estimates_.intrinsic_parameters[1].distortion_coefficients,
                          R2,
                          P2,
                          image_size ,
                          CV_16SC2,
                          rmap[1][0],
                          rmap[1][1]);

}

bool RectifiedImageViewWindow::CanShow() {
  return !R1.empty() && !R2.empty();
}
