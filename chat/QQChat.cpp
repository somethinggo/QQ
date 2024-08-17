#include "QQChat.h"

void QQChatProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == PE_Frame)
	{
		return;
	}
	else if (element == PE_FrameLineEdit)
	{
		return;
	}
	return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void QQChatProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	QString name = widget->objectName();
	if (element == CE_PushButton)
	{
		QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
		// 鼠标移入加深颜色，鼠标按下的颜色加深
		if (name == "clearInputBtn" ||
			name == "seneMsgBtn")
		{
			if (buttonOption && (buttonOption->state & State_MouseOver))
			{
				buttonOption->state &= ~State_MouseOver;
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
		// 鼠标移入与按下改变图片
		if (name == "emojiBtn" ||
			name == "fileBtn" ||
			name == "pictureBtn" ||
			name == "upperBtn" ||
			name == "fontBtn" ||
			name == "historyBtn" ||
			name == "goToAudioTabBtn")
		{
			if (buttonOption && (buttonOption->state & State_MouseOver))
			{
				buttonOption->state &= ~State_MouseOver;
				if (!buttonOption->icon.isNull())
				{
					buttonOption->icon = buttonOption->icon.pixmap(buttonOption->iconSize, QIcon::Active, QIcon::On);
				}
			}
			if (buttonOption && (buttonOption->state & State_Sunken))
			{
				buttonOption->state &= ~State_Sunken;
				buttonOption->state &= ~State_MouseOver;
				if (!buttonOption->icon.isNull())
				{
					buttonOption->icon = buttonOption->icon.pixmap(buttonOption->iconSize, QIcon::Active, QIcon::On);
				}
			}
		}
		// 选中的背景板
		if (name == "upperBtn")
		{
			if (buttonOption && (buttonOption->state & State_On))
			{
				buttonOption->state &= ~State_On;
				buttonOption->icon = buttonOption->icon.pixmap(buttonOption->iconSize, QIcon::Active, QIcon::On);
				painter->save();
				painter->setRenderHint(QPainter::Antialiasing);
				painter->setPen(Qt::NoPen);
				painter->setBrush(QColor("#CCE4F7"));
				painter->drawEllipse(option->rect);
				painter->restore();
			}
		}
	}
	return QProxyStyle::drawControl(element, option, painter, widget);
}

QRect QQChatProxyStyle::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
	QString name = widget->objectName();
	if (element == SE_ShapedFrameContents)
	{
		if (name == "nicknameLab")
		{
			QRect rect = QProxyStyle::subElementRect(element, option, widget);
			const int padding = 25;
			rect.adjust(padding, 0, 0, 0);
			return rect;
		}
	}
	else if (element == SE_FrameContents)
	{
		if (name == "textInput")
		{
			QRect rect = QProxyStyle::subElementRect(element, option, widget);
			const int padding = 25;
			rect.adjust(padding, 0, -padding, 0);
			return rect;
		}
	}
	return QProxyStyle::subElementRect(element, option, widget);
}

void QMSearchProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
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

void QMSearchProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
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

