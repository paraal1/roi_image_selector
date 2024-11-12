#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

class CustomLabel : public QLabel {
    Q_OBJECT

public:
    explicit CustomLabel(QWidget *parent = nullptr);
    QRect roiRect;
    bool isSelecting;
    QColor highlightColor;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPoint startPoint;
    QPoint currentPoint;
};

#endif // CUSTOMLABEL_H
