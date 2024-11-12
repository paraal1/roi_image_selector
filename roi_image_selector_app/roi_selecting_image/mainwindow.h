#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QColor>
#include <QColorDialog>
#include <QMouseEvent>
#include <opencv2/opencv.hpp>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadImage();
    void selectColor();
    void applyBackgroundColor();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;


private:
    QLabel *imageLabel;
    QPushButton *loadImageButton;
    QPushButton *selectColorButton;
    QPushButton *applyBackgroundButton;
    cv::Mat originalImage;
    cv::Mat modifiedImage;
    QColor backgroundColor;
    QRect roiRect;
    bool isSelecting;

    void updateImageDisplay();
};

#endif // MAINWINDOW_H
