#include <qpainter.h>
#include <qbitmap.h>
#include <qpainterpath.h>
#include <qsize.h>

#include "qglobalvalue.h"
#include "qdefine.h"

enum class RoundPostionType : char
{
    topleft = 0x01,
    topright = 0x02,
    bottomleft = 0x04,
    bottomright = 0x08,
    top = topleft | topright,
    bottom = bottomleft | bottomright,
    left = topleft | bottomleft,
    right = topright | bottomright,
    all = top | bottom,
};
Q_DECLARE_FLAGS(RoundPostionTypes, RoundPostionType)
Q_MDECLARE_OPERATORS_FOR_FLAGS(RoundPostionTypes)
// 取得圆角蒙版
inline QBitmap getRoundedRect(const QSize &size, qreal radius, RoundPostionType type = RoundPostionType::all)
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
    if (type & RoundPostionType::topleft)
    {
        path.arcTo(topLeft, 180, -90);
    }
    else
    {
        path.lineTo(bitmap.rect().topLeft());
        path.lineTo(bitmap.rect().topLeft() + QPoint(radius, 0));
    }
    path.lineTo(bitmap.rect().topRight() + QPoint(-radius, 0));
    if (type & RoundPostionType::topright)
    {
        path.arcTo(topRight, 90, -90);
    }
    else
    {
        path.lineTo(bitmap.rect().topRight());
        path.lineTo(bitmap.rect().topRight() + QPoint(0, radius));
    }
    path.lineTo(bitmap.rect().bottomRight() + QPoint(0, -radius));
    if (type & RoundPostionType::bottomright)
    {
        path.arcTo(bottomRight, 0, -90);
    }
    else
    {
        path.lineTo(bitmap.rect().bottomRight());
        path.lineTo(bitmap.rect().bottomRight() + QPoint(-radius, 0));
    }
    path.lineTo(bitmap.rect().bottomLeft() + QPoint(radius, 0));
    if (type & RoundPostionType::bottomleft)
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
