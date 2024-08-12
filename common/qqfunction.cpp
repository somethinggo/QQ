#include "qqfunction.h"

namespace QQFunctions
{
    void initSystem()
    {
    }

    void exitSystem()
    {
    }

    QQConfigs::UserFriend_C *getUserInfo(const QString &ID)
    {
        return nullptr;
    }

    QQConfigs::UserGroup_C *getGroupInfo(const QString &ID)
    {
        return nullptr;
    }

    QPixmap getRounedPixmap(const QPixmap &src, int radius)
    {
        if (src.isNull())
        {
            return QPixmap();
        }
        QPixmap pixmap(src.size());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
#else
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
#endif
        QPainterPath path;
        path.addRoundedRect(src.rect(), radius, radius);
        painter.setClipPath(path);
        painter.drawPixmap(src.rect(), src);
        return pixmap;
    }

    QPair<QRect, QString> getCalculateTextRects(const QString &text, const QFont &font, quint64 MAXW, bool elided)
    {
        QString result = text;
        QFontMetrics fm(font);
        qreal width = fm.horizontalAdvance(result);
        qreal height = fm.height();
        if (elided)
        {
            result = fm.elidedText(result, Qt::ElideRight, MAXW);
            width = fm.horizontalAdvance(result);
        }
        else if (width > MAXW)
        {
            width = MAXW;
            int start = 0;
            while (start < result.length())
            {
                int end = start;
                while (end < result.length())
                {
                    int len = fm.horizontalAdvance(result.mid(start, end - start));
                    if (len >= MAXW)
                    {
                        result.insert(end - 1, "\n");
                        height += fm.height();
                        break;
                    }
                    end++;
                }
                start = end;
            }
        }
        return QPair<QRect, QString>(QRect(0, 0, width, height), result);
    }

    QBitmap getRoundedMask(const QSize &size, qreal radius, QQEnums::RoundPostionTypes types)
    {
        QBitmap bitmap(size);
        bitmap.fill(Qt::color0);
        QPainter painter(&bitmap);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        painter.setBrush(Qt::color1);
        QRectF topLeft(0, 0, 2 * radius, 2 * radius);
        QRectF topRight(size.width() - 2 * radius, 0, 2 * radius, 2 * radius);
        QRectF bottomLeft(0, size.height() - 2 * radius, 2 * radius, 2 * radius);
        QRectF bottomRight(size.width() - 2 * radius, size.height() - 2 * radius, 2 * radius, 2 * radius);
        QPainterPath path;
        path.moveTo(0, radius);
        if (types.testFlag(QQEnums::RoundPostionType::topleft))
        {
            path.arcTo(topLeft, 180, -90);
        }
        else
        {
            path.lineTo(bitmap.rect().topLeft());
            path.lineTo(bitmap.rect().topLeft() + QPoint(radius, 0));
        }
        path.lineTo(bitmap.rect().topRight() + QPoint(-radius, 0));
        if (types.testFlag(QQEnums::RoundPostionType::topright))
        {
            path.arcTo(topRight, 90, -90);
        }
        else
        {
            path.lineTo(bitmap.rect().topRight());
            path.lineTo(bitmap.rect().topRight() + QPoint(0, radius));
        }
        path.lineTo(bitmap.rect().bottomRight() + QPoint(0, -radius));
        if (types.testFlag(QQEnums::RoundPostionType::bottomright))
        {
            path.arcTo(bottomRight, 0, -90);
        }
        else
        {
            path.lineTo(bitmap.rect().bottomRight());
            path.lineTo(bitmap.rect().bottomRight() + QPoint(-radius, 0));
        }
        path.lineTo(bitmap.rect().bottomLeft() + QPoint(radius, 0));
        if (types.testFlag(QQEnums::RoundPostionType::bottomleft))
        {
            path.arcTo(bottomLeft, 270, -90);
        }
        else
        {
            path.lineTo(bitmap.rect().bottomLeft());
            path.lineTo(bitmap.rect().bottomLeft() + QPoint(0, -radius));
        }
        path.lineTo(bitmap.rect().topLeft() + QPoint(0, radius));
        path.closeSubpath();
        painter.drawPath(path);
        return bitmap;
    }

    QPixmap getRotatePixmap(const QPixmap &src, qreal angle)
    {
        QTransform transform;
        transform.rotate(angle);
        return src.transformed(transform, Qt::SmoothTransformation);
    }

    QPair<qreal, QString> getFileFormatSize(qint64 bytes)
    {
        const qint64 KILOBYTE = 1024;
        const qint64 MEGABYTE = 1024 * KILOBYTE;
        const qint64 GIGABYTE = 1024 * MEGABYTE;
        const qint64 TERABYTE = 1024 * GIGABYTE;
        qreal result = bytes;
        QString type = "B";
        if (bytes >= TERABYTE)
        {
            result /= TERABYTE;
            type = "TB";
        }
        else if (bytes >= GIGABYTE)
        {
            result /= GIGABYTE;
            type = "GB";
        }
        else if (bytes >= MEGABYTE)
        {
            result /= MEGABYTE;
            type = "MB";
        }
        else if (bytes >= KILOBYTE)
        {
            result /= KILOBYTE;
            type = "KB";
        }
        return QPair<qreal, QString>(result, type);
    }

    QPair<QString, QString> getImageToBase64(const QImage &image)
    {
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        buffer.open(QIODevice::WriteOnly);
        QImageWriter writer(&buffer, "png");
        if (!writer.write(image))
        {
            return QPair<QString, QString>();
        }
        buffer.close();
        return QPair<QString, QString>(byteArray.toBase64(), "png");
    }

    QPair<QString, QString> getImageToBase64(const QString &fileName)
    {
        return QQFunctions::getImageToBase64(QImage(fileName));
    }

    QImage getBase64ToImage(const QString &base64, const QString &type)
    {
        QByteArray byteArray = QByteArray::fromBase64(base64.toUtf8());
        QImage image;
        image.loadFromData(byteArray);
        return image;
    }

    QString getGenerateRandomText(int min, int max)
    {
        const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
        const int possibleCharactersLength = possibleCharacters.length();
        int realLength = min + rand() % (max - min) + 1;
        QString randomString;
        randomString.reserve(realLength);
        for (int i = 0; i < realLength; ++i)
        {
            int index = QRandomGenerator::global()->bounded(possibleCharactersLength);
            randomString.append(possibleCharacters.at(index));
        }
        return randomString;
    }

    bool getMouseIsInWidget(QWidget *widget)
    {
        if (widget == nullptr || !widget->isVisible())
        {
            return false;
        }
        QPoint pos = QCursor::pos();
        QRect widgetRect = widget->rect().translated(widget->mapToGlobal(QPoint(0, 0)));
        return widgetRect.contains(pos);
    }
}