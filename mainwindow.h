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
    ~MainWindow();
    void resizeEvent(QResizeEvent *event) override;

    void paintEvent(QPaintEvent *event) override;

private:
    void NDC_DrawLine2D(const Eigen::Vector2f &p0, const Eigen::Vector2f &p1, QImage *img);
    Ui::MainWindow *ui;
    uint32_t canvas_w_;
    uint32_t canvas_h_;
    Eigen::Vector2i NDC2Screen2D(const Eigen::Vector2f &p);
    QImage *img_;
    QLabel *main_;
};
#endif  // MAINWINDOW_H