QQChat::QQChat(QWidget *parent)
	: QWidget(parent), ui(new Ui::QQChatClass())
{
	ui->setupUi(this);
	m_commonStyle = new QQChatProxyStyle;
	m_searchStyle = new QMSearchProxyStyle;
	m_searchAction = new QAction(this);
	m_searchAction->setIcon(QIcon(":/widget/common/images/widget/common/searchLab.png"));
	m_searchMoreMenu = new ElaMenu(this);
	m_searchMoreMenu->addAction(QIcon(":/widget/common/images/widget/common/searchMoreOne.png"), QString::fromLocal8Bit("发起群聊"));
	m_searchMoreMenu->addAction(QIcon(":/widget/common/images/widget/common/searchMoreTwo.png"), QString::fromLocal8Bit("加好友/群"));

	m_indexListModel = new QStandardItemModel(this);
	m_indexListDelegate = new QQChatIndexDelegate(this);
	m_indexListDelegate->setItemSize(QSize(350, 90));
	m_indexListDelegate->setMAXStringWidth(300);
	m_messageListModel = new QStandardItemModel(this);
	m_messageListDelegate = new QQChatMessageDelegate(this);
	m_chatFriendContextMenu = new ElaMenu(this);
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::ArrowUp), QString::fromLocal8Bit("置顶"));
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::Ban), QString::fromLocal8Bit("消息免打扰"));
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::TrashCan), QString::fromLocal8Bit("删除该聊天"));
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::BrakeWarning), QString::fromLocal8Bit("举报"));

	ui->searchInput->setStyle(m_searchStyle);
	ui->searchInput->addAction(m_searchAction, QLineEdit::LeadingPosition);
	ui->searchMoreBtn->setStyle(m_searchStyle);
	ui->searchMoreBtn->setMask(QQFunctions::getRoundedMask(ui->searchMoreBtn->size(), 5));
	ui->indexList->setModel(m_indexListModel);
	ui->indexList->setItemDelegate(m_indexListDelegate);
	ui->nicknameLab->setStyle(m_commonStyle);
	ui->messageList->setModel(m_messageListModel);
	ui->messageList->setItemDelegate(m_messageListDelegate);
	ui->messageList->setStyle(m_commonStyle);
	ui->inputOption->setStyle(m_commonStyle);
	ui->emojiBtn->setStyle(m_commonStyle);
	ui->fileBtn->setStyle(m_commonStyle);
	ui->pictureBtn->setStyle(m_commonStyle);
	ui->upperBtn->setStyle(m_commonStyle);
	ui->histroyBtn->setStyle(m_commonStyle);
	ui->textInput->setStyle(m_commonStyle);
	ui->otherOption->setStyle(m_commonStyle);
	ui->goToAudioTabBtn->setStyle(m_commonStyle);
	ui->clearInputBtn->setMask(QQFunctions::getRoundedMask(ui->clearInputBtn->size(), 10));
	ui->clearInputBtn->setStyle(m_commonStyle);
	ui->sendMsgBtn->setMask(QQFunctions::getRoundedMask(ui->sendMsgBtn->size(), 10));
	ui->sendMsgBtn->setStyle(m_commonStyle);

	qApp->installEventFilter(this);
	connect(ui->searchMoreBtn, &QPushButton::clicked, this, &QQChat::do_userClickSearchMore);
	connect(m_searchMoreMenu, &QMenu::triggered, this, &QQChat::do_userClickSearchMoreAction);
	connect(ui->indexList, &QListView::clicked, this, &QQChat::do_userClickChatIndex);
	connect(m_chatFriendContextMenu, &QMenu::triggered, this, &QQChat::do_userContextMenuAction);
	connect(ui->emojiBtn, &QPushButton::clicked, this, &QQChat::do_userClickSendEmoji);
	connect(ui->fileBtn, &QPushButton::clicked, this, &QQChat::do_userClickSendFile);
	connect(ui->pictureBtn, &QPushButton::clicked, this, &QQChat::do_userClickSendPicture);
	connect(ui->upperBtn, &QPushButton::toggled, this, &QQChat::do_userOpenBigWrite);
	connect(ui->histroyBtn, &QPushButton::clicked, this, &QQChat::do_userClickLookHistroy);
	connect(ui->textInput, &QTextEdit::textChanged, this, &QQChat::do_limitUserInputTextCount);
	connect(ui->goToAudioTabBtn, &QPushButton::clicked, this, &QQChat::do_userClickSendAudio);
	connect(ui->clearInputBtn, &QPushButton::clicked, this, &QQChat::do_userClickClearInput);
	connect(ui->sendMsgBtn, &QPushButton::clicked, this, &QQChat::do_userClickSendMessage);
}

QQChat::~QQChat()
{
	delete m_commonStyle;
	delete m_searchStyle;
	delete ui;
}

bool QQChat::event(QEvent *event)
{
	switch (event->type())
	{
	}
	return QWidget::event(event);
}

bool QQChat::eventFilter(QObject *watch, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		if (mouseEvent->button() == Qt::LeftButton)
		{
			if (QQFunctions::getMouseIsInWidget(ui->searchInput, mouseEvent->globalPos()))
			{
				ui->searchInput->setFocus();
			}
			else
			{
				ui->searchInput->clearFocus();
			}
			if (QQFunctions::getMouseIsInWidget(ui->textInput, mouseEvent->globalPos()))
			{
				ui->textInput->setFocus();
			}
			else
			{
				ui->textInput->clearFocus();
			}
			if (!QQEmoji::getInstance()->isHidden() && !QQFunctions::getMouseIsInWidget(QQEmoji::getInstance(), mouseEvent->globalPos()))
			{
				QQEmoji::getInstance()->hide();
			}
			if (!QQAudio::getInstance()->isHidden() && !QQFunctions::getMouseIsInWidget(QQAudio::getInstance(), mouseEvent->globalPos()))
			{
				QQAudio::getInstance()->hide();
			}
		}
	}
	return QWidget::eventFilter(watch, event);
}

void QQChat::sendMessageToNetWork(const QSharedPointer<QQConfigs::MessageConfig> &message)
{
	QJsonObject sendData;
	sendData.insert("version", "1.0");
	sendData.insert("sender", "chat");
	switch (message->m_messageType)
	{
	case QQConfigs::MessageConfig::MessageType::text:
	{
		sendData.insert("type", "text");
		break;
	}
	case QQConfigs::MessageConfig::MessageType::image:
	{
		sendData.insert("type", "image");
		break;
	}
	case QQConfigs::MessageConfig::MessageType::link:
	{
		sendData.insert("type", "link");
		break;
	}
	}
	QJsonObject data;
	data.insert("senderID", QString::fromStdString(message->m_senderID));
	data.insert("receiverID", QString::fromStdString(message->m_receiverID));
	data.insert("content", QString::fromStdString(message->m_content));
	data.insert("time", QString::number(message->m_time));
	sendData.insert("data", data);
	switch (message->m_messageType)
	{
	case QQConfigs::MessageConfig::MessageType::text:
	{
		Q_MSEND_EVENT(QQEnums::sendtext, QJsonDocument(sendData).toJson());
		break;
	}
	case QQConfigs::MessageConfig::MessageType::image:
	{
		Q_MSEND_EVENT(QQEnums::sendimage, QJsonDocument(sendData).toJson());
		break;
	}
	case QQConfigs::MessageConfig::MessageType::link:
	{
		Q_MSEND_EVENT(QQEnums::sendlink, QJsonDocument(sendData).toJson());
		break;
	}
	}
}

