#include "mainwindow.h"

#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    main_ = new QLabel(this);
    img_->load("jieni.jpg");
    img_->setPixelColor(0, 0, QColor(255, 0, 0));
    main_->setScaledContents(true);
    setCentralWidget(main_);
    canvas_h_ = main_->height();
    canvas_w_ = main_->width();
    main_->setPixmap(QPixmap::fromImage(*img_));
    *img_ = main_->pixmap()->toImage();
}

Eigen::Vector2i MainWindow::NDC2Screen2D(const Eigen::Vector2f& p) {
    Eigen::Matrix3f trans = Eigen::Matrix3f::Identity();
    trans(0, 0) = trans(0, 2) = 0.5f * img_->width();
    trans(1, 1) = trans(1, 2) = 0.5f * img_->height();
    const Eigen::Vector3f pos(p.x(), p.y(), 1.0f);
    return (trans * pos).block<2, 1>(0, 0).cast<int32_t>();
}

void MainWindow::NDC_DrawLine2D(const Eigen::Vector2f& p0, const Eigen::Vector2f& p1, QImage* img) {
    Eigen::Vector2i start = NDC2Screen2D(p0), end = NDC2Screen2D(p1);
    if (p0.x() > p1.x()) {
        std::swap(start, end);
    }
    const int32_t delta_x = start.x() - end.x();
    const int32_t delta_y = start.y() - end.y();
    if (std::fabs(delta_x) > std::fabs(delta_y)) {
        const float k = delta_y / delta_x;
        for (int32_t _x = start.x(), _y = start.y(); _x < end.x(); ++_x) {
            img->setPixelColor(_x, _y, QColor(255, 0, 0));
            _y = static_cast<int32_t>(std::lround(_y + k));
        }
    } else {
        const float k = delta_x / delta_y;
        for (int32_t _x = start.x(), _y = start.y(); _y < end.y(); ++_y) {
            img->setPixelColor(_x, _y, QColor(255, 0, 0));
            _x = static_cast<int32_t>(std::lround(_x + k));
        }
    }
}
void MainWindow::resizeEvent(QResizeEvent* event) {
    canvas_h_ = centralWidget()->height();
    canvas_w_ = centralWidget()->width();
}

void MainWindow::paintEvent(QPaintEvent* event) {
    NDC_DrawLine2D(Eigen::Vector2f::Zero(), Eigen::Vector2f::UnitX(), img_);
    main_->setPixmap(QPixmap::fromImage(*img_));
}

MainWindow::~MainWindow() { delete ui; }
