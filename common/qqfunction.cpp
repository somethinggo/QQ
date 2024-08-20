#include "qqfunction.h"

namespace QQFunctions
{
    void initSystem()
    {
    }

    void exitSystem()
    {
    }

    QQConfigs::FriendConfig *getUserInfo(const QString &ID)
    {
        return nullptr;
    }

    QQConfigs::GroupConfig *getGroupInfo(const QString &ID)
    {
        return nullptr;
    }

    QPixmap getRoundedPixmap(const QPixmap &src, int radius)
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

    QString getCalculateText(const QString &text, const QFont &font, quint64 MAXW, bool elided)
    {
        QFontMetrics fm(font);
        if (elided)
        {
            return fm.elidedText(text, Qt::ElideRight, MAXW);
        }
        int width = fm.horizontalAdvance(text);
        if (width < MAXW)
        {
            return text;
        }
        else
        {
            QStringList lines;
            int start = 0;
            int length = text.length();
            while (start < length)
            {
                int end = start;
                int len = 0;
                while (end < length)
                {
                    len += fm.horizontalAdvance(text.mid(end, 1));
                    if (len > MAXW)
                    {
                        break;
                    }
                    end++;
                }
                lines.append(text.mid(start, end - start));
                start = end;
            }
            return lines.join("\n");
        }
    }

    QRect getCalculateTextRect(const QString &text, const QFont &font)
    {
        QFontMetrics fm(font);
        QStringList lines = text.split('\n');
        int width = 0;
        int height = 0;
        for (const QString &line : lines)
        {
            QRect rect = fm.boundingRect(line);
            width = qMax(width, rect.width());
            height += rect.height();
        }
        return QRect(0, 0, width, height);
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

    QImage getBase64ToImage(const QString &base64)
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

    bool getMouseIsInWidget(QWidget *widget, QPoint pos)
    {
        if (widget == nullptr || !widget->isVisible())
        {
            return false;
        }
        if (pos.isNull())
        {
            pos = QCursor::pos();
        }
        QRect widgetRect = widget->rect().translated(widget->mapToGlobal(QPoint(0, 0)));
        return widgetRect.contains(pos);
    }

    QImage getFillImage(const QImage &image, const QColor &pre, const QColor &next)
    {
        QImage result = image;
        QRgb preRgb = pre.rgb();
        int r = pre.red(), g = pre.green(), b = pre.blue(), a = pre.alpha();
        for (int y = 0; y < result.height(); y++)
        {
            for (int x = 0; x < result.width(); x++)
            {
                QRgb pixel = result.pixel(x, y);
                if (qRed(pixel) == r && qGreen(pixel) == g && qBlue(pixel) == b && qAlpha(pixel) == a)
                {
                    result.setPixelColor(x, y, next);
                }
            }
        }
        return result;
    }
}