void QQChat::loadPeopleInModelItem(QStandardItem *item, const QQConfigs::FriendConfig &user)
{
	item->setData(QVariant::fromValue(user), Qt::UserRole);

	QPixmap icon = QPixmap::fromImage(QQFunctions::getBase64ToImage(QString::fromStdString(user.m_icon)));
	icon = QQFunctions::getRoundedPixmap(icon.scaled(QQGlobals::g_theme->m_chat_index_icon_size, Qt::KeepAspectRatio), QQGlobals::g_theme->m_chat_index_icon_size.width() / 2);
	item->setData(icon, Qt::UserRole + 2);

	QString name = QString::fromLocal8Bit(user.m_name.c_str());
	if (!user.m_nickname.isNull())
	{
		name = user.m_nickname;
	}
	name = QQFunctions::getCalculateText(name, QQGlobals::g_theme->m_chat_index_name_font, m_indexListDelegate->getItemSize().width() - 100, true);
	item->setData(name, Qt::UserRole + 3);

	QString lastMsg = user.m_lastMsg;
	lastMsg = QQFunctions::getCalculateText(lastMsg, QQGlobals::g_theme->m_chat_index_message_font, m_indexListDelegate->getItemSize().width() - 100, true);
	item->setData(lastMsg, Qt::UserRole + 4);

	QString time = user.m_lastMsgTime.toString();
	qint64 daysDiff = QDateTime::fromString(time).secsTo(QDateTime::currentDateTime()) / (24 * 3600);
	if (daysDiff < 1)
	{
		time = QDateTime::fromString(time).toString("HH:mm");
	}
	else if (daysDiff < 2)
	{
		time = "昨天";
	}
	else if (daysDiff < 7)
	{
		switch (QDateTime::fromString(time).date().dayOfWeek())
		{
		case 1:
			time = "星期一";
			break;
		case 2:
			time = "星期二";
			break;
		case 3:
			time = "星期三";
			break;
		case 4:
			time = "星期四";
			break;
		case 5:
			time = "星期五";
			break;
		case 6:
			time = "星期六";
			break;
		case 7:
			time = "星期日";
			break;
		}
	}
	else
	{
		time = QDateTime::fromString(time).toString("yyyy/MM/dd");
	}
	item->setData(time, Qt::UserRole + 5);

	item->setData(user.m_newMsgCount, Qt::UserRole + 6);
}

void QQChat::loadPeopleInModelItem(QStandardItem *item, const QQConfigs::GroupConfig &group)
{
	item->setData(QVariant::fromValue(group), Qt::UserRole);

	QPixmap icon = QPixmap::fromImage(QQFunctions::getBase64ToImage(QString::fromStdString(group.m_icon)));
	icon = QQFunctions::getRoundedPixmap(icon.scaled(QQGlobals::g_theme->m_chat_index_icon_size, Qt::KeepAspectRatio), QQGlobals::g_theme->m_chat_index_icon_size.width() / 2);
	item->setData(icon, Qt::UserRole + 2);

	QString name = QString::fromLocal8Bit(group.m_name.c_str());
	if (!group.m_nickname.isNull())
	{
		name = group.m_nickname;
	}
	name = QQFunctions::getCalculateText(name, QQGlobals::g_theme->m_chat_index_name_font, m_indexListDelegate->getItemSize().width() - 100, true);
	item->setData(group.m_lastMsg, Qt::UserRole + 4);

	QString lastMsg = group.m_lastMsg;
	lastMsg = QQFunctions::getCalculateText(lastMsg, QQGlobals::g_theme->m_chat_index_message_font, m_indexListDelegate->getItemSize().width() - 100, true);
	item->setData(lastMsg, Qt::UserRole + 4);

	QString time = group.m_lastMsgTime.toString();
	qint64 daysDiff = QDateTime::fromString(time).secsTo(QDateTime::currentDateTime()) / (24 * 3600);
	if (daysDiff < 1)
	{
		time = QDateTime::fromString(time).toString("HH:mm");
	}
	else if (daysDiff < 2)
	{
		time = "昨天";
	}
	else if (daysDiff < 7)
	{
		switch (QDateTime::fromString(time).date().dayOfWeek())
		{
		case 1:
			time = "星期一";
			break;
		case 2:
			time = "星期二";
			break;
		case 3:
			time = "星期三";
			break;
		case 4:
			time = "星期四";
			break;
		case 5:
			time = "星期五";
			break;
		case 6:
			time = "星期六";
			break;
		case 7:
			time = "星期日";
			break;
		}
	}
	else
	{
		time = QDateTime::fromString(time).toString("yyyy/MM/dd");
	}
	item->setData(time, Qt::UserRole + 5);

	item->setData(group.m_newMsgCount, Qt::UserRole + 6);
}

