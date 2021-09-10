#include "mainwindow.h"
#include <QDebug>
#include <QTimer>

#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setCentralWidget(new QLabel(this));
    auto canvas = getCanvasLabel();
    img_ = new QImage();
    img_->load("jieni.jpg");
    img_->setPixelColor(0, 0, QColor(255, 0, 0));
    canvas->setScaledContents(true);
    canvas->setPixmap(QPixmap::fromImage(*img_));
    //    test_NDC_DrawLine2D();
}

void MainWindow::test_NDC_DrawLine2D() {
    QTimer* test = new QTimer(this);
    img_->fill(QColor(255, 255, 255));
    connect(test, &QTimer::timeout, this, [=]() {
        NDC_DrawLine2D(Eigen::Vector2f::Zero(), Eigen::Vector2f::UnitX(), img_);
        NDC_DrawLine2D(Eigen::Vector2f::Zero(), Eigen::Vector2f::UnitY(), img_);
        NDC_DrawLine2D(Eigen::Vector2f::Zero(), -Eigen::Vector2f::UnitX(), img_);
        NDC_DrawLine2D(Eigen::Vector2f::Zero(), -Eigen::Vector2f::UnitY(), img_);
        const float x0 = float(rand()) / RAND_MAX;
        const float y0 = float(rand()) / RAND_MAX;
        const float x1 = float(rand()) / RAND_MAX;
        const float y1 = float(rand()) / RAND_MAX;
        qDebug() << "(" << x0 << y0 << ") - (" << x1 << y1 << ")";
        const Eigen::Vector2f p0(x0, y0);
        const Eigen::Vector2f p1(x1, y1);
        NDC_DrawLine2D(p0, p1, img_);
        getCanvasLabel()->setPixmap(QPixmap::fromImage(*img_));
    });
    test->start(1);
}

Eigen::Vector2i MainWindow::NDC2Screen2D(const Eigen::Vector2f& p) {
    canvas_h_ = img_->height();
    canvas_w_ = img_->width();
    Eigen::Matrix3f trans = Eigen::Matrix3f::Identity();
    trans(0, 0) = trans(0, 2) = 0.5f * canvas_w_;
    trans(1, 1) = trans(1, 2) = 0.5f * canvas_h_;
    const Eigen::Vector3f pos(p.x(), p.y(), 1.0f);
    return (trans * pos).block<2, 1>(0, 0).cast<int32_t>();
}

void MainWindow::NDC_DrawLine2D(const Eigen::Vector2f& p0, const Eigen::Vector2f& p1, QImage* img) {
    Eigen::Vector2i start = NDC2Screen2D(p0), end = NDC2Screen2D(p1);
    if (std::fabs(p1.x() - p0.x()) > std::fabs(p1.y() - p0.y())) {
        if (p0.x() > p1.x()) {
            std::swap(start, end);
        }
        const float k = float(start.y() - end.y()) / float(start.x() - end.x());
        for (float _x = start.x(), _y = start.y(); _x < end.x(); ++_x) {
            img->setPixelColor(_round(_x), canvas_h_ - _round(_y) - 1, QColor(255, 0, 0));
            _y += k;
        }
    } else {
        if (p0.y() > p1.y()) {
            std::swap(start, end);
        }
        const float k = float(start.x() - end.x()) / float(start.y() - end.y());
        for (float _x = start.x(), _y = start.y(); _y < end.y(); ++_y) {
            img->setPixelColor(_round(_x), canvas_h_ - _round(_y) - 1, QColor(255, 0, 0));
            _x += k;
        }
    }
}

MainWindow::~MainWindow() {
    delete img_;
    delete ui;
}
