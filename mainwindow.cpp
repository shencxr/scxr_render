#include "mainwindow.h"
#include <QDebug>
#include <QMouseEvent>
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
    //    test_NDC_DrawTriangle2D();
}

void MainWindow::test_NDC_DrawLine2D() {
    QTimer* test = new QTimer(this);
    img_->fill(QColor(255, 255, 255));
    connect(test, &QTimer::timeout, this, [=]() {
        NDC_DrawLine2D(Eigen::Vector2f::Zero(), Eigen::Vector2f::UnitX());
        NDC_DrawLine2D(Eigen::Vector2f::Zero(), Eigen::Vector2f::UnitY());
        NDC_DrawLine2D(Eigen::Vector2f::Zero(), -Eigen::Vector2f::UnitX());
        NDC_DrawLine2D(Eigen::Vector2f::Zero(), -Eigen::Vector2f::UnitY());
        const float x0 = (std::pow(-1, rand() % 2)) * float(rand()) / RAND_MAX;
        const float y0 = (std::pow(-1, rand() % 2)) * float(rand()) / RAND_MAX;
        const float x1 = (std::pow(-1, rand() % 2)) * float(rand()) / RAND_MAX;
        const float y1 = (std::pow(-1, rand() % 2)) * float(rand()) / RAND_MAX;
        const Eigen::Vector2f p0(x0, y0);
        const Eigen::Vector2f p1(x1, y1);
        NDC_DrawLine2D(p0, p1);
        getCanvasLabel()->setPixmap(QPixmap::fromImage(*img_));
    });
    test->start(1);
}

void MainWindow::test_NDC_DrawTriangle2D() {
    QTimer* test = new QTimer(this);
    img_->fill(QColor(255, 255, 255));
    NDC_DrawLine2D(Eigen::Vector2f::Zero(), Eigen::Vector2f::UnitX());
    NDC_DrawLine2D(Eigen::Vector2f::Zero(), Eigen::Vector2f::UnitY());
    NDC_DrawLine2D(Eigen::Vector2f::Zero(), -Eigen::Vector2f::UnitX());
    NDC_DrawLine2D(Eigen::Vector2f::Zero(), -Eigen::Vector2f::UnitY());
    connect(test, &QTimer::timeout, this, [=]() {
        img_->fill(QColor(255, 255, 255));
        const float x0 = (std::pow(-1, rand() % 2)) * float(rand()) / RAND_MAX;
        const float y0 = (std::pow(-1, rand() % 2)) * float(rand()) / RAND_MAX;
        const float x1 = (std::pow(-1, rand() % 2)) * float(rand()) / RAND_MAX;
        const float y1 = (std::pow(-1, rand() % 2)) * float(rand()) / RAND_MAX;
        const float x2 = (std::pow(-1, rand() % 2)) * float(rand()) / RAND_MAX;
        const float y2 = (std::pow(-1, rand() % 2)) * float(rand()) / RAND_MAX;
        const Eigen::Vector2f p0(x0, y0);
        const Eigen::Vector2f p1(x1, y1);
        const Eigen::Vector2f p2(x2, y2);
        NDC_DrawTriangle2D(p0, p1, p2);
        getCanvasLabel()->setPixmap(QPixmap::fromImage(*img_));
    });
    test->start(1);
}

//(0,0) at left down corner
Eigen::Vector2i MainWindow::NDC2Screen2D(const Eigen::Vector2f& p) {
    Eigen::Matrix3f trans = Eigen::Matrix3f::Identity();
    trans(0, 0) = trans(0, 2) = 0.5f * img_->width();
    trans(1, 1) = trans(1, 2) = 0.5f * img_->height();
    const Eigen::Vector3f pos(p.x(), p.y(), 1.0f);
    return (trans * pos).block<2, 1>(0, 0).cast<int32_t>();
}

//(0,0) at left down corner
Eigen::Vector2f MainWindow::Screen2D2NDC(const Eigen::Vector2i& p) {
    Eigen::Matrix3f trans = Eigen::Matrix3f::Identity();
    trans(0, 0) = trans(0, 2) = 0.5f * img_->width();
    trans(1, 1) = trans(1, 2) = 0.5f * img_->height();
    const Eigen::Vector3f pos(p.x(), p.y(), 1.0f);
    return (trans.inverse() * pos).block<2, 1>(0, 0);
}