void QQChat::loadMessageInModelItem(QStandardItem *item, const QSharedPointer<QQConfigs::MessageConfig> &message)
{
	item->setData(QVariant::fromValue(message.data()), Qt::UserRole);
	switch (message->m_senderType)
	{
	case QQConfigs::MessageConfig::SenderType::me:
	case QQConfigs::MessageConfig::SenderType::she:
	{
		QPixmap icon = QPixmap::fromImage(QQFunctions::getBase64ToImage(QString::fromStdString(QQGlobals::g_user->m_icon)));
		icon = QQFunctions::getRoundedPixmap(icon.scaled(QQGlobals::g_theme->m_chat_message_icon_size, Qt::KeepAspectRatio), QQGlobals::g_theme->m_chat_message_icon_size.width() / 2);
		item->setData(icon, Qt::UserRole + 1);

		if (message->m_senderType == QQConfigs::MessageConfig::SenderType::she)
		{
			QString nickname = QQFunctions::getUserInfo(QString::fromStdString(message->m_senderID))->m_nickname;
			if (!nickname.isEmpty())
			{
				item->setData(nickname, Qt::UserRole + 2);
			}
			else
			{
				item->setData(QString::fromLocal8Bit(QQFunctions::getUserInfo(QString::fromStdString(message->m_senderID))->m_name.c_str()), Qt::UserRole + 2);
			}
		}

		switch (message->m_messageType)
		{
		case QQConfigs::MessageConfig::MessageType::text:
		{
			QString text = QString::fromLocal8Bit(message->m_content.c_str());
			text = QQFunctions::getCalculateText(text, QQGlobals::g_theme->m_chat_message_text_font, ui->messageList->width() - 100);
			item->setData(text, Qt::UserRole + 3);
			break;
		}
		case QQConfigs::MessageConfig::MessageType::image:
		{
			QPixmap image = QPixmap::fromImage(QQFunctions::getBase64ToImage(QString::fromStdString(message->m_content)));
			image = QQFunctions::getRoundedPixmap(image.scaled(QQGlobals::g_theme->m_chat_message_image_frame_max_size, Qt::KeepAspectRatio), 10);
			item->setData(image, Qt::UserRole + 4);
			break;
		}
		case QQConfigs::MessageConfig::MessageType::file:
		{
			QString fileName = QString::fromLocal8Bit(message->m_content.c_str());
			QPair<qreal, QString> pair = QQFunctions::getFileFormatSize(QFile(fileName).size());
			QString fileSize = QString::number(pair.first, 'f', 2) + " " + pair.second;
			item->setData(fileSize, Qt::UserRole + 3);
			QPixmap fileIcon = QFileIconProvider().icon(QFileInfo(fileName)).pixmap(QQGlobals::g_theme->m_chat_message_file_icon_size);
			item->setData(fileIcon, Qt::UserRole + 4);
			break;
		}
		case QQConfigs::MessageConfig::MessageType::link:
		{
			QString link = QString::fromLocal8Bit(message->m_content.c_str());
			link = QQFunctions::getCalculateText(link, QQGlobals::g_theme->m_chat_message_link_font, ui->messageList->width() - 100);
			item->setData(link, Qt::UserRole + 3);
			break;
		}
		case QQConfigs::MessageConfig::MessageType::emoji:
		{
			item->setData(QString::fromStdString(message->m_content.c_str()), Qt::UserRole + 3);
			break;
		}
		default:
			break;
		}
		break;
	}
	case QQConfigs::MessageConfig::SenderType::system:
	{
		QString system = QString::fromLocal8Bit(message->m_content.c_str());
		system = QQFunctions::getCalculateText(system, QQGlobals::g_theme->m_chat_message_system_font, ui->messageList->width() - 100, true);
		item->setData(system, Qt::UserRole + 3);
		break;
	}
	case QQConfigs::MessageConfig::SenderType::time:
	{
		QString time = QString::fromStdString(message->m_content.c_str());
		qint64 daysDiff = QDateTime::fromString(time).secsTo(QDateTime::currentDateTime()) / (24 * 3600);
		if (daysDiff < 1)
		{
			time = QDateTime::fromString(time).toString("HH:mm");
		}
		else if (daysDiff < 2)
		{
			time = "昨天";
			time += " " + QDateTime::fromString(time).toString("HH:mm");
		}
		else if (daysDiff < 7)
		{
			switch (QDateTime::fromString(time).date().dayOfWeek())
			{
			case 1:
				time = "星期一";
				break;
			case 2:
				time = "星期二";
				break;
			case 3:
				time = "星期三";
				break;
			case 4:
				time = "星期四";
				break;
			case 5:
				time = "星期五";
				break;
			case 6:
				time = "星期六";
				break;
			case 7:
				time = "星期日";
				break;
			}
			time += " " + QDateTime::fromString(time).toString("HH:mm");
		}
		else
		{
			time = QDateTime::fromString(time).toString("yyyy/MM/dd");
			time += " " + QDateTime::fromString(time).toString("HH:mm");
		}
		item->setData(time, Qt::UserRole + 3);
		break;
	}
	default:
		break;
	}
	// 设置标志位
	switch (message->m_senderType)
	{
	case QQConfigs::MessageConfig::SenderType::me:
	{
		item->setData(false, Qt::UserRole + 5);
		if (message->m_messageType == QQConfigs::MessageConfig::MessageType::file)
		{
			item->setData(true, Qt::UserRole + 6);
		}
		break;
	}
	case QQConfigs::MessageConfig::SenderType::system:
	case QQConfigs::MessageConfig::SenderType::time:
	{
		item->setData(true, Qt::UserRole + 5);
		break;
	}
	}
}

