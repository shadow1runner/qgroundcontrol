#ifndef OPENCVQUICKPAINTEDITEM_H
#define OPENCVQUICKPAINTEDITEM_H

#include <opencv2/core.hpp>

#include <QQuickPaintedItem>
#include <QImage>

class QPainter;

class OpenCVQuickPaintedItem : public QQuickPaintedItem {
    Q_OBJECT
    Q_PROPERTY(QImage image NOTIFY imageChanged)

public:
    OpenCVQuickPaintedItem(QQuickItem *parent = 0);
    QImage* matToQImage(cv::Mat mat);
    void paint(QPainter *painter);

signals:

public slots:
    void setFrame(cv::Mat mat);

private:
    QImage _image;
};

#endif // OPENCVQUICKPAINTEDITEM_H
