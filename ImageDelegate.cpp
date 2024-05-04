#include "ImageDelegate.h"
#include <QPainter>
#include <QImage>
#include <QModelIndex>


ImageDelegate::ImageDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

void ImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QVariant value = index.data(Qt::DisplayRole);
    if(value.isValid() && !value.isNull()) {
        QString imagePath = value.toString();
        QImage image(imagePath);
        if (!image.isNull()) {
            QRect rect = option.rect;
            QSize size = image.size();
            size.scale(rect.size(), Qt::KeepAspectRatio);
            QPoint center = rect.center() - QPoint(size.width() / 2, size.height() / 2);
            QRect centerRect(center, size);
            painter->drawImage(centerRect, image);
        } else {
            QStyledItemDelegate::paint(painter, option, index);
        }
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