void QQChat::sendFileByName(const QString &fileName)
{
	// 更新界面
	QModelIndex index = ui->indexList->currentIndex();
	QQConfigs::FriendConfig *user = const_cast<QQConfigs::FriendConfig *>(qvariant_cast<QQConfigs::FriendConfig *>(index.data(Qt::UserRole)));
	QQConfigs::GroupConfig *group = const_cast<QQConfigs::GroupConfig *>(qvariant_cast<QQConfigs::GroupConfig *>(index.data(Qt::UserRole)));
	QSharedPointer<QQConfigs::MessageConfig> message(new QQConfigs::MessageConfig);
	message->m_messageType = QQConfigs::MessageConfig::MessageType::file;
	message->m_senderType = QQConfigs::MessageConfig::SenderType::me;
	if (user != nullptr)
	{
		message->m_receiverID = user->m_ID;
	}
	else if (group != nullptr)
	{
		message->m_receiverID = group->m_ID;
	}
	message->m_content = fileName.toStdString();
	message->m_time = QDateTime::currentDateTime().toSecsSinceEpoch();
	message->m_isDownLoaded.emplace(true);
	appendMessage(message);

	// 请求单独的文件模块进行处理与发送
	QJsonObject sendData;
	sendData.insert("version", "1.0");
	sendData.insert("sender", "chat");
	sendData.insert("action", "requestfile");
	QJsonObject data;
	data.insert("senderID", QString::fromStdString(QQGlobals::g_user->m_ID));
	data.insert("receiverID", QString::fromStdString(message->m_receiverID));
	data.insert("fileName", fileName);
	sendData.insert("data", data);
	Q_MSEND_EVENT(QQEnums::sendfile, QJsonDocument(sendData).toJson());
}

void QQChat::appendMessage(const QSharedPointer<QQConfigs::MessageConfig> &message, int index)
{
	QStandardItem *item = new QStandardItem;
	QListView *listView = ui->messageList;
	loadMessageInModelItem(item, message);
	if (index == -1)
	{
		m_messageListModel->insertRow(m_messageListModel->rowCount() - 1, item);
		listView->scrollToBottom();
		QModelIndex currentIndex = ui->indexList->currentIndex();
		QQConfigs::FriendConfig *user = const_cast<QQConfigs::FriendConfig *>(qvariant_cast<QQConfigs::FriendConfig *>(currentIndex.data(Qt::UserRole)));
		QQConfigs::GroupConfig *group = const_cast<QQConfigs::GroupConfig *>(qvariant_cast<QQConfigs::GroupConfig *>(currentIndex.data(Qt::UserRole)));
		if (user != nullptr)
		{
			user->m_msgList.append(message);
		}
		else if (group != nullptr)
		{
			group->m_msgList.append(message);
		}
		if (message->m_senderType == QQConfigs::MessageConfig::SenderType::me)
		{
			sendMessageToNetWork(message);
		}
	}
	else
	{
		m_messageListModel->setItem(index, item);
	}
}

void QQChat::loadMessages(QQConfigs::FriendConfig *user)
{
	// 预分配空间
	m_messageListModel->insertRows(0, user->m_msgList.count());
	for (int i = 0; i < user->m_msgList.count(); ++i)
	{
		int index = user->m_msgList.count() - i - 1;
		QSharedPointer<QQConfigs::MessageConfig> message = user->m_msgList.at(index);
		appendMessage(message, index);
	}
}

void QQChat::loadMessages(QQConfigs::GroupConfig *group)
{
	// 预分配空间
	m_messageListModel->insertRows(0, group->m_msgList.count());
	for (int i = 0; i < group->m_msgList.count(); ++i)
	{
		int index = group->m_msgList.count() - i - 1;
		QSharedPointer<QQConfigs::MessageConfig> message = group->m_msgList.at(index);
		appendMessage(message, index);
	}
}

void QQChat::do_userClickSearchMore()
{
	QPoint pos = qobject_cast<QPushButton *>(sender())->mapToGlobal(QPoint(0, 0));
	QPoint searchMoreMenuPos = pos + QPoint(5, qobject_cast<QPushButton *>(sender())->height());
	m_searchMoreMenu->popup(searchMoreMenuPos);
}

void QQChat::do_userClickSearchMoreAction(QAction *action)
{
	if (action == m_searchMoreMenu->actions()[0])
	{
	}
	else if (action == m_searchMoreMenu->actions()[1])
	{
	}
}

