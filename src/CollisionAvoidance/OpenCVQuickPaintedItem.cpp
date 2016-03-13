#include "OpenCVQuickPaintedItem.h"

#include <QImage>
#include <QPainter>
#include <QQuickView>

OpenCVQuickPaintedItem::OpenCVQuickPaintedItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
}

QImage* OpenCVQuickPaintedItem::matToQImage(cv::Mat mat) {
    // http://stackoverflow.com/a/12312326/2559632
    return new QImage((uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
}

void OpenCVQuickPaintedItem::paint(QPainter * painter) {
    // http://stackoverflow.com/a/20693161/2559632
    QRectF source(0.0, 0.0, _image.width(), _image.height());

    // https://doc.qt.io/qt-4.8/qpainter.html#drawImage
    painter->drawImage(source, _image, source);
}

void OpenCVQuickPaintedItem::setFrame(cv::Mat mat) {
    _image = *(matToQImage(mat));
}
