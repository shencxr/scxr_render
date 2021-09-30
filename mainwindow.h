#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <eigen3/Eigen/Dense>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    static bool TriangleInside(const Eigen::Vector2d &p0, const Eigen::Vector2d &p1,
                               const Eigen::Vector2d &p2, const Eigen::Vector2d &o);

private:
    void mousePressEvent(QMouseEvent *e) override;

    int32_t _round(float n) { return static_cast<int32_t>(std::lround(n)); }
    void NDC_DrawLine2D(const Eigen::Vector2f &p0, const Eigen::Vector2f &p1);
    Ui::MainWindow *ui;
    Eigen::Vector2i NDC2Screen2D(const Eigen::Vector2f &p);
    QImage *img_;
    QLabel *getCanvasLabel() { return dynamic_cast<QLabel *>(centralWidget()); }
    void test_NDC_DrawLine2D();
    void DrawPixel(uint32_t x, uint32_t y, const QColor &color);
    void NDC_DrawTriangle2D(const Eigen::Vector2f &p0, const Eigen::Vector2f &p1,
                            const Eigen::Vector2f &p2);
    void test_NDC_DrawTriangle2D();
    Eigen::Vector2f Screen2D2NDC(const Eigen::Vector2i &p);
    void NDC_DrawTriangle2D_AntiAlias(const Eigen::Vector2f &p0, const Eigen::Vector2f &p1,
                                      const Eigen::Vector2f &p2);
};
#endif  // MAINWINDOW_H