void QQChat::do_userContextMenuAction(QAction *action)
{
	QModelIndex index = ui->indexList->indexAt(QCursor::pos());
	if (action == m_chatFriendContextMenu->actions()[0])
	{
		m_indexListModel->insertRow(0, m_indexListModel->takeRow(index.row())[0]);
	}
	else if (action == m_chatFriendContextMenu->actions()[1])
	{
		QString text = action->text();
		QQConfigs::FriendConfig *user = const_cast<QQConfigs::FriendConfig *>(qvariant_cast<QQConfigs::FriendConfig *>(index.data(Qt::UserRole)));
		QQConfigs::GroupConfig *group = const_cast<QQConfigs::GroupConfig *>(qvariant_cast<QQConfigs::GroupConfig *>(index.data(Qt::UserRole)));

		if (text == QString::fromLocal8Bit("消息免打扰"))
		{
			action->setText(QString::fromLocal8Bit("取消免打扰"));
			if (user != nullptr)
			{
				user->m_state = QQConfigs::FriendConfig::UserStateType::distrup;
			}
			else if (group != nullptr)
			{
				group->m_state = QQConfigs::GroupConfig::GroupStateType::disnotice;
			}
		}
		else if (text == QString::fromLocal8Bit("取消免打扰"))
		{
			action->setText(QString::fromLocal8Bit("消息免打扰"));
			if (user != nullptr)
			{
				user->m_state = QQConfigs::FriendConfig::UserStateType::online;
			}
			else if (group != nullptr)
			{
				group->m_state = QQConfigs::GroupConfig::GroupStateType::normal;
			}
		}
	}
	else if (action == m_chatFriendContextMenu->actions()[2])
	{
		m_haveInChatList.remove(qvariant_cast<QQConfigs::FriendConfig *>(index.data(Qt::UserRole))->m_ID);
		// 清除默认的移除选中项,选中项效果下移
		if (ui->indexList->selectionModel()->isSelected(index))
		{
			ui->indexList->selectionModel()->clear();
			ui->chatStackWidget->setCurrentIndex(0);
		};
		m_indexListModel->removeRow(index.row());
		// 如果没有聊天索引,则隐藏聊天界面
		if (m_indexListModel->rowCount() == 0)
		{
			ui->chatStackWidget->setCurrentIndex(0);
			ui->indexStackWidget->setCurrentIndex(0);
		}
	}
	else if (action == m_chatFriendContextMenu->actions()[3])
	{
		QQWidgets::MessageDialog *messageDialog = QQWidgets::MessageDialog::getInstance();
		messageDialog->removeAllWidget();
		QPoint pos = this->mapToGlobal(QPoint(0, 0)) + QPoint(this->width() / 2 - 200, this->height() / 2 - 100);
		messageDialog->setGeometry(QRect(pos, QSize(400, 100)));
		messageDialog->setTittle(QString::fromLocal8Bit("举报聊天"));
		messageDialog->setOkButtonEnable(false);
		QLabel *label = new QLabel(messageDialog);
		label->setText(QString::fromLocal8Bit("您确定要举报该聊天?"));
		label->setFixedHeight(40);
		label->setAlignment(Qt::AlignCenter);
		label->setFont(QFont("Microsoft YaHei", 11));
		messageDialog->addWidget(label);
		QTextEdit *textEdit = new QTextEdit(messageDialog);
		textEdit->setFont(QFont("Microsoft YaHei", 11));
		textEdit->setPlaceholderText(QString::fromLocal8Bit("请说明举报理由"));
		textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		messageDialog->addWidget(textEdit);
		messageDialog->layout()->setSpacing(10);
		messageDialog->layout()->setContentsMargins(20, 5, 20, 10);
		connect(textEdit, &QTextEdit::textChanged, [textEdit, messageDialog]()
				{ messageDialog->setOkButtonEnable(!textEdit->toPlainText().isEmpty()); });
		std::function<void()> function = [this]
		{
			// 获取举报信息
			QModelIndex index = ui->indexList->currentIndex();
			QQConfigs::FriendConfig *user = const_cast<QQConfigs::FriendConfig *>(qvariant_cast<QQConfigs::FriendConfig *>(index.data(Qt::UserRole)));
			QQConfigs::GroupConfig *group = const_cast<QQConfigs::GroupConfig *>(qvariant_cast<QQConfigs::GroupConfig *>(index.data(Qt::UserRole)));
			QString report = QQWidgets::MessageDialog::getInstance()->findChild<QTextEdit *>()->toPlainText();
			// 发送举报消息
			QJsonObject reportData;
			reportData.insert("version", "1.0");
			reportData.insert("sender", "chat");
			reportData.insert("action", "sendreport");
			QJsonObject data;
			if (user != nullptr)
			{
				data.insert("type", "user");
				data.insert("ID", QString::fromStdString(user->m_ID));
			}
			else if (group != nullptr)
			{
				data.insert("type", "group");
				data.insert("ID", QString::fromStdString(group->m_ID));
			}
			data.insert("report", report);
			reportData.insert("data", data);
			Q_MSEND_EVENT(QQEnums::sendreport, QJsonDocument(reportData).toJson());
			// 提示发送成功
			ElaMessageBar::success(ElaMessageBarType::PositionPolicy::TopRight, "", QString::fromLocal8Bit("发送举报消息成功，感谢您的指正！"), 2000, this);
		};
		messageDialog->setFunction(function);
		messageDialog->exec();
	}
}

