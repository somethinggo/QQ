#include "qqtheme.h"

void QQThemes::Style::SearchProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == PE_FrameLineEdit)
    {
        QRect rect = option->rect;
        QPen pen;
        pen.setWidth(1);
        if (widget && widget->hasFocus())
        {
            pen.setColor(QColor("#0099FF"));
        }
        else
        {
            pen.setColor(Qt::color0);
        }
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(pen);
        painter->drawRoundedRect(rect, 5, 5);
        return;
    }
    return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void QQThemes::Style::SearchProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == CE_PushButton)
    {
        QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
        if (buttonOption && (buttonOption->state & State_MouseOver))
        {
            int h, s, v;
            QColor color = widget->palette().brush(QPalette::Button).color();
            color.getHsv(&h, &s, &v);
            v = abs((v - 25) % 256);
            color.setHsv(h, s, v);
            painter->fillRect(option->rect, color);
        }
        if (buttonOption && (buttonOption->state & State_Sunken))
        {
            buttonOption->state &= ~State_Sunken;
            buttonOption->state &= ~State_MouseOver;
            int h, s, v;
            QColor color = widget->palette().brush(QPalette::Button).color();
            color.getHsv(&h, &s, &v);
            v = abs((v - 50) % 256);
            color.setHsv(h, s, v);
            painter->fillRect(option->rect, color);
        }
    }
    return QProxyStyle::drawControl(element, option, painter, widget);
}