void MainWindow::NDC_DrawLine2D(const Eigen::Vector2f& p0 /*[-1,1]*/,
                                const Eigen::Vector2f& p1 /*[-1,1]*/) {
    Eigen::Vector2i start = NDC2Screen2D(p0), end = NDC2Screen2D(p1);
    if (std::fabs(p1.x() - p0.x()) > std::fabs(p1.y() - p0.y())) {
        if (p0.x() > p1.x()) {
            std::swap(start, end);
        }
        const float k = float(start.y() - end.y()) / float(start.x() - end.x());
        for (float _x = start.x(), _y = start.y(); _x < end.x(); ++_x) {
            DrawPixel(_round(_x), _round(_y), QColor(255, 0, 0));
            _y += k;
        }
    } else {
        if (p0.y() > p1.y()) {
            std::swap(start, end);
        }
        const float k = float(start.x() - end.x()) / float(start.y() - end.y());
        for (float _x = start.x(), _y = start.y(); _y < end.y(); ++_y) {
            DrawPixel(_round(_x), _round(_y), QColor(255, 0, 0));
            _x += k;
        }
    }
}

void MainWindow::NDC_DrawTriangle2D(const Eigen::Vector2f& p0, const Eigen::Vector2f& p1,
                                    const Eigen::Vector2f& p2) {
    Eigen::Matrix2Xi t_p(2, 3);
    t_p << NDC2Screen2D(p0), NDC2Screen2D(p1), NDC2Screen2D(p2);

    const Eigen::Vector2i min = t_p.rowwise().minCoeff();
    const Eigen::Vector2i max = t_p.rowwise().maxCoeff();

    for (int32_t y = min.y(); y <= max.y(); ++y) {
        for (int32_t x = min.x(); x <= max.x(); ++x) {
            if (TriangleInside(t_p.col(0), t_p.col(1), t_p.col(2), Eigen::Vector2i(x, y))) {
                DrawPixel(x, y, QColor(0, 255, 0));
            }
        }
    }
}

bool MainWindow::TriangleInside(const Eigen::Vector2i& p0, const Eigen::Vector2i& p1,
                                const Eigen::Vector2i& p2, const Eigen::Vector2i& o) {
    const Eigen::Vector3i t_p0(p0.x(), p0.y(), 0);
    const Eigen::Vector3i t_p1(p1.x(), p1.y(), 0);
    const Eigen::Vector3i t_p2(p2.x(), p2.y(), 0);
    const Eigen::Vector3i t_o(o.x(), o.y(), 0);

    Eigen::Vector3d v0 = (t_p1 - t_p0).cross(t_o - t_p0).cast<double>();
    if (v0 == Eigen::Vector3d::Zero()) {
        return false;
    } else {
        v0 = v0.normalized();
    }
    Eigen::Vector3d v1 = (t_p2 - t_p1).cross(t_o - t_p1).cast<double>();
    if (v1 == Eigen::Vector3d::Zero()) {
        return false;
    } else {
        v1 = v1.normalized();
    }
    Eigen::Vector3d v2 = (t_p0 - t_p2).cross(t_o - t_p2).cast<double>();
    if (v2 == Eigen::Vector3d::Zero()) {
        return false;
    } else {
        v2 = v2.normalized();
    }

    return v0 == v1 && v1 == v2 && v2 == v0;
}

void MainWindow::mousePressEvent(QMouseEvent* e) {
    static uint8_t num = 0;
    static Eigen::Matrix2Xf points(2, 3);
    auto size = getCanvasLabel()->size();
    qDebug() << num << e->pos() << size;
    points.col(num) << (float(e->x()) / size.width() - 0.5f) * 2.0f,
        (0.5f - float(e->y()) / size.height()) * 2.0f;
    ++num;
    if (num == 3) {
        NDC_DrawTriangle2D(points.col(0), points.col(1), points.col(2));
        getCanvasLabel()->setPixmap(QPixmap::fromImage(*img_));
        num = 0;
    }
}

//(0,0) at left down corner
void MainWindow::DrawPixel(uint32_t x, uint32_t y, const QColor& color) {
    x = x % img_->width();
    y = y % img_->height();
    img_->setPixelColor(x, img_->height() - (y)-1, color);
}

MainWindow::~MainWindow() {
    delete img_;
    delete ui;
}