void QQChat::do_userClickChatIndex(const QModelIndex &index)
{
	// 如果当前聊天索引与点击索引相同,则不做任何操作--避免重复加载
	if (m_activeChatIndex.isValid())
	{
		if (m_activeChatIndex == index)
		{
			return;
		}
	}
	ui->chatStackWidget->setCurrentIndex(1);
	QQConfigs::FriendConfig *user = const_cast<QQConfigs::FriendConfig *>(qvariant_cast<QQConfigs::FriendConfig *>(index.data(Qt::UserRole)));
	QQConfigs::GroupConfig *group = const_cast<QQConfigs::GroupConfig *>(qvariant_cast<QQConfigs::GroupConfig *>(index.data(Qt::UserRole)));
	// 更新聊天界面的信息与模型的信息
	if (user != nullptr)
	{
		user->m_newMsgCount = 0;
		m_indexListModel->itemFromIndex(index)->setData(0, Qt::UserRole + 6);
		ui->nicknameLab->setText(user->m_nickname);
	}
	else if (group != nullptr)
	{
		group->m_newMsgCount = 0;
		m_indexListModel->itemFromIndex(index)->setData(0, Qt::UserRole + 6);
		ui->nicknameLab->setText(group->m_nickname);
	}
	m_messageListModel->clear();
	if (user != nullptr)
	{
		loadMessages(user);
	}
	else if (group != nullptr)
	{
		loadMessages(group);
	}
	// 尾部空白--主要是美观
	QStandardItem *item = new QStandardItem;
	int width = ui->messageList->width();
	item->setSizeHint(QSize(width, 25));
	m_messageListModel->appendRow(item);
	m_haveInChatList.insert(user->m_ID, m_indexListModel->itemFromIndex(index));
	QTimer::singleShot(50, [this]
					   { ui->messageList->scrollToBottom(); });
	m_activeChatIndex = index;
}

void QQChat::do_userClickSendEmoji()
{
	QPoint pos = ui->emojiBtn->mapToGlobal(QPoint(0, 0));
	pos.setY(pos.y() - QQEmoji::getInstance()->height() - 10);
	QQEmoji::getInstance()->setGeometry(QRect(pos, QQEmoji::getInstance()->size()));
	QQEmoji::getInstance()->show();
}

void QQChat::do_userClickSendFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择文件"), "C:/", QString::fromLocal8Bit("所有文件(*.*)"));
	sendFileByName(fileName);
}

void QQChat::do_userClickSendPicture()
{
	// 判断是为了判断用户的图片状态是否正常,文件是单独的判读
	QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择图片"), "C:/", QString::fromLocal8Bit("图片文件(*.png *.jpg *.bmp)"));
	if (fileName.isEmpty())
	{
		return;
	}
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}
	file.close();
	// 图片大小超限,则提示是否改为文件发送
	if (file.size() / 1024 / 1024 > QQGlobals::MAX_IMAGE_SIZE)
	{
		QQWidgets::MessageDialog *messageDialog = QQWidgets::MessageDialog::getInstance();
		messageDialog->removeAllWidget();
		QPoint pos = this->mapToGlobal(QPoint(0, 0)) + QPoint(this->width() / 2 - 200, this->height() / 2 - 50);
		messageDialog->setGeometry(QRect(pos, QSize(400, 100)));
		messageDialog->setOkButtonEnable(true);
		QLabel *label = new QLabel(messageDialog);
		label->setText(QString::fromLocal8Bit("图片过大，是否改为文件进行发送?"));
		label->setFixedHeight(40);
		label->setAlignment(Qt::AlignCenter);
		label->setFont(QFont("Microsoft YaHei", 11));
		messageDialog->addWidget(label);
		messageDialog->layout()->setSpacing(10);
		messageDialog->layout()->setContentsMargins(20, 5, 20, 10);
		std::function<void()> function = [this, &fileName]
		{
			sendFileByName(fileName);
		};
		messageDialog->setFunction(function);
		messageDialog->exec();
		return;
	}

	QModelIndex index = ui->indexList->currentIndex();
	QQConfigs::FriendConfig *user = const_cast<QQConfigs::FriendConfig *>(qvariant_cast<QQConfigs::FriendConfig *>(index.data(Qt::UserRole)));
	QQConfigs::GroupConfig *group = const_cast<QQConfigs::GroupConfig *>(qvariant_cast<QQConfigs::GroupConfig *>(index.data(Qt::UserRole)));
	QPair<QString, QString> byteArray = QQFunctions::getImageToBase64(fileName);
	QSharedPointer<QQConfigs::MessageConfig> message(new QQConfigs::MessageConfig);
	message->m_messageType = QQConfigs::MessageConfig::MessageType::image;
	message->m_senderType = QQConfigs::MessageConfig::SenderType::me;
	message->m_senderID = QQGlobals::g_user->m_ID;
	if (user != nullptr)
	{
		message->m_receiverID = user->m_ID;
	}
	else if (group != nullptr)
	{
		message->m_receiverID = group->m_ID;
	}
	message->m_time = QDateTime::currentDateTime().toSecsSinceEpoch();
	message->m_content = byteArray.first.toStdString();
	appendMessage(message);
}

