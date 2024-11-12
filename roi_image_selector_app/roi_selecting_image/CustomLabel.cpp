#include "CustomLabel.h"

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent), isSelecting(false), highlightColor(Qt::cyan) {}

void CustomLabel::mousePressEvent(QMouseEvent *event) {
    if (this->underMouse()) {
        startPoint = event->pos();
        roiRect.setTopLeft(startPoint);
        isSelecting = true;
    }
}

void CustomLabel::mouseMoveEvent(QMouseEvent *event) {
    if (isSelecting) {
        currentPoint = event->pos();
        roiRect.setBottomRight(currentPoint);
        update(); // Reîmprospătează pentru a evidenția în timp real
    }
}

void CustomLabel::mouseReleaseEvent(QMouseEvent *event) {
    if (isSelecting) {
        roiRect.setBottomRight(event->pos());
        isSelecting = false;
        update();
    }
}

void CustomLabel::paintEvent(QPaintEvent *event) {
    QLabel::paintEvent(event);

    if (!roiRect.isNull()) {
        QPainter painter(this);
        painter.setPen(QPen(highlightColor, 2, Qt::DashLine));
        painter.setBrush(QBrush(highlightColor, Qt::Dense4Pattern));
        painter.drawRect(roiRect);
    }
}
