#include "MainWindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPainter>
#include <opencv2/imgproc.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isSelecting(false) {
    // Set up UI components
    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(640, 480);  // Set a default size or adjust based on your image size
    imageLabel->setMouseTracking(true);  // Enable mouse tracking for precise ROI drawing

    loadImageButton = new QPushButton("Load Image", this);
    selectColorButton = new QPushButton("Select Background Color", this);
    applyBackgroundButton = new QPushButton("Apply Background Color", this);

    // Connect signals to slots
    connect(loadImageButton, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(selectColorButton, &QPushButton::clicked, this, &MainWindow::selectColor);
    connect(applyBackgroundButton, &QPushButton::clicked, this, &MainWindow::applyBackgroundColor);

    // Set up layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(loadImageButton);
    layout->addWidget(selectColorButton);
    layout->addWidget(applyBackgroundButton);
    layout->addWidget(imageLabel);
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::loadImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image File");
    if (!fileName.isEmpty()) {
        originalImage = cv::imread(fileName.toStdString());
        modifiedImage = originalImage.clone();
        updateImageDisplay();
    }
}

void MainWindow::selectColor() {
    backgroundColor = QColorDialog::getColor(Qt::white, this, "Choose Background Color");
}

void MainWindow::applyBackgroundColor() {
    if (roiRect.isNull() || originalImage.empty())
        return;

    cv::Mat mask = cv::Mat::zeros(originalImage.size(), CV_8UC1);
    cv::rectangle(mask, cv::Rect(roiRect.x(), roiRect.y(), roiRect.width(), roiRect.height()), cv::Scalar(255), cv::FILLED);

    cv::Mat coloredBackground = originalImage.clone();
    cv::Scalar color(backgroundColor.red(), backgroundColor.green(), backgroundColor.blue());

    // Apply background color to areas outside ROI
    coloredBackground.setTo(color, ~mask);

    // Apply background color to areas within ROI if necessary
    cv::Mat roi = coloredBackground(cv::Rect(roiRect.x(), roiRect.y(), roiRect.width(), roiRect.height()));
    cv::bitwise_not(mask(cv::Rect(roiRect.x(), roiRect.y(), roiRect.width(), roiRect.height())), mask(cv::Rect(roiRect.x(), roiRect.y(), roiRect.width(), roiRect.height())));
    roi.setTo(color, mask(cv::Rect(roiRect.x(), roiRect.y(), roiRect.width(), roiRect.height())));

    modifiedImage = coloredBackground.clone();
    updateImageDisplay();
}

void MainWindow::updateImageDisplay() {
    if (modifiedImage.empty())
        return;

    // Convert modifiedImage to QImage and set it on imageLabel
    QImage img(modifiedImage.data, modifiedImage.cols, modifiedImage.rows, modifiedImage.step, QImage::Format_BGR888);
    imageLabel->setPixmap(QPixmap::fromImage(img));
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (imageLabel->underMouse()) {
        QPoint labelPos = imageLabel->mapFromParent(event->pos());
        roiRect.setTopLeft(labelPos);
        isSelecting = true;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isSelecting) {
        QPoint labelPos = imageLabel->mapFromParent(event->pos());
        roiRect.setBottomRight(labelPos);

        // Create a copy of the original image for temporary drawing
        QPixmap tempPixmap = QPixmap::fromImage(QImage(modifiedImage.data, modifiedImage.cols, modifiedImage.rows, modifiedImage.step, QImage::Format_BGR888));

        // Draw the rectangle on the temporary pixmap
        QPainter painter(&tempPixmap);
        painter.setPen(QPen(Qt::DashLine));
        painter.drawRect(roiRect);

        // Set the temporary pixmap with the rectangle as the current display
        imageLabel->setPixmap(tempPixmap);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (isSelecting) {
        QPoint labelPos = imageLabel->mapFromParent(event->pos());
        roiRect.setBottomRight(labelPos);
        isSelecting = false;
        updateImageDisplay();  // Final update after selection
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    if (!roiRect.isNull()) {
        QPixmap pixmap = imageLabel->pixmap();
        QPainter painter(&pixmap);
        painter.setPen(QPen(Qt::DashLine));
        painter.drawRect(roiRect);
        imageLabel->setPixmap(pixmap);
    }
    QMainWindow::paintEvent(event);
}