void QQChat::do_userOpenBigWrite(bool isChecked)
{
	// 暂时无法判断是否打开大写锁定--win是可以的,linux与mac无法判断
#ifdef Q_OS_WIN
	keybd_event(VK_CAPITAL, 0x3a, 0, 0);
	keybd_event(VK_CAPITAL, 0x3a, KEYEVENTF_KEYUP, 0);
#elif Q_OS_LINUX
	Display *display = XOpenDisplay(nullptr);
	if (display == nullptr)
	{
		qWarning() << "Unable to open X display";
		return;
	}
	try
	{
		KeyCode keycode = XKeysymToKeycode(display, XK_Caps_Lock);
		XTestFakeKeyEvent(display, keycode, True, 0);
		XTestFakeKeyEvent(display, keycode, False, 0);
		XFlush(display);
	}
	catch (...)
	{
		XCloseDisplay(display);
		throw;
	}
	XCloseDisplay(display);
#elif Q_OS_MAC
	CGEventRef event = CGEventCreateKeyboardEvent(nullptr, kVK_CapsLock, true);
	CGEventPost(kCGHIDEventTap, event);
	CFRelease(event);
	event = CGEventCreateKeyboardEvent(nullptr, kVK_CapsLock, false);
	CGEventPost(kCGHIDEventTap, event);
	CFRelease(event);
#endif
}

void QQChat::do_userClickLookHistroy()
{
}

void QQChat::do_limitUserInputTextCount()
{
	// 统计输入的文本数量
	int count = 0;
	QTextDocument *textDocument = ui->textInput->document();
	for (QTextBlock block = textDocument->begin(); block != textDocument->end(); block = block.next())
	{
		for (QTextBlock::iterator itFragment = block.begin(); !itFragment.atEnd(); ++itFragment)
		{
			QTextFragment fragment = itFragment.fragment();
			if (fragment.isValid() && !fragment.charFormat().isImageFormat())
			{
				count += fragment.text().length();
			}
		}
	}
	// 移动光标,并发出提示信息--间接实现文本长度限制
	if (count > QQGlobals::MAX_MSG_LENGTH)
	{
		ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::BottomLeft, "", QString::fromLocal8Bit("文本数量不能超过500个字"), 2000, this);
		int position = ui->textInput->textCursor().position();
		QTextCursor textCursor = ui->textInput->textCursor();
		textCursor.movePosition(QTextCursor::End);
		if (textCursor.hasSelection())
		{
			textCursor.clearSelection();
		}
		textCursor.deletePreviousChar();
		ui->textInput->setTextCursor(textCursor);
	}
}

void QQChat::do_userClickSendAudio()
{
	QPoint pos = this->mapToGlobal(QPoint(0, 0));
	QWidget *audio = QQAudio::getInstance();
	pos += QPoint(this->width() / 2 - audio->width() / 2, this->height() / 2 - audio->height() / 2);
	audio->move(pos);
	audio->show();
	QJsonObject sendData;
	sendData.insert("version", "1.0");
	sendData.insert("sender", "chat");
	sendData.insert("action", "requestaudio");
	QModelIndex index = ui->indexList->currentIndex();
	QQConfigs::FriendConfig *user = const_cast<QQConfigs::FriendConfig *>(qvariant_cast<QQConfigs::FriendConfig *>(index.data(Qt::UserRole)));
	QQConfigs::GroupConfig *group = const_cast<QQConfigs::GroupConfig *>(qvariant_cast<QQConfigs::GroupConfig *>(index.data(Qt::UserRole)));
	if (user != nullptr)
	{
		sendData.insert("senderID", QString::fromStdString(QQGlobals::g_user->m_ID));
		sendData.insert("receiverID", QString::fromStdString(user->m_ID));
	}
	else if (group != nullptr)
	{
		sendData.insert("senderID", QString::fromStdString(QQGlobals::g_user->m_ID));
		sendData.insert("receiverID", QString::fromStdString(group->m_ID));
	}
	Q_MSEND_EVENT(QQEnums::requestaudio, QJsonDocument(sendData).toJson());
}

void QQChat::do_userClickClearInput()
{
	// 清除输入框的内容,并设置焦点
	ui->textInput->clear();
	ui->textInput->setFocus();
}

void QQChat::do_userClickSendMessage()
{
	if (ui->textInput->toPlainText() == "")
	{
		ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::Bottom, "", QString::fromLocal8Bit("发送内容不能为空"), 3000, this);
		return;
	}
	QModelIndex index = ui->indexList->currentIndex();
	QQConfigs::FriendConfig *user = const_cast<QQConfigs::FriendConfig *>(qvariant_cast<QQConfigs::FriendConfig *>(index.data(Qt::UserRole)));
	QQConfigs::GroupConfig *group = const_cast<QQConfigs::GroupConfig *>(qvariant_cast<QQConfigs::GroupConfig *>(index.data(Qt::UserRole)));
	QSharedPointer<QQConfigs::MessageConfig> message(new QQConfigs::MessageConfig);
	message->m_messageType = QQConfigs::MessageConfig::MessageType::text;
	message->m_senderType = QQConfigs::MessageConfig::SenderType::me;
	message->m_senderID = QQGlobals::g_user->m_ID;
	if (user != nullptr)
	{
		message->m_receiverID = user->m_ID;
	}
	else if (group != nullptr)
	{
		message->m_receiverID = group->m_ID;
	}
	message->m_content = ui->textInput->toPlainText().toLocal8Bit();
	message->m_time = QDateTime::currentDateTime().toSecsSinceEpoch();
	appendMessage(message);
	ui->textInput->clear();
}