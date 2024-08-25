#include "QQFriendIndexDelegate.h"

QQFriendIndexDelegate::QQFriendIndexDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    m_commonSpace = 10;
}

QQFriendIndexDelegate::~QQFriendIndexDelegate()
{
}

void QQFriendIndexDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    if (!index.parent().isValid())
    {
        QStyleOptionViewItem opt = option;
        opt.rect.adjust(20, 0, -20, 0);
        if (option.state & QStyle::State_MouseOver)
        {
            painter->fillRect(option.rect, QQGlobals::g_theme->m_friend_index_parent_hover_color);
        }
        QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
        QRect iconRect = QRect(QPoint(0, 0), opt.decorationSize);
        iconRect.moveTopLeft(QPoint(opt.rect.x(), opt.rect.y() + opt.rect.height() / 2 - iconRect.height() / 2));
        painter->drawPixmap(iconRect, icon.pixmap(opt.decorationSize));
        QString text = index.data(Qt::DisplayRole).toString();
        QRect textRect = QRect(QPoint(0, 0), QSize(opt.fontMetrics.horizontalAdvance(text), opt.fontMetrics.height()));
        textRect.moveTopLeft(QPoint(iconRect.right() + 5, iconRect.top() - 2));
        painter->drawText(textRect, text);
    }
    else
    {
        if (option.state & QStyle::State_MouseOver)
        {
            painter->fillRect(option.rect, QQGlobals::g_theme->m_friend_index_hover_color);
        }
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, QQGlobals::g_theme->m_friend_index_selected_color);
        }

        painter->save();
        QPixmap icon = index.data(Qt::UserRole + 2).value<QPixmap>();
        QRect iconRect = QRect(QPoint(option.rect.height() / 2 - QQGlobals::g_theme->m_friend_index_icon_size.width() / 2, option.rect.y() + option.rect.height() / 2 - QQGlobals::g_theme->m_friend_index_icon_size.width() / 2),
                               QQGlobals::g_theme->m_friend_index_icon_size);
        painter->drawPixmap(iconRect, icon);
        painter->restore();

        if (index.data(Qt::UserRole + 1).toInt() == 0)
        {
            painter->save();
            QString name = index.data(Qt::UserRole + 3).toString();
            if (option.state & QStyle::State_Selected)
            {
                painter->setPen(QQGlobals::g_theme->m_friend_index_name_selected_color);
            }
            painter->setFont(QQGlobals::g_theme->m_friend_index_name_font);
            QRect nameRect = QQFunctions::getCalculateTextRect(name, painter->font());
            nameRect.moveTopLeft(QPoint(iconRect.right() + m_commonSpace, iconRect.top()));
            painter->drawText(nameRect, Qt::AlignCenter, name);

            painter->setFont(QQGlobals::g_theme->m_friend_index_sign_font);
            QString sign = index.data(Qt::UserRole + 4).toString();
            QRect signRect = QQFunctions::getCalculateTextRect(sign, painter->font());
            signRect.moveTopLeft(QPoint(nameRect.left(), nameRect.bottom()));
            painter->drawText(signRect, Qt::AlignCenter, sign);
            painter->restore();
        }
        else if (index.data(Qt::UserRole + 1).toInt() == 1)
        {
            painter->save();
            QString name = index.data(Qt::UserRole + 3).toString();
            if (option.state & QStyle::State_Selected)
            {
                painter->setPen(QQGlobals::g_theme->m_friend_index_name_selected_color);
            }
            painter->setFont(QQGlobals::g_theme->m_friend_index_name_font);
            QRect nameRect = QQFunctions::getCalculateTextRect(name, painter->font());
            nameRect.moveTopLeft(QPoint(iconRect.right() + m_commonSpace, iconRect.top()));
            painter->drawText(nameRect, Qt::AlignCenter, name);
            painter->restore();
        }
    }
}

QSize QQFriendIndexDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.parent().isValid())
    {
        int width = option.rect.width();
        return QSize(width, 40);
    }
    return getItemSize();
}

bool QQFriendIndexDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (index.parent().isValid())
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::RightButton)
            {
                return true;
            }
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}