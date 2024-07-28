#include "QQCustomDelegate.h"

QQChatMessageSingleViewDelegate::QQChatMessageSingleViewDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    m_iconSize = QSize(40, 40);
}

QQChatMessageSingleViewDelegate::~QQChatMessageSingleViewDelegate()
{
}

// UserRole--0:消息体, 1:头像, 2:名字--昵称, 3:消息内容--文本,文件大小,链接,system,time, 4:图片内容--图片,文件图标, 5:是否发送中, 6:是否下载中, 7:下载进度
void QQChatMessageSingleViewDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ClientConfigs::MessageBaseConfig *message = qvariant_cast<ClientConfigs::MessageBaseConfig *>(index.data(Qt::UserRole));
    if (message == nullptr)
    {
        // 尾部空白
        return QStyledItemDelegate::paint(painter, option, index);
    }
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    QPixmap icon = index.data(Qt::UserRole + 1).value<QPixmap>();
    icon = getRounedPixmap(icon.scaled(m_iconSize, Qt::KeepAspectRatio), m_iconSize.width() / 2);
    QString name = index.data(Qt::UserRole + 2).toString();
    int contentWidth = option.rect.width() - 2 * m_contentMargin;
    QRect frame;

    switch (message->m_senderType)
    {
    case ClientConfigs::MessageBaseConfig::SenderType::me:
    {
        painter->save();
        QRect iconRect = QRect(QPoint(option.rect.width() - m_iconMargin - m_iconSize.width(), option.rect.y() + m_iconMargin), m_iconSize);
        painter->drawPixmap(iconRect, icon);
        painter->restore();
        painter->save();
        painter->setFont(QFont("Microsoft YaHei", 9));
        painter->setPen(Qt::gray);
        QRect nameRect = getCalculateTextRects(name, painter->font(), contentWidth, true);
        nameRect.moveTopLeft(QPoint(option.rect.width() - m_contentMargin - nameRect.width(), iconRect.top()));
        painter->drawText(nameRect, name);
        painter->restore();
        switch (message->m_messageType)
        {
        case ClientConfigs::MessageBaseConfig::MessageType::text:
        {
            QString textContent = index.data(Qt::UserRole + 3).toString();
            painter->save();
            painter->setFont(QFont("Microsoft YaHei", 10));
            painter->setPen(Qt::black);
            painter->setBrush(QBrush(Qt::white));
            QRect textRect = getCalculateTextRects(textContent, painter->font(), contentWidth - 2 * m_contentPadding);
            frame = QRect(0, 0, textRect.width() + 2 * m_contentPadding, textRect.height() + 2 * m_contentPadding);
            frame.moveTopLeft(QPoint(option.rect.width() - m_contentMargin - frame.width(), nameRect.bottom() + m_nameMargin));
            textRect.moveTopLeft(frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
            QPainterPath path;
            path.addRoundedRect(frame, 10, 10);
            painter->fillPath(path, Qt::white);
            painter->drawText(textRect, textContent);
            painter->restore();
            break;
        }
        case ClientConfigs::MessageBaseConfig::MessageType::image:
        {
            QPixmap imageContent = index.data(Qt::UserRole + 4).value<QPixmap>();
            painter->save();
            imageContent = imageContent.scaled(imageContent.size().boundedTo(QSize(300, 300)), Qt::KeepAspectRatio);
            imageContent = getRounedPixmap(imageContent, 10);
            frame = QRect(QPoint(option.rect.width() - m_contentMargin - imageContent.width(), nameRect.bottom() + m_nameMargin), imageContent.size());
            painter->drawPixmap(frame, imageContent);
            painter->restore();
            break;
        }
        case ClientConfigs::MessageBaseConfig::MessageType::file:
        {
            painter->save();
            QSize frameSize(280, 110);
            QSize filem_iconSize(50, 50);
            frame = QRect(QPoint(option.rect.width() - m_contentMargin - frameSize.width(), nameRect.bottom() + m_nameMargin), frameSize);
            painter->setFont(QFont("Microsoft YaHei", 10));
            painter->setPen(Qt::black);
            QPainterPath path;
            path.addRoundedRect(frame, 10, 10);
            painter->fillPath(path, Qt::white);
            QFileInfo fileInfo(QString::fromLocal8Bit(message->m_content.c_str()));
            QString realFileName = fileInfo.fileName();
            QRect fileNameRect = getCalculateTextRects(realFileName, painter->font(), frameSize.width() - 2 * m_contentPadding - filem_iconSize.width() - 12 * m_nameMargin);
            fileNameRect.moveTopLeft(frame.topLeft() + QPoint(1.5 * m_contentPadding, 1.5 * m_contentPadding));
            painter->drawText(fileNameRect, realFileName);
            QRect fileIconRect = QRect(QPoint(frame.right() - 1.5 * m_contentPadding - filem_iconSize.width(), frame.top() + 1.5 * m_contentPadding), filem_iconSize);
            QPixmap fileIcon = index.data(Qt::UserRole + 4).value<QPixmap>();
            painter->drawPixmap(fileIconRect, fileIcon);
            painter->setFont(QFont("Microsoft YaHei", 9));
            painter->setPen(Qt::gray);
            QString textContent = index.data(Qt::UserRole + 3).toString();
            QRect fileSizeRect = getCalculateTextRects(textContent, painter->font(), frameSize.width() - 2 * m_contentPadding);
            fileSizeRect.moveTopLeft(QPoint(frame.left() + 1.5 * m_contentPadding, frame.bottom() - 1.5 * m_contentPadding - fileSizeRect.height()));
            painter->drawText(fileSizeRect, textContent);
            painter->restore();
            break;
        }
        case ClientConfigs::MessageBaseConfig::MessageType::link:
        {
            QString textContent = index.data(Qt::UserRole + 3).toString();
            painter->save();
            QFont font = QFont("Microsoft YaHei", 10);
            font.setUnderline(true);
            painter->setFont(font);
            painter->setPen(Qt::blue);
            QRect linkRect = getCalculateTextRects(textContent, painter->font(), contentWidth - 2 * m_contentPadding);
            frame = QRect(0, 0, linkRect.width() + 2 * m_contentPadding, linkRect.height() + 2 * m_contentPadding);
            frame.moveTopLeft(QPoint(option.rect.width() - m_contentMargin - frame.width(), nameRect.bottom() + m_nameMargin));
            linkRect.moveTopLeft(frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
            QPainterPath path;
            path.addRoundedRect(frame, 10, 10);
            painter->fillPath(path, Qt::white);
            painter->drawText(linkRect, textContent);
            painter->restore();
            break;
        }
        case ClientConfigs::MessageBaseConfig::MessageType::emoji:
        {
            QString textContent = index.data(Qt::UserRole + 3).toString();
            painter->save();
            QString emojiType = textContent.split(",")[0].split(":")[1];
            QString emojiData = textContent.split(",")[1].split(":")[1];
            if (emojiType == "unicode")
            {
                painter->setFont(QFont("Segoe UI Emoji", 15));
                QRect emojiRect = getCalculateTextRects(emojiData, painter->font(), contentWidth - 2 * m_contentPadding);
                frame = QRect(0, 0, emojiRect.width() + 2 * m_contentPadding, emojiRect.height() + 2 * m_contentPadding);
                frame.moveTopLeft(QPoint(option.rect.width() - m_contentMargin - frame.width(), nameRect.bottom() + m_nameMargin));
                emojiRect.moveTopLeft(frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
                QPainterPath path;
                path.addRoundedRect(frame, 10, 10);
                painter->fillPath(path, Qt::white);
                painter->drawText(emojiRect, emojiData);
            }
            painter->restore();
            break;
        }
        default:
            break;
        }
        QSize stateIconSize(30, 30);
        QRect stateRect;
        QPixmap stateIcon;
        if (index.data(Qt::UserRole + 5).toBool())
        {
            stateRect = QRect(QPoint(0, 0), stateIconSize);
            stateRect.moveBottom(frame.bottom());
            stateRect.moveLeft(frame.left() - stateIconSize.width() - m_nameMargin);
            painter->drawPixmap(stateRect, stateIcon);
        }
        else if (!message->m_isSended)
        {
            stateRect = QRect(QPoint(0, 0), stateIconSize.boundedTo(QSize(20, 20)));
            stateRect.moveBottom(frame.bottom());
            stateRect.moveLeft(frame.left() - stateIconSize.boundedTo(QSize(20, 20)).width() - m_nameMargin);
            stateIcon = QPixmap(":/widget/common/images/widget/common/error.png");
            painter->drawPixmap(stateRect, stateIcon);
        }
        break;
    }
    case ClientConfigs::MessageBaseConfig::SenderType::she:
    {
        painter->save();
        QRect iconRect = QRect(QPoint(m_iconMargin, m_iconMargin), m_iconSize);
        painter->drawPixmap(iconRect, icon);
        painter->restore();
        painter->save();
        painter->setFont(QFont("Microsoft YaHei", 9));
        painter->setPen(Qt::gray);
        QRect nameRect = getCalculateTextRects(name, painter->font(), contentWidth, true);
        nameRect.moveTopLeft(QPoint(m_contentMargin, m_iconMargin));
        painter->drawText(nameRect, name);
        painter->restore();
        switch (message->m_messageType)
        {
        case ClientConfigs::MessageBaseConfig::MessageType::text:
        {
            QString textContent = index.data(Qt::UserRole + 3).toString();
            painter->save();
            painter->setFont(QFont("Microsoft YaHei", 10));
            painter->setPen(Qt::black);
            painter->setBrush(QBrush(Qt::white));
            QRect textRect = getCalculateTextRects(textContent, painter->font(), contentWidth - 2 * m_contentPadding);
            frame = QRect(0, 0, textRect.width() + 2 * m_contentPadding, textRect.height() + 2 * m_contentPadding);
            frame.moveTopLeft(QPoint(m_contentMargin, nameRect.bottom() + m_nameMargin));
            textRect.moveTopLeft(frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
            QPainterPath path;
            path.addRoundedRect(frame, 10, 10);
            painter->fillPath(path, Qt::white);
            painter->drawText(textRect, textContent);
            painter->restore();
            break;
        }
        case ClientConfigs::MessageBaseConfig::MessageType::image:
        {
            QPixmap imageContent = index.data(Qt::UserRole + 4).value<QPixmap>();
            painter->save();
            imageContent = imageContent.scaled(imageContent.size().boundedTo(QSize(option.rect.width() / 2 - m_contentMargin, 300)), Qt::KeepAspectRatio);
            imageContent = getRounedPixmap(imageContent, 10);
            frame = QRect(QPoint(m_contentMargin, nameRect.bottom() + m_nameMargin), imageContent.size());
            painter->drawPixmap(frame, imageContent);
            painter->restore();
            break;
        }
        case ClientConfigs::MessageBaseConfig::MessageType::file:
        {
            painter->save();
            QSize frameSize(280, 110);
            QSize filem_iconSize(50, 50);
            frame = QRect(QPoint(m_contentMargin, nameRect.bottom() + m_nameMargin), frameSize);
            painter->setFont(QFont("Microsoft YaHei", 10));
            painter->setPen(Qt::black);
            QPainterPath path;
            path.addRoundedRect(frame, 10, 10);
            painter->fillPath(path, Qt::white);
            QString fileName = QString::fromLocal8Bit(message->m_content.c_str());
            QFileInfo fileInfo(fileName);
            QString realFileName = fileInfo.fileName();
            QRect fileNameRect = getCalculateTextRects(realFileName, painter->font(), frameSize.width() - 2 * m_contentPadding - filem_iconSize.width() - 12 * m_nameMargin);
            fileNameRect.moveTopLeft(frame.topLeft() + QPoint(1.5 * m_contentPadding, 1.5 * m_contentPadding));
            painter->drawText(fileNameRect, realFileName);
            QRect fileIconRect = QRect(QPoint(frame.right() - 1.5 * m_contentPadding - filem_iconSize.width(), frame.top() + 1.5 * m_contentPadding), filem_iconSize);
            QPixmap fileIcon = index.data(Qt::UserRole + 4).value<QPixmap>();
            painter->drawPixmap(fileIconRect, fileIcon);

            if (message->m_isDownLoaded.has_value() && !message->m_isDownLoaded)
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
            else if (index.data(Qt::UserRole + 6).toBool())
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
                painter->drawArc(fileCancelDownLoadIconRect, 90 * 16, -index.data(Qt::UserRole + 7).toInt() * 16);
                painter->drawPixmap(fileCancelDownLoadIconRect, QPixmap(":/file/images/file/cancel.png"));
                painter->restore();
            }

            painter->setFont(QFont("Microsoft YaHei", 9));
            painter->setPen(Qt::gray);
            QString textContent = index.data(Qt::UserRole + 3).toString();
            QRect fileSizeRect = getCalculateTextRects(textContent, painter->font(), frameSize.width() - 2 * m_contentPadding);
            fileSizeRect.moveTopLeft(QPoint(frame.left() + 1.5 * m_contentPadding, frame.bottom() - 1.5 * m_contentPadding - fileSizeRect.height()));
            painter->drawText(fileSizeRect, textContent);
            painter->restore();
            break;
        }
        case ClientConfigs::MessageBaseConfig::MessageType::link:
        {
            QString textContent = index.data(Qt::UserRole + 3).toString();
            painter->save();
            painter->setFont(QFont("Microsoft YaHei", 10));
            QFont font = painter->font();
            font.setUnderline(true);
            painter->setFont(font);
            painter->setPen(Qt::blue);
            QRect linkRect = getCalculateTextRects(textContent, painter->font(), contentWidth - 2 * m_contentPadding);
            frame = QRect(0, 0, linkRect.width() + 2 * m_contentPadding, linkRect.height() + 2 * m_contentPadding);
            frame.moveTopLeft(QPoint(m_contentMargin, nameRect.bottom() + m_nameMargin));
            linkRect.moveTopLeft(frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
            QPainterPath path;
            path.addRoundedRect(frame, 10, 10);
            painter->fillPath(path, Qt::white);
            painter->drawText(linkRect, textContent);
            painter->restore();
            break;
        }
        case ClientConfigs::MessageBaseConfig::MessageType::emoji:
        {
            QString textContent = index.data(Qt::UserRole + 3).toString();
            painter->save();
            QString emojiType = textContent.split(",")[0].split(":")[1];
            QString emojiData = textContent.split(",")[1].split(":")[1];
            if (emojiType == "unicode")
            {
                painter->setFont(QFont("Segoe UI Emoji", 15));
                QRect emojiRect = getCalculateTextRects(emojiData, painter->font(), contentWidth - 2 * m_contentPadding);
                frame = QRect(0, 0, emojiRect.width() + 2 * m_contentPadding, emojiRect.height() + 2 * m_contentPadding);
                frame.moveTopLeft(QPoint(m_contentMargin, nameRect.bottom() + m_nameMargin));
                emojiRect.moveTopLeft(frame.topLeft() + QPoint(m_contentPadding, m_contentPadding));
                QPainterPath path;
                path.addRoundedRect(frame, 10, 10);
                painter->fillPath(path, Qt::white);
                painter->drawText(emojiRect, emojiData);
            }
            painter->restore();
            break;
        }
        default:
            break;
        }
        break;
    }
    case ClientConfigs::MessageBaseConfig::SenderType::system:
    {
        painter->save();
        painter->setPen(Qt::gray);
        painter->setFont(QFont("Microsoft YaHei", 9));
        QString system = index.data(Qt::UserRole + 3).toString();
        frame = getCalculateTextRects(system, painter->font(), contentWidth);
        frame.moveTopLeft(QPoint(m_contentMargin, m_iconMargin));
        frame.setWidth(contentWidth);
        painter->drawText(frame, Qt::AlignCenter, system);
        painter->restore();
        break;
    }
    case ClientConfigs::MessageBaseConfig::SenderType::time:
    {
        painter->save();
        painter->setFont(QFont("Microsoft YaHei", 9));
        painter->setPen(Qt::gray);
        QString time = index.data(Qt::UserRole + 3).toString();
        frame = getCalculateTextRects(time, painter->font(), contentWidth);
        frame.moveTopLeft(QPoint(m_contentMargin, 2 * m_iconMargin));
        frame.setWidth(contentWidth);
        painter->drawText(frame, Qt::AlignCenter, time);
        painter->restore();
        break;
    }
    default:
        break;
    }
}

QSize QQChatMessageSingleViewDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ClientConfigs::MessageBaseConfig *message = qvariant_cast<ClientConfigs::MessageBaseConfig *>(index.data(Qt::UserRole));
    if (message == nullptr)
    {
        // 尾部空白
        return QStyledItemDelegate::sizeHint(option, index);
    }
    QString name = index.data(Qt::UserRole + 2).toString();
    int contentWidth = option.rect.width() - 2 * m_contentMargin;
    QRect frame;
    switch (message->m_senderType)
    {
    case ClientConfigs::MessageBaseConfig::SenderType::me:
    case ClientConfigs::MessageBaseConfig::SenderType::she:
    {
        QRect iconRect = QRect(QPoint(option.rect.width() - m_iconMargin - m_iconSize.width(), option.rect.y() + m_iconMargin), m_iconSize);
        QRect nameRect = getCalculateTextRects(name, QFont("Microsoft YaHei", 9), contentWidth, true);
        switch (message->m_messageType)
        {
        case ClientConfigs::MessageBaseConfig::MessageType::text:
        {
            QString textContent = index.data(Qt::UserRole + 3).toString();
            QRect textRect = getCalculateTextRects(textContent, QFont("Microsoft YaHei", 10), contentWidth - 2 * m_contentPadding);
            frame = QRect(0, 0, textRect.width() + 2 * m_contentPadding, textRect.height() + 2 * m_contentPadding);
            break;
        }
        case ClientConfigs::MessageBaseConfig::MessageType::image:
        {
            QPixmap imageContent = index.data(Qt::UserRole + 4).value<QPixmap>();
            imageContent = imageContent.scaled(imageContent.size().boundedTo(QSize(option.rect.width() / 2 - m_contentMargin, 300)), Qt::KeepAspectRatio);
            imageContent = getRounedPixmap(imageContent, 10);
            frame = QRect(QPoint(0, 0), imageContent.size());
            break;
        }
        case ClientConfigs::MessageBaseConfig::MessageType::file:
        {
            QSize frameSize(280, 110);
            frame = QRect(QPoint(0, 0), frameSize);
            break;
        }
        case ClientConfigs::MessageBaseConfig::MessageType::link:
        {
            QString textContent = index.data(Qt::UserRole + 3).toString();
            QFont font = QFont("Microsoft YaHei", 10);
            font.setUnderline(true);
            QRect linkRect = getCalculateTextRects(textContent, font, contentWidth - 2 * m_contentPadding);
            frame = QRect(0, 0, linkRect.width() + 2 * m_contentPadding, linkRect.height() + 2 * m_contentPadding);
            break;
        }
        case ClientConfigs::MessageBaseConfig::MessageType::emoji:
        {
            QString textContent = index.data(Qt::UserRole + 3).toString();
            QString emojiType = textContent.split(",")[0].split(":")[1];
            QString emojiData = textContent.split(",")[1].split(":")[1];
            if (emojiType == "unicode")
            {
                QRect emojiRect = getCalculateTextRects(emojiData, QFont("Segoe UI Emoji", 15), contentWidth - 2 * m_contentPadding);
                frame = QRect(0, 0, emojiRect.width() + 2 * m_contentPadding, emojiRect.height() + 2 * m_contentPadding);
            }
            break;
        }
        default:
            break;
        }
        return QSize(option.rect.width(), frame.height() + nameRect.height() + m_iconMargin + m_nameMargin);
    }
    case ClientConfigs::MessageBaseConfig::SenderType::system:
    {
        QString system = index.data(Qt::UserRole + 3).toString();
        frame = getCalculateTextRects(system, QFont("Microsoft YaHei", 9), contentWidth);
        return QSize(option.rect.width(), frame.height() + 2 * m_iconMargin);
    }
    case ClientConfigs::MessageBaseConfig::SenderType::time:
    {
        QString time = index.data(Qt::UserRole + 3).toString();
        frame = getCalculateTextRects(time, QFont("Microsoft YaHei", 9), contentWidth);
        return QSize(option.rect.width(), frame.height() + 4 * m_iconMargin);
    }
    default:
        break;
    }
    return QStyledItemDelegate::sizeHint(option, index);
}

bool QQChatMessageSingleViewDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QQSinglePeopleWidgetDelegate::QQSinglePeopleWidgetDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    m_iconSize = QSize(50, 50);
    m_maxStringWidth = 200;
    m_commonSpace = 10;
}

QQSinglePeopleWidgetDelegate::~QQSinglePeopleWidgetDelegate()
{
}
// UserRole--0:用户或是群信息, 1:widget类型, 2:头像, 3:名字--昵称, 4:最新消息内容--签名,账号, 5:消息时间, 6:新消息数量 , 7:消息状态Icon
void QQSinglePeopleWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    ClientConfigs::UserFriend_C *user = qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole));
    ClientConfigs::UserGroup_C *group = qvariant_cast<ClientConfigs::UserGroup_C *>(index.data(Qt::UserRole));
    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    if (user == nullptr && group == nullptr)
    {
        return;
    }
    if (option.state & QStyle::State_MouseOver)
    {
        painter->fillRect(option.rect, QColor("#F2F2F2"));
    }
    if (option.state & QStyle::State_Selected)
    {
        painter->fillRect(option.rect, QColor("#0099FF"));
    }
    QPixmap icon = index.data(Qt::UserRole + 2).value<QPixmap>();
    icon = getRounedPixmap(icon.scaled(m_iconSize, Qt::KeepAspectRatio), m_iconSize.width() / 2);
    painter->save();
    QRect iconRect = QRect(QPoint(option.rect.height() / 2 - m_iconSize.width() / 2, option.rect.y() + option.rect.height() / 2 - m_iconSize.width() / 2), m_iconSize);
    painter->drawPixmap(iconRect, icon);
    painter->restore();
    ClientEnums::PeopleWidgetType type = qvariant_cast<ClientEnums::PeopleWidgetType>(index.data(Qt::UserRole + 1));
    switch (type)
    {
    case ClientEnums::PeopleWidgetType::friendchats:
    case ClientEnums::PeopleWidgetType::groupchats:
    {
        painter->save();
        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(Qt::white);
        }
        QString name = index.data(Qt::UserRole + 3).toString();
        painter->setFont(QFont("Microsoft YaHei", 12));
        QRect nameRect = getCalculateTextRects(name, painter->font(), m_maxStringWidth, true);
        nameRect.moveTopLeft(QPoint(iconRect.right() + m_commonSpace, iconRect.top()));
        painter->drawText(nameRect, name, Qt::AlignLeft | Qt::AlignVCenter);
        painter->setFont(QFont("Microsoft YaHei", 10));
        QString message = index.data(Qt::UserRole + 4).toString();
        QRect messageRect = getCalculateTextRects(message, painter->font(), m_maxStringWidth, true);
        messageRect.moveTopLeft(QPoint(nameRect.left(), nameRect.bottom()));
        painter->drawText(messageRect, message, Qt::AlignLeft | Qt::AlignVCenter);
        painter->setFont(QFont("Microsoft YaHei", 8));
        QString time = index.data(Qt::UserRole + 5).toString();
        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(Qt::white);
        }
        else
        {
            painter->setPen(Qt::gray);
        }
        QRect timeRect = getCalculateTextRects(time, painter->font());
        timeRect.moveTopLeft(QPoint(option.rect.width() - timeRect.width() - 2.5 * m_commonSpace, iconRect.top() + 0.2 * m_commonSpace));
        painter->setFont(QFont("Microsoft YaHei", 8));
        painter->drawText(timeRect, time, Qt::AlignLeft | Qt::AlignVCenter);
        QRect stateRect = QRect(timeRect.left() + m_commonSpace, timeRect.bottom() + m_commonSpace, 2 * m_commonSpace, 2 * m_commonSpace);
        int newMsgCount = index.data(Qt::UserRole + 6).toInt();
        if (newMsgCount <= 0)
        {
            QIcon stateIcon = index.data(Qt::UserRole + 7).value<QIcon>();
            if (option.state & QStyle::State_Selected)
            {
                painter->drawPixmap(stateRect, stateIcon.pixmap(stateRect.size(), QIcon::Normal, QIcon::On));
            }
            else
            {
                painter->drawPixmap(stateRect, stateIcon.pixmap(stateRect.size(), QIcon::Normal, QIcon::Off));
            }
        }
        else if (user != nullptr && user->m_state == ClientConfigs::UserFriend_C::UserStateType::distrup ||
                 group != nullptr && group->m_state == ClientConfigs::UserGroup_C::GroupStateType::disnotice)
        {
            painter->setFont(QFont("Microsoft YaHei", 7));
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
                painter->drawText(stateRect, Qt::AlignCenter, "99+");
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
                painter->drawText(stateRect, Qt::AlignCenter, "99+");
            }
        }
        painter->restore();
        break;
    }
    case ClientEnums::PeopleWidgetType::friends:
    {
        QString name = index.data(Qt::UserRole + 3).toString();
        painter->save();
        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(Qt::white);
        }
        painter->setFont(QFont("Microsoft YaHei", 12));
        QRect nameRect = getCalculateTextRects(name, painter->font(), m_maxStringWidth, true);
        nameRect.moveTopLeft(QPoint(iconRect.right() + m_commonSpace, iconRect.top()));
        painter->drawText(nameRect, name, Qt::AlignLeft | Qt::AlignVCenter);
        painter->setFont(QFont("Microsoft YaHei", 10));
        QString message = index.data(Qt::UserRole + 4).toString();
        QRect messageRect = getCalculateTextRects(message, painter->font(), m_maxStringWidth, true);
        messageRect.moveTopLeft(QPoint(nameRect.left(), nameRect.bottom()));
        painter->drawText(messageRect, message, Qt::AlignLeft | Qt::AlignVCenter);
        painter->restore();
        break;
    }
    case ClientEnums::PeopleWidgetType::groups:
    {
        QString name = index.data(Qt::UserRole + 3).toString();
        painter->save();
        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(Qt::white);
        }
        painter->setFont(QFont("Microsoft YaHei", 12));
        QRect nameRect = getCalculateTextRects(name, painter->font(), m_maxStringWidth, true);
        nameRect.moveTopLeft(QPoint(iconRect.right() + m_commonSpace, option.rect.height() / 2 - nameRect.height() / 2));
        painter->drawText(nameRect, name, Qt::AlignLeft | Qt::AlignVCenter);
        painter->restore();
        break;
    }
    case ClientEnums::PeopleWidgetType::friendappend:
    case ClientEnums::PeopleWidgetType::groupappend:
    {
        QString name = index.data(Qt::UserRole + 3).toString();
        painter->save();
        painter->setFont(QFont("Microsoft YaHei", 12));
        QRect nameRect = getCalculateTextRects(name, painter->font(), m_maxStringWidth, true);
        nameRect.moveTopLeft(QPoint(iconRect.right() + m_commonSpace, iconRect.top()));
        painter->drawText(nameRect, name, Qt::AlignLeft | Qt::AlignTop);
        painter->restore();
        QString account = index.data(Qt::UserRole + 4).toString();
        painter->save();
        painter->setFont(QFont("Microsoft YaHei", 10));
        painter->setPen(Qt::gray);
        QRect messageRect = getCalculateTextRects(account, painter->font(), m_maxStringWidth, true);
        messageRect.moveTopLeft(QPoint(nameRect.left(), nameRect.bottom()));
        painter->drawText(messageRect, account, Qt::AlignLeft | Qt::AlignVCenter);
        painter->restore();
        break;
    }
    default:
        break;
    }
}

QSize QQSinglePeopleWidgetDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return m_itemSize;
}

bool QQSinglePeopleWidgetDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QQSingleFriendWidgetDelegate::QQSingleFriendWidgetDelegate(QObject *parent)
    : QQSinglePeopleWidgetDelegate(parent)
{
    m_iconSize = QSize(50, 50);
    m_commonSpace = 10;
    this->setMMAXStringWidth(200);
}

QQSingleFriendWidgetDelegate::~QQSingleFriendWidgetDelegate()
{
}

void QQSingleFriendWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.parent().isValid())
    {
        QStyleOptionViewItem opt = option;
        opt.rect.adjust(20, 0, -20, 0);
        painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
        if (option.state & QStyle::State_MouseOver)
        {
            painter->fillRect(option.rect, QColor("#F2F2F2"));
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
    return QQSinglePeopleWidgetDelegate::paint(painter, option, index);
}

QSize QQSingleFriendWidgetDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.parent().isValid())
    {
        int width = option.rect.width();
        return QSize(width, 40);
    }
    return QQSinglePeopleWidgetDelegate::sizeHint(option, index);
}

bool QQSingleFriendWidgetDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
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
    return QQSinglePeopleWidgetDelegate::editorEvent(event, model, option, index);
}