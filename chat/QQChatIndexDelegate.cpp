#include "QQChatIndexDelegate.h"

QQChatIndexDelegate::QQChatIndexDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    m_commonSpace = 10;
}

QQChatIndexDelegate::~QQChatIndexDelegate()
{
}

void QQChatIndexDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QQConfigs::FriendConfig *user = qvariant_cast<QQConfigs::FriendConfig *>(index.data(Qt::UserRole));
    QQConfigs::GroupConfig *group = qvariant_cast<QQConfigs::GroupConfig *>(index.data(Qt::UserRole));
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    if (user == nullptr && group == nullptr)
    {
        return;
    }
    if (option.state & QStyle::State_MouseOver)
    {
        painter->fillRect(option.rect, QQGlobals::g_theme->m_chat_index_frame_hover_color);
    }
    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, QQGlobals::g_theme->m_chat_index_frame_selected_color);
    }

    painter->save();
    QPixmap icon = index.data(Qt::UserRole + 1).value<QPixmap>();
    QRect iconRect = QRect(QPoint(option.rect.height() / 2 - QQGlobals::g_theme->m_chat_index_icon_size.width() / 2, option.rect.y() + option.rect.height() / 2 - QQGlobals::g_theme->m_chat_index_icon_size.width() / 2), QQGlobals::g_theme->m_chat_index_icon_size);
    painter->drawPixmap(iconRect, icon);
    painter->restore();

    painter->save();
    painter->setFont(QQGlobals::g_theme->m_chat_index_name_font);
    if (option.state & QStyle::State_Selected)
    {
        painter->setPen(QQGlobals::g_theme->m_chat_index_name_selected_color);
    }
    else
    {
        painter->setPen(QQGlobals::g_theme->m_chat_index_name_color);
    }
    QString name = index.data(Qt::UserRole + 2).toString();
    QRect nameRect = QQFunctions::getCalculateTextRect(name, painter->font());
    nameRect.moveTopLeft(QPoint(iconRect.right() + m_commonSpace, iconRect.top()));
    painter->drawText(nameRect, Qt::AlignCenter, name);
    painter->restore();

    painter->save();
    painter->setFont(QQGlobals::g_theme->m_chat_index_message_font);
    if (option.state & QStyle::State_Selected)
    {
        painter->setPen(QQGlobals::g_theme->m_chat_index_message_selected_color);
    }
    else
    {
        painter->setPen(QQGlobals::g_theme->m_chat_index_message_color);
    }
    QString message = index.data(Qt::UserRole + 3).toString();
    QRect messageRect = QQFunctions::getCalculateTextRect(message, painter->font());
    messageRect.moveTopLeft(QPoint(nameRect.left(), nameRect.bottom()));
    painter->drawText(messageRect, Qt::AlignCenter, message);
    painter->restore();

    painter->save();
    painter->setFont(QQGlobals::g_theme->m_chat_index_time_font);
    if (option.state & QStyle::State_Selected)
    {
        painter->setPen(QQGlobals::g_theme->m_chat_index_time_selected_color);
    }
    else
    {
        painter->setPen(QQGlobals::g_theme->m_chat_index_time_color);
    }
    QString time = index.data(Qt::UserRole + 4).toString();
    QRect timeRect = QQFunctions::getCalculateTextRect(time, painter->font());
    timeRect.moveTopLeft(QPoint(option.rect.width() - timeRect.width() - 2.5 * m_commonSpace, iconRect.top() + 0.2 * m_commonSpace));
    painter->drawText(timeRect, Qt::AlignCenter, time);
    painter->restore();

    QRect stateRect = QRect(timeRect.left() + m_commonSpace, timeRect.bottom() + m_commonSpace, 2 * m_commonSpace, 2 * m_commonSpace);
    int newMsgCount = index.data(Qt::UserRole + 5).toInt();
    if (newMsgCount <= 0)
    {
    }
    else if (user != nullptr && user->m_state == QQConfigs::FriendConfig::UserStateType::distrup ||
             group != nullptr && group->m_state == QQConfigs::GroupConfig::GroupStateType::disnotice)
    {
        painter->setFont(QQGlobals::g_theme->m_chat_index_count_font);
        painter->setBrush(Qt::gray);
        painter->setPen(Qt::transparent);
        painter->drawEllipse(stateRect);
        painter->setPen(QColor(255, 255, 255));
        if (newMsgCount < 100)
        {
            painter->drawText(stateRect, Qt::AlignCenter, QString::number(newMsgCount));
        }
        else
        {
            painter->drawText(stateRect, Qt::AlignCenter, QString("99+"));
        }
    }
    else if (newMsgCount > 0)
    {
        painter->setFont(QFont("Microsoft YaHei", 7));
        painter->setBrush(Qt::red);
        painter->setPen(Qt::transparent);
        painter->drawEllipse(stateRect);
        painter->setPen(QColor(255, 255, 255));
        if (newMsgCount < 100)
        {
            painter->drawText(stateRect, Qt::AlignCenter, QString::number(newMsgCount));
        }
        else
        {
            painter->drawText(stateRect, Qt::AlignCenter, QString("99+"));
        }
    }
}

QSize QQChatIndexDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return getItemSize();
}