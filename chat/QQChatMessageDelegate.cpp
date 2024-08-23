#include "QQChatMessageDelegate.h"

QQChatMessageDelegate::QQChatMessageDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

QQChatMessageDelegate::~QQChatMessageDelegate()
{
}

void QQChatMessageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);

    switch (index.data(Qt::UserRole + 1).toInt())
    {
    case static_cast<int>(QQConfigs::MessageConfig::SenderType::me):
    {
        painter->save();
        QPixmap icon = index.data(Qt::UserRole + 1).value<QPixmap>();
        QRect iconRect = QRect(QPoint(option.rect.width() - m_iconMargin - m_iconSize.width(), option.rect.y() + m_iconMargin), m_iconSize);
        painter->drawPixmap(iconRect, icon);
        painter->restore();

        painter->save();
        painter->setPen(QQGlobals::g_theme->m_chat_message_name_color);
        painter->setFont(QQGlobals::g_theme->m_chat_message_name_font);
        QString name = index.data(Qt::UserRole + 2).toString();
        QRect nameRect = QQFunctions::getCalculateTextRect(name, painter->font());
        nameRect.moveTopLeft(QPoint(option.rect.width() - m_contentMargin - nameRect.width(), iconRect.top()));
        painter->drawText(nameRect, Qt::AlignCenter, name);
        painter->restore();
        QRect frame;
        switch (index.data(Qt::UserRole + 2).toInt())
        {
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::text):
        {
            painter->save();
            painter->setPen(QQGlobals::g_theme->m_chat_message_text_color);
            painter->setFont(QQGlobals::g_theme->m_chat_message_text_font);
            QString text = index.data(Qt::UserRole + 3).toString();
            QRect textRect = QQFunctions::getCalculateTextRect(text, painter->font());
            frame = QRect(0, 0, textRect.width() + 2 * m_contentPadding, textRect.height() + 2 * m_contentPadding);
            frame.moveTopLeft(QPoint(option.rect.width() - m_contentMargin - frame.width(), nameRect.bottom() + m_nameMargin));
            textRect.moveTopLeft(frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
            QPainterPath path;
            path.addRoundedRect(frame, 10, 10);
            painter->fillPath(path, Qt::white);
            painter->drawText(textRect, text);
            painter->restore();
            break;
        }
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::image):
        {
            painter->save();
            QPixmap image = index.data(Qt::UserRole + 4).value<QPixmap>();
            frame = QRect(QPoint(option.rect.width() - m_contentMargin - image.width(), nameRect.bottom() + m_nameMargin), image.size());
            painter->drawPixmap(frame, image);
            painter->restore();
            break;
        }
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::file):
        {
            painter->save();
            frame = QRect(QPoint(option.rect.width() - m_contentMargin - QQGlobals::g_theme->m_chat_message_file_frame_size.width(), nameRect.bottom() + m_nameMargin),
                          QQGlobals::g_theme->m_chat_message_file_frame_size);
            QPainterPath path;
            path.addRoundedRect(frame, 10, 10);
            painter->fillPath(path, Qt::white);

            painter->setPen(QQGlobals::g_theme->m_chat_message_file_name_color);
            painter->setFont(QQGlobals::g_theme->m_chat_message_file_name_font);
            QFileInfo fileInfo(index.data(Qt::UserRole + 3).toString());
            QRect fileNameRect = QQFunctions::getCalculateTextRect(fileInfo.fileName(), painter->font());
            fileNameRect.moveTopLeft(frame.topLeft() + QPoint(1.5 * m_contentPadding, 1.5 * m_contentPadding));
            painter->drawText(fileNameRect, fileInfo.fileName(), Qt::AlignLeft | Qt::AlignCenter);

            QRect fileIconRect = QRect(QPoint(frame.right() - 1.5 * m_contentPadding - QQGlobals::g_theme->m_chat_message_file_icon_size.width(), frame.top() + 1.5 * m_contentPadding),
                                       QQGlobals::g_theme->m_chat_message_file_icon_size);
            QPixmap fileIcon = index.data(Qt::UserRole + 4).value<QPixmap>();
            painter->drawPixmap(fileIconRect, fileIcon);

            painter->setPen(QQGlobals::g_theme->m_chat_message_file_size_color);
            painter->setFont(QQGlobals::g_theme->m_chat_message_file_size_font);
            QString fileSize = index.data(Qt::UserRole + 10).toString();
            QRect fileSizeRect = QQFunctions::getCalculateTextRect(fileSize, painter->font());
            fileSizeRect.moveTopLeft(QPoint(frame.left() + 1.5 * m_contentPadding, frame.bottom() - 1.5 * m_contentPadding - fileSizeRect.height()));
            painter->drawText(fileSizeRect, Qt::AlignCenter, fileSize);
            painter->restore();
            break;
        }
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::link):
        {
            painter->save();
            painter->setPen(QQGlobals::g_theme->m_chat_message_link_color);
            QFont font = QQGlobals::g_theme->m_chat_message_link_font;
            font.setUnderline(true);
            painter->setFont(font);
            QString link = index.data(Qt::UserRole + 3).toString();
            QRect linkRect = QQFunctions::getCalculateTextRect(link, painter->font());
            frame = QRect(0, 0, linkRect.width() + 2 * m_contentPadding, linkRect.height() + 2 * m_contentPadding);
            frame.moveTopLeft(QPoint(option.rect.width() - m_contentMargin - frame.width(), nameRect.bottom() + m_nameMargin));
            linkRect.moveTopLeft(frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
            QPainterPath path;
            path.addRoundedRect(frame, 10, 10);
            painter->fillPath(path, Qt::white);
            painter->drawText(linkRect, link, Qt::AlignLeft | Qt::AlignCenter);
            painter->restore();
            break;
        }
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::emoji):
        {
            // QString emoji = index.data(Qt::UserRole + 3).toString();
            // painter->save();
            // QString emojiType = emoji.split(",")[0].split(":")[1];
            // QString emojiData = emoji.split(",")[1].split(":")[1];
            // if (emojiType == "unicode")
            // {
            //     painter->setFont(QQGlobals::g_theme->m_chat_message_emoji_font);
            //     QRect emojiRect = QQFunctions::getCalculateTextRect(emojiData, painter->font());
            //     frame = QRect(0, 0, emojiRect.width() + 2 * m_contentPadding, emojiRect.height() + 2 * m_contentPadding);
            //     frame.moveTopLeft(QPoint(option.rect.width() - m_contentMargin - frame.width(), nameRect.bottom() + m_nameMargin));
            //     emojiRect.moveTopLeft(frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
            //     QPainterPath path;
            //     path.addRoundedRect(frame, 10, 10);
            //     painter->fillPath(path, Qt::white);
            //     painter->drawText(emojiRect, emojiData);
            // }
            // painter->restore();
            break;
        }
        default:
            break;
        }
        if (index.data(Qt::UserRole + 7).toBool())
        {
            QRect stateRect = QRect(QPoint(0, 0), QQGlobals::g_theme->m_chat_message_state_icon_size);
            stateRect.moveBottom(frame.bottom());
            stateRect.moveLeft(frame.left() - QQGlobals::g_theme->m_chat_message_state_icon_size.width() - m_nameMargin);
            painter->drawPixmap(stateRect, QQGlobals::g_theme->m_chat_message_state_icon.pixmap(QQGlobals::g_theme->m_chat_message_state_icon_size));
        }
        else if (!index.data(Qt::UserRole + 8).toBool())
        {
            QRect stateRect = QRect(QPoint(0, 0), QQGlobals::g_theme->m_chat_message_state_icon_size);
            stateRect.moveBottom(frame.bottom());
            stateRect.moveLeft(frame.left() - QQGlobals::g_theme->m_chat_message_state_icon_size.width() - m_nameMargin);
            painter->drawPixmap(stateRect, QQGlobals::g_theme->m_chat_message_state_icon.pixmap(QQGlobals::g_theme->m_chat_message_state_icon_size));
        }
        break;
    }
    case static_cast<int>(QQConfigs::MessageConfig::SenderType::she):
    {
        painter->save();
        QPixmap icon = index.data(Qt::UserRole + 1).value<QPixmap>();
        QRect iconRect = QRect(QPoint(m_iconMargin, m_iconMargin), m_iconSize);
        painter->drawPixmap(iconRect, icon);
        painter->restore();

        painter->save();
        painter->setPen(QQGlobals::g_theme->m_chat_message_name_color);
        painter->setFont(QQGlobals::g_theme->m_chat_message_name_font);
        QString name = index.data(Qt::UserRole + 2).toString();
        QRect nameRect = QQFunctions::getCalculateTextRect(name, painter->font());
        nameRect.moveTopLeft(QPoint(m_contentMargin, m_iconMargin));
        painter->drawText(nameRect, Qt::AlignCenter, name);
        painter->restore();

        switch (index.data(Qt::UserRole + 2).toInt())
        {
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::text):
        {
            painter->save();
            painter->setPen(QQGlobals::g_theme->m_chat_message_text_color);
            painter->setFont(QQGlobals::g_theme->m_chat_message_text_font);
            QString text = index.data(Qt::UserRole + 3).toString();
            QRect textRect = QQFunctions::getCalculateTextRect(text, painter->font());
            QRect frame = QRect(0, 0, textRect.width() + 2 * m_contentPadding, textRect.height() + 2 * m_contentPadding);
            frame.moveTopLeft(QPoint(m_contentMargin, nameRect.bottom() + m_nameMargin));
            textRect.moveTopLeft(frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
            QPainterPath path;
            path.addRoundedRect(frame, 10, 10);
            painter->fillPath(path, Qt::white);
            painter->drawText(textRect, text, Qt::AlignLeft | Qt::AlignCenter);
            painter->restore();
            break;
        }
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::image):
        {
            painter->save();
            QPixmap image = index.data(Qt::UserRole + 4).value<QPixmap>();
            QRect frame = QRect(QPoint(m_contentMargin, nameRect.bottom() + m_nameMargin), image.size());
            painter->drawPixmap(frame, image);
            painter->restore();
            break;
        }
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::file):
        {
            painter->save();
            QRect frame = QRect(QPoint(m_contentMargin, nameRect.bottom() + m_nameMargin), QQGlobals::g_theme->m_chat_message_file_frame_size);
            QPainterPath path;
            path.addRoundedRect(frame, 10, 10);
            painter->fillPath(path, Qt::white);

            painter->setPen(QQGlobals::g_theme->m_chat_message_file_name_color);
            painter->setFont(QQGlobals::g_theme->m_chat_message_file_name_font);
            QFileInfo fileInfo(index.data(Qt::UserRole + 3).toString());
            QRect fileNameRect = QQFunctions::getCalculateTextRect(fileInfo.fileName(), painter->font());
            fileNameRect.moveTopLeft(frame.topLeft() + QPoint(1.5 * m_contentPadding, 1.5 * m_contentPadding));
            painter->drawText(fileNameRect, fileInfo.fileName(), Qt::AlignLeft | Qt::AlignCenter);

            QRect fileIconRect = QRect(QPoint(frame.right() - 1.5 * m_contentPadding - QQGlobals::g_theme->m_chat_message_file_icon_size.width(), frame.top() + 1.5 * m_contentPadding),
                                       QQGlobals::g_theme->m_chat_message_file_icon_size);
            QPixmap fileIcon = index.data(Qt::UserRole + 4).value<QPixmap>();
            painter->drawPixmap(fileIconRect, fileIcon);

            if (index.data(Qt::UserRole + 9).toInt() == 0)
            {
                painter->save();
                QPainterPath iconPath;
                iconPath.addRoundedRect(fileIconRect, 10, 10);
                painter->fillPath(iconPath, QColor(0, 0, 0, 50));
                QRect fileDownLoadIconRect = QRect(fileIconRect.topLeft() + QPoint(2 * m_nameMargin, 2 * m_nameMargin), QSize(30, 30));
                QPen pen;
                pen.setColor(Qt::white);
                pen.setWidth(2);
                painter->setPen(pen);
                painter->drawEllipse(fileDownLoadIconRect);
                painter->drawPixmap(fileDownLoadIconRect, QPixmap(":/file/images/file/download.png"));
                painter->restore();
            }
            else if (index.data(Qt::UserRole + 9).toInt() != 100)
            {
                painter->save();
                QPainterPath iconPath;
                iconPath.addRoundedRect(fileIconRect, 10, 10);
                painter->fillPath(iconPath, QColor(0, 0, 0, 50));
                QRect fileCancelDownLoadIconRect = QRect(fileIconRect.topLeft() + QPoint(2 * m_nameMargin, 2 * m_nameMargin), QSize(30, 30));
                QPen pen;
                pen.setColor(Qt::white);
                pen.setWidth(2);
                painter->setPen(pen);
                painter->drawArc(fileCancelDownLoadIconRect, 90 * 16, -index.data(Qt::UserRole + 6).toInt() * 16);
                painter->drawPixmap(fileCancelDownLoadIconRect, QPixmap(":/file/images/file/cancel.png"));
                painter->restore();
            }

            painter->setPen(QQGlobals::g_theme->m_chat_message_file_size_color);
            painter->setFont(QQGlobals::g_theme->m_chat_message_file_size_font);
            QString fileSize = index.data(Qt::UserRole + 10).toString();
            QRect fileSizeRect = QQFunctions::getCalculateTextRect(fileSize, painter->font());
            fileSizeRect.moveTopLeft(QPoint(frame.left() + 1.5 * m_contentPadding, frame.bottom() - 1.5 * m_contentPadding - fileSizeRect.height()));
            painter->drawText(fileSizeRect, fileSize);
            painter->restore();
            break;
        }
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::link):
        {
            painter->save();
            painter->setPen(QQGlobals::g_theme->m_chat_message_link_color);
            QFont font = QQGlobals::g_theme->m_chat_message_link_font;
            font.setUnderline(true);
            painter->setFont(font);
            QString link = index.data(Qt::UserRole + 3).toString();
            QRect linkRect = QQFunctions::getCalculateTextRect(link, painter->font());
            QRect frame = QRect(0, 0, linkRect.width() + 2 * m_contentPadding, linkRect.height() + 2 * m_contentPadding);
            frame.moveTopLeft(QPoint(m_contentMargin, nameRect.bottom() + m_nameMargin));
            linkRect.moveTopLeft(frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
            QPainterPath path;
            path.addRoundedRect(frame, 10, 10);
            painter->fillPath(path, Qt::white);
            painter->drawText(linkRect, link, Qt::AlignLeft | Qt::AlignCenter);
            painter->restore();
            break;
        }
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::emoji):
        {
            // QString textContent = index.data(Qt::UserRole + 3).toString();
            // painter->save();
            // QString emojiType = textContent.split(",")[0].split(":")[1];
            // QString emojiData = textContent.split(",")[1].split(":")[1];
            // if (emojiType == "unicode")
            // {
            //     painter->setFont(QFont("Segoe UI Emoji", 15));
            //     QRect emojiRect = QQFunctions::getCalculateTextRect(emojiData, painter->font());
            //     QRect frame = QRect(0, 0, emojiRect.width() + 2 * m_contentPadding, emojiRect.height() + 2 * m_contentPadding);
            //     frame.moveTopLeft(QPoint(m_contentMargin, nameRect.bottom() + m_nameMargin));
            //     emojiRect.moveTopLeft(frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
            //     QPainterPath path;
            //     path.addRoundedRect(frame, 10, 10);
            //     painter->fillPath(path, Qt::white);
            //     painter->drawText(emojiRect, emojiData);
            // }
            // painter->restore();
            break;
        }
        default:
            break;
        }
        break;
    }
    case static_cast<int>(QQConfigs::MessageConfig::SenderType::system):
    {
        painter->save();
        painter->setPen(QQGlobals::g_theme->m_chat_message_system_color);
        painter->setFont(QQGlobals::g_theme->m_chat_message_system_font);
        QString system = index.data(Qt::UserRole + 3).toString();
        QRect frame = QQFunctions::getCalculateTextRect(system, painter->font());
        frame.moveTopLeft(QPoint(m_contentMargin, m_iconMargin));
        frame.setWidth(option.rect.width());
        painter->drawText(frame, Qt::AlignCenter, system);
        painter->restore();
        break;
    }
    case static_cast<int>(QQConfigs::MessageConfig::SenderType::time):
    {
        painter->save();
        painter->setPen(QQGlobals::g_theme->m_chat_message_time_color);
        painter->setFont(QQGlobals::g_theme->m_chat_message_time_font);
        QString time = index.data(Qt::UserRole + 3).toString();
        QRect frame = QQFunctions::getCalculateTextRect(time, painter->font());
        frame.moveTopLeft(QPoint(m_contentMargin, 2 * m_iconMargin));
        frame.setWidth(option.rect.width());
        painter->drawText(frame, Qt::AlignCenter, time);
        painter->restore();
        break;
    }
    default:
        return QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize QQChatMessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.data(Qt::UserRole + 1).toInt())
    {
    case static_cast<int>(QQConfigs::MessageConfig::SenderType::me):
    case static_cast<int>(QQConfigs::MessageConfig::SenderType::she):
    {
        int height = 0;
        switch (index.data(Qt::UserRole + 2).toInt())
        {
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::text):
        {
            QRect textRect = QQFunctions::getCalculateTextRect(index.data(Qt::UserRole + 3).toString(), QQGlobals::g_theme->m_chat_message_text_font);
            height = textRect.height() + 2 * m_contentPadding;
            break;
        }
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::image):
        {
            QPixmap image = index.data(Qt::UserRole + 4).value<QPixmap>();
            height = image.size().height();
            break;
        }
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::file):
        {
            height = QQGlobals::g_theme->m_chat_message_file_frame_size.height();
            break;
        }
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::link):
        {
            QRect linkRect = QQFunctions::getCalculateTextRect(index.data(Qt::UserRole + 3).toString(), QQGlobals::g_theme->m_chat_message_link_font);
            height = linkRect.height() + 2 * m_contentPadding;
            break;
        }
        case static_cast<int>(QQConfigs::MessageConfig::MessageType::emoji):
        {
            // QString textContent = index.data(Qt::UserRole + 3).toString();
            // QString emojiType = textContent.split(",")[0].split(":")[1];
            // QString emojiData = textContent.split(",")[1].split(":")[1];
            // if (emojiType == "unicode")
            // {
            //     QRect emojiRect = QQFunctions::getCalculateTextRect(emojiData, QQGlobals::g_theme->m_chat_message_emoji_font);
            //     height = emojiRect.height() + 2 * m_contentPadding;
            // }
            break;
        }
        default:
            break;
        }
        QRect nameRect = QQFunctions::getCalculateTextRect(index.data(Qt::UserRole + 2).toString(), QQGlobals::g_theme->m_chat_message_name_font);
        return QSize(option.rect.width(), height + nameRect.height() + m_iconMargin + m_nameMargin);
    }
    case static_cast<int>(QQConfigs::MessageConfig::SenderType::system):
    {
        QRect frame = QQFunctions::getCalculateTextRect(index.data(Qt::UserRole + 3).toString(), QQGlobals::g_theme->m_chat_message_time_font);
        return QSize(option.rect.width(), frame.height() + 2 * m_iconMargin);
    }
    case static_cast<int>(QQConfigs::MessageConfig::SenderType::time):
    {
        QRect frame = QQFunctions::getCalculateTextRect(index.data(Qt::UserRole + 3).toString(), QQGlobals::g_theme->m_chat_message_system_font);
        return QSize(option.rect.width(), frame.height() + 4 * m_iconMargin);
    }
    default:
        break;
    }
    return QStyledItemDelegate::sizeHint(option, index);
}