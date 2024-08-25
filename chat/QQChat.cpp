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
		if (buttonOption && (buttonOption->state & State_On))
		{
			buttonOption->state &= ~State_On;
			painter->save();
			painter->setRenderHint(QPainter::Antialiasing);
			painter->setPen(Qt::NoPen);
			painter->setBrush(QColor("#CCE4F7"));
			painter->drawEllipse(option->rect);
			painter->restore();
			if (!buttonOption->icon.isNull())
			{
				buttonOption->icon = buttonOption->icon.pixmap(buttonOption->iconSize, QIcon::Active, QIcon::On);
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
		QRect rect = QProxyStyle::subElementRect(element, option, widget);
		const int padding = 25;
		rect.adjust(padding, 0, 0, 0);
		return rect;
	}
	else if (element == SE_FrameContents)
	{
		QRect rect = QProxyStyle::subElementRect(element, option, widget);
		const int padding = 25;
		rect.adjust(padding, 0, -padding, 0);
		return rect;
	}
	return QProxyStyle::subElementRect(element, option, widget);
}

QQChat::QQChat(QWidget *parent)
	: QWidget(parent), ui(new Ui::QQChatClass())
{
	ui->setupUi(this);
	m_commonStyle = new QQChatProxyStyle;
	m_searchAction = new QAction(this);
	m_searchAction->setIcon(QIcon(":/widget/common/images/widget/common/searchLab.png"));
	m_searchMoreMenu = new ElaMenu(this);
	m_searchMoreMenu->addAction(QIcon(":/widget/common/images/widget/common/searchMoreOne.png"), QString::fromLocal8Bit("发起群聊"));
	m_searchMoreMenu->addAction(QIcon(":/widget/common/images/widget/common/searchMoreTwo.png"), QString::fromLocal8Bit("加好友/群"));

	m_indexListModel = new QStandardItemModel(this);
	m_indexListDelegate = new QQChatIndexDelegate(this);
	m_indexListDelegate->setItemSize(QSize(350, 90));
	m_messageListModel = new QStandardItemModel(this);
	m_messageListDelegate = new QQChatMessageDelegate(this);
	m_chatFriendContextMenu = new ElaMenu(this);
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::ArrowUp), QString::fromLocal8Bit("置顶"));
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::Ban), QString::fromLocal8Bit("消息免打扰"));
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::TrashCan), QString::fromLocal8Bit("删除该聊天"));
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::BrakeWarning), QString::fromLocal8Bit("举报"));

	ui->searchInput->setStyle(QQThemes::searchProxyStyle);
	ui->searchInput->addAction(m_searchAction, QLineEdit::LeadingPosition);
	ui->searchMoreBtn->setStyle(QQThemes::searchProxyStyle);
	ui->searchMoreBtn->setMask(QQFunctions::getRoundedMask(ui->searchMoreBtn->size(), 5));
	ui->indexList->setStyle(m_commonStyle);
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

	// 请求同步本地存储聊天索引数据
	QJsonObject sendData;
	sendData.insert("version", *(QQGlobals::g_version));
	sendData.insert("sender", "chat");
	sendData.insert("receiver", "storage");
	sendData.insert("action", "loadindex");
	sendData.insert("data", QJsonObject());
	QQ_SEND_EVENT_GLOBAL(QQEnums::sendmessage, QJsonDocument(sendData).toJson());

	this->installEventFilter(this);
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
	delete ui;
}

bool QQChat::event(QEvent *event)
{
	switch (event->type())
	{
		QQ_HANDLE_EVENT_THIS(QQEnums::receivemessage, handleNetwork)
		QQ_HANDLE_EVENT_THIS(QQEnums::receivestorage, handleStorage)
	case QQEnums::requestmoudel::EventType:
	{
		QJsonObject receiveData = QJsonDocument::fromJson(static_cast<QQEnums::requestmoudel *>(event)->data).object();
		if (receiveData["receiver"].toString() == QString("friend"))
		{
			this->handleFriend(static_cast<QQEnums::requestmoudel *>(event)->data);
		}
		else if (receiveData["receiver"].toString() == QString("emoji"))
		{
			this->handleEmoji(static_cast<QQEnums::requestmoudel *>(event)->data);
		}
		else if (receiveData["receiver"].toString() == QString("audio"))
		{
			this->handleAudio(static_cast<QQEnums::requestmoudel *>(event)->data);
		}
	};
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

void QQChat::resizeEvent(QResizeEvent *event)
{
	// for (auto item : m_slide)
	// {
	// 	QQConfigs::MessageConfig *message = qvariant_cast<QQConfigs::MessageConfig *>(item->data(Qt::UserRole));
	// 	if (message == nullptr)
	// 	{
	// 		continue;
	// 	}
	// 	switch (message->m_messageType)
	// 	{
	// 	case QQConfigs::MessageConfig::MessageType::text:
	// 	{
	// 		QString text = QString::fromLocal8Bit(message->m_content.c_str());
	// 		text = QQFunctions::getCalculateText(text, QQGlobals::g_theme->m_chat_message_text_font, ui->messageList->width() - 100);
	// 		item->setData(text, Qt::UserRole + 3);
	// 		break;
	// 	}
	// 	case QQConfigs::MessageConfig::MessageType::link:
	// 	{
	// 		QString link = QString::fromLocal8Bit(message->m_content.c_str());
	// 		link = QQFunctions::getCalculateText(link, QQGlobals::g_theme->m_chat_message_link_font, ui->messageList->width() - 100);
	// 		item->setData(link, Qt::UserRole + 3);
	// 		break;
	// 	}
	// 	default:
	// 		break;
	// 	}
	// }
	return QWidget::resizeEvent(event);
}

void QQChat::sendMessageToNetWork(const QVariantMap &message)
{
	QJsonObject sendData;
	sendData.insert("version", *(QQGlobals::g_version));
	sendData.insert("sender", "chat");
	sendData.insert("receiver", "network");
	sendData.insert("action", "sendmessage");
	// todo-使用反射序列化对象
	QJsonObject data;
	QString type = QMetaEnum::fromType<QQEnums::MessageTypes>().valueToKey(message["type"].toInt());
	data.insert("type", type);
	data.insert("sender", message["sender"].toString());
	data.insert("receiver", message["receiver"].toString());
	data.insert("content", message["content"].toString());
	data.insert("time", message["time"].toInt());
	sendData.insert("data", data);
	QQ_SEND_EVENT_GLOBAL(QQEnums::sendmessage, QJsonDocument(sendData).toJson());
	sendData.insert("receiver", "storage");
	QQ_SEND_EVENT_GLOBAL(QQEnums::sendmessage, QJsonDocument(sendData).toJson());
}

void QQChat::loadPeopleInModelItem(QStandardItem *item, const QVariantMap &data)
{
	item->setData(data["ID"].toString(), Qt::UserRole);

	QPixmap icon = QPixmap::fromImage(QQFunctions::getBase64ToImage(data["icon"].toString()));
	icon = QQFunctions::getRoundedPixmap(icon.scaled(QQGlobals::g_theme->m_chat_index_icon_size, Qt::KeepAspectRatio), QQGlobals::g_theme->m_chat_index_icon_size.width() / 2);
	item->setData(icon, Qt::UserRole + 1);

	QString name = data["name"].toString();
	if (!data["nickname"].toString().isNull())
	{
		name = data["nickname"].toString();
	}
	name = QQFunctions::getCalculateText(name, QQGlobals::g_theme->m_chat_index_name_font, m_indexListDelegate->getItemSize().width() - 100, true);

	item->setData(name, Qt::UserRole + 2);

	QString lastMsg = data["message"].toString();
	lastMsg = QQFunctions::getCalculateText(lastMsg, QQGlobals::g_theme->m_chat_index_message_font, m_indexListDelegate->getItemSize().width() - 100, true);
	item->setData(lastMsg, Qt::UserRole + 3);

	QString time = data["time"].toString();
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
	item->setData(time, Qt::UserRole + 4);

	item->setData(data["count"].toInt(), Qt::UserRole + 5);
}

void QQChat::loadMessageInModelItem(QStandardItem *item, const QVariantMap &data)
{
	item->setData(data["ID"], Qt::UserRole);
	item->setData(data["sender"].toInt(), Qt::UserRole + 1);
	item->setData(data["type"].toInt(), Qt::UserRole + 2);
	switch (data["sender"].toInt())
	{
	case static_cast<int>(QQConfigs::MessageConfig::SenderType::me):
	case static_cast<int>(QQConfigs::MessageConfig::SenderType::she):
	{
		// todo-从本地缓存中根据ID号获取用户信息
		// QPixmap icon = QPixmap::fromImage(QQFunctions::getBase64ToImage(data["icon"].toString()));
		// icon = QQFunctions::getRoundedPixmap(icon.scaled(QQGlobals::g_theme->m_chat_message_icon_size, Qt::KeepAspectRatio), QQGlobals::g_theme->m_chat_message_icon_size.width() / 2);
		// item->setData(icon, Qt::UserRole + 1);

		// if (data["sender"].toInt() == 1)
		// {
		// 	QString nickname;
		// 	if (!nickname.isEmpty())
		// 	{
		// 		item->setData(nickname, Qt::UserRole + 2);
		// 	}
		// 	else
		// 	{
		// 		// item->setData(QString::fromLocal8Bit(QQFunctions::getUserInfo(QString::fromStdString(message->m_senderID))->m_name.c_str()), Qt::UserRole + 2);
		// 	}
		// }
		switch (data["type"].toInt())
		{
		case static_cast<int>(QQConfigs::MessageConfig::MessageType::text):
		{
			QString text = data["content"].toString();
			text = QQFunctions::getCalculateText(text, QQGlobals::g_theme->m_chat_message_text_font, ui->messageList->width() - 100);
			item->setData(text, Qt::UserRole + 3);
			break;
		}
		case static_cast<int>(QQConfigs::MessageConfig::MessageType::image):
		{
			QPixmap image = QPixmap::fromImage(QQFunctions::getBase64ToImage(data["content"].toString()));
			image = QQFunctions::getRoundedPixmap(image.scaled(QQGlobals::g_theme->m_chat_message_image_frame_max_size, Qt::KeepAspectRatio), 10);
			item->setData(image, Qt::UserRole + 4);
			break;
		}
		case static_cast<int>(QQConfigs::MessageConfig::MessageType::file):
		{
			QString fileName = data["content"].toString();
			QPair<qreal, QString> pair = QQFunctions::getFileFormatSize(QFile(fileName).size());
			QString fileSize = QString::number(pair.first, 'f', 2) + " " + pair.second;
			item->setData(fileSize, Qt::UserRole + 3);
			QPixmap fileIcon = QFileIconProvider().icon(QFileInfo(fileName)).pixmap(QQGlobals::g_theme->m_chat_message_file_icon_size);
			item->setData(fileIcon, Qt::UserRole + 4);
			if (data["sender"].toInt() == static_cast<int>(QQConfigs::MessageConfig::SenderType::me))
			{
				item->setData(100, Qt::UserRole + 7);
			}
			break;
		}
		case static_cast<int>(QQConfigs::MessageConfig::MessageType::link):
		{
			QString link = data["content"].toString();
			link = QQFunctions::getCalculateText(link, QQGlobals::g_theme->m_chat_message_link_font, ui->messageList->width() - 100);
			item->setData(link, Qt::UserRole + 3);
			break;
		}
		case static_cast<int>(QQConfigs::MessageConfig::MessageType::emoji):
		{
			break;
		}
		default:
			break;
		}
		break;
	}
	case static_cast<int>(QQConfigs::MessageConfig::SenderType::system):
	{
		QString system = data["content"].toString();
		system = QQFunctions::getCalculateText(system, QQGlobals::g_theme->m_chat_message_system_font, ui->messageList->width() - 100, true);
		item->setData(system, Qt::UserRole + 3);
		break;
	}
	case static_cast<int>(QQConfigs::MessageConfig::SenderType::time):
	{
		QString time = data["content"].toString();
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
	item->setData(data["sending"].toBool(), Qt::UserRole + 5);
	item->setData(data["sended"].toBool(), Qt::UserRole + 6);
}

void QQChat::sendFileByName(const QString &fileName)
{
	// 更新界面
	QModelIndex index = ui->indexList->currentIndex();
	QStandardItem *item = new QStandardItem;
	QVariantMap map;
	QVariantList list;
	QString receiver = index.data(Qt::UserRole).toString();
	QDateTime time = QDateTime::currentDateTime();
	list << QString::fromStdString(QQGlobals::g_user->m_ID)
		 << receiver
		 << time.toSecsSinceEpoch();
	map.insert("ID", QQFunctions::getUniqueHashID(list));
	map.insert("sender", static_cast<int>(QQConfigs::MessageConfig::SenderType::me));
	map.insert("type", static_cast<int>(QQConfigs::MessageConfig::MessageType::file));
	map.insert("content", fileName);
	map.insert("sending", true);
	map.insert("sended", false);
	loadMessageInModelItem(item, map);
	m_messageListModel->appendRow(item);

	// 请求单独的文件模块进行处理与发送
	QJsonObject sendData;
	sendData.insert("version", "1.0");
	sendData.insert("sender", "chat");
	sendData.insert("reciver", "file");
	sendData.insert("action", "sendfile");
	QJsonObject data;
	data.insert("senderID", QString::fromStdString(QQGlobals::g_user->m_ID));
	data.insert("receiverID", receiver);
	data.insert("fileName", fileName);
	sendData.insert("data", data);
	QQ_SEND_EVENT_GLOBAL(QQEnums::requestmoudel, QJsonDocument(sendData).toJson());
}

void QQChat::handleNetwork(const QByteArray &data)
{
	QJsonObject receiveData = QJsonDocument::fromJson(data).object();
	if (receiveData["action"].toString() == QString("receivemessage"))
	{
		QJsonObject realData = receiveData["data"].toObject();
		QStandardItem *item = new QStandardItem;
		QVariantMap map;
		map.insert("ID", realData["ID"].toString());
		map.insert("sender", realData["sender"].toInt());
		map.insert("type", realData["type"].toInt());
		map.insert("content", realData["content"].toString());
		map.insert("time", realData["time"].toString());
		map.insert("sending", false);
		map.insert("sended", true);
		loadMessageInModelItem(item, map);
		m_messageListModel->appendRow(item);
		// 更新本地存储
		QJsonObject sendData;
		sendData.insert("version", *(QQGlobals::g_version));
		sendData.insert("sender", "chat");
		sendData.insert("receiver", "storage");
		sendData.insert("action", "savemessage");
		sendData.insert("data", realData);
		QQ_SEND_EVENT_GLOBAL(QQEnums::savestorage, QJsonDocument(sendData).toJson());
	}
	else if (receiveData["action"].toString() == QString("updatemessage"))
	{
		QJsonObject realData = receiveData["data"].toObject();
		// 检查界面是否需要更新
		QString sender = realData["sender"].toString();
		QString receiver = realData["receiver"].toString();
		QString time = realData["time"].toString();
		QString currentID = ui->indexList->currentIndex().data(Qt::UserRole).toString();
		if (sender == currentID)
		{
			// 查询消息的位置
			QString ID = realData["ID"].toString();
			for (int i = 0; i < m_messageListModel->rowCount(); ++i)
			{
				if (m_messageListModel->item(i)->data(Qt::UserRole).toString() == ID)
				{
					QStandardItem *item = m_messageListModel->item(i);
					item->setData(realData["sending"].toBool(), Qt::UserRole + 5);
					item->setData(realData["sended"].toBool(), Qt::UserRole + 6);
					if (realData["delete"].toBool())
					{
						QVariantMap map;
						QVariantList list;
						list << sender << receiver << time;
						map.insert("ID", QQFunctions::getUniqueHashID(list));
						map.insert("sender", static_cast<int>(QQConfigs::MessageConfig::SenderType::system));
						map.insert("type", static_cast<int>(QQConfigs::MessageConfig::MessageType::text));
						map.insert("content", "对方撤回了一条消息");
						map.insert("time", time);
						map.insert("sending", false);
						map.insert("sended", true);
						loadMessageInModelItem(item, map);
					}
					break;
				}
			}
		}

		QJsonObject sendData;
		sendData.insert("version", *(QQGlobals::g_version));
		sendData.insert("sender", "chat");
		sendData.insert("receiver", "storage");
		sendData.insert("action", "updatemessage");
		sendData.insert("data", realData);
		QQ_SEND_EVENT_GLOBAL(QQEnums::savestorage, QJsonDocument(sendData).toJson());
	}
}

void QQChat::handleStorage(const QByteArray &data)
{
	QJsonObject receiveData = QJsonDocument::fromJson(data).object();
	if (receiveData["action"].toString() == QString("receiveindex"))
	{
		QJsonObject realData = receiveData["data"].toObject();
		QJsonArray indexList = realData["indexs"].toArray();
		m_indexListModel->insertRows(0, indexList.size());
		for (int i = 0; i < indexList.size(); ++i)
		{
			QJsonObject index = indexList.at(i).toObject();
			QStandardItem *item = new QStandardItem;
			if (index["type"].toString() == QString("user"))
			{
				QVariantMap map;
				map.insert("ID", index["ID"].toString());
				map.insert("name", index["name"].toString());
				map.insert("nickname", index["nickname"].toString());
				map.insert("icon", index["icon"].toString());
				map.insert("message", index["message"].toString());
				map.insert("time", index["time"].toString());
				map.insert("count", index["count"].toInt());
				loadPeopleInModelItem(item, map);
			}
			else if (index["type"].toString() == QString("group"))
			{
				QVariantMap map;
				map.insert("ID", index["ID"].toString());
				map.insert("name", index["name"].toString());
				map.insert("nickname", index["nickname"].toString());
				map.insert("icon", index["icon"].toString());
				map.insert("message", index["message"].toString());
				map.insert("time", index["time"].toString());
				map.insert("count", index["count"].toInt());
				loadPeopleInModelItem(item, map);
			}
			m_indexListModel->setItem(i, item);
		}
	}
	else if (receiveData["action"].toString() == QString("receivemessage"))
	{
		QJsonObject realData = receiveData["data"].toObject();
		QJsonArray messageList = realData["messages"].toArray();
		m_messageListModel->insertRows(0, messageList.size());
		for (int i = 0; i < messageList.size(); ++i)
		{
			QJsonObject message = messageList.at(i).toObject();
			QStandardItem *item = new QStandardItem;
			QVariantMap map;
			map.insert("ID", message["ID"].toString());
			map.insert("sender", message["sender"].toInt());
			map.insert("type", message["type"].toInt());
			map.insert("content", message["content"].toString());
			map.insert("sending", message["sending"].toBool());
			map.insert("sended", message["sended"].toBool());
			loadMessageInModelItem(item, map);
			m_messageListModel->setItem(i, item);
		}
	}
}

void QQChat::handleEmoji(const QByteArray &data)
{
	QJsonObject receiveData = QJsonDocument::fromJson(data).object();
	if (receiveData["action"].toString() == QString("sendemoji"))
	{
		QStandardItem *item = new QStandardItem;
		QVariantMap map;
		QVariantList list;
		QString receiver = ui->indexList->currentIndex().data(Qt::UserRole).toString();
		QDateTime time = QDateTime::currentDateTime();
		list << QString::fromStdString(QQGlobals::g_user->m_ID)
			 << receiver
			 << time.toSecsSinceEpoch();
		map.insert("ID", QQFunctions::getUniqueHashID(list));
		map.insert("sender", static_cast<int>(QQConfigs::MessageConfig::SenderType::me));
		map.insert("type", static_cast<int>(QQConfigs::MessageConfig::MessageType::emoji));
		map.insert("content", receiveData["data"].toObject()["content"].toString());
		map.insert("sending", true);
		map.insert("sended", false);
		loadMessageInModelItem(item, map);
		m_messageListModel->appendRow(item);
		map.insert("sender", QString::fromStdString(QQGlobals::g_user->m_ID));
		map.insert("receiver", receiver);
		map.insert("time", time.toSecsSinceEpoch());
		sendMessageToNetWork(map);
	}
}

void QQChat::handleFriend(const QByteArray &data)
{
	QJsonObject receiveData = QJsonDocument::fromJson(data).object();
	if (receiveData["action"].toString() == QString("addchat"))
	{
		QStandardItem *item = new QStandardItem;
		QVariantMap map;
		map.insert("ID", receiveData["data"].toObject()["ID"].toString());
		map.insert("name", receiveData["data"].toObject()["name"].toString());
		map.insert("nickname", receiveData["data"].toObject()["nickname"].toString());
		map.insert("icon", receiveData["data"].toObject()["icon"].toString());
		map.insert("message", receiveData["data"].toObject()["message"].toString());
		map.insert("time", receiveData["data"].toObject()["time"].toString());
		map.insert("count", receiveData["data"].toObject()["count"].toInt());
		loadPeopleInModelItem(item, map);
		m_indexListModel->insertRow(0, item);
	}
	else if (receiveData["action"].toString() == QString("deletefriend"))
	{
		QString ID = receiveData["data"].toObject()["ID"].toString();
		for (int i = 0; i < m_indexListModel->rowCount(); ++i)
		{
			if (m_indexListModel->item(i)->data(Qt::UserRole).toString() == ID)
			{
				m_indexListModel->removeRow(i);
				break;
			}
		}
	}
}

void QQChat::handleAudio(const QByteArray &data)
{
	QJsonObject receiveData = QJsonDocument::fromJson(data).object();
	if (receiveData["action"].toString() == QString("sendaudio"))
	{
		QStandardItem *item = new QStandardItem;
		QVariantMap map;
		QVariantList list;
		QString sender = receiveData["data"].toObject()["sender"].toString();
		QString receiver = receiveData["data"].toObject()["receiver"].toString();
		qint64 time = receiveData["data"].toObject()["time"].toInt();
		list << sender << receiver << time;
		map.insert("ID", QQFunctions::getUniqueHashID(list));
		map.insert("sender", static_cast<int>(QQConfigs::MessageConfig::SenderType::me));
		map.insert("type", static_cast<int>(QQConfigs::MessageConfig::MessageType::audio));
		map.insert("content", receiveData["data"].toObject()["content"].toString());
		map.insert("sending", true);
		map.insert("sended", false);
		loadMessageInModelItem(item, map);
		m_messageListModel->appendRow(item);
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
		if (text == QString::fromLocal8Bit("消息免打扰"))
		{
			action->setText(QString::fromLocal8Bit("取消免打扰"));
		}
		else if (text == QString::fromLocal8Bit("取消免打扰"))
		{
			action->setText(QString::fromLocal8Bit("消息免打扰"));
		}
	}
	else if (action == m_chatFriendContextMenu->actions()[2])
	{
		m_haveInChatList.remove(index.data(Qt::UserRole).toString());
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
			QString report = QQWidgets::MessageDialog::getInstance()->findChild<QTextEdit *>()->toPlainText();
			// 发送举报消息
			QJsonObject sendData;
			sendData.insert("version", "1.0");
			sendData.insert("sender", "chat");
			sendData.insert("receiver", "network");
			sendData.insert("action", "sendreport");
			QJsonObject data;
			data.insert("ID", index.data(Qt::UserRole).toString());
			data.insert("report", report);
			sendData.insert("data", data);
			QQ_SEND_EVENT_GLOBAL(QQEnums::sendmessage, QJsonDocument(sendData).toJson());
			//  提示发送成功
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
	m_indexListModel->itemFromIndex(index)->setData(0, Qt::UserRole + 5);
	QString name = m_indexListModel->itemFromIndex(index)->data(Qt::UserRole + 2).toString();
	ui->nicknameLab->setText(name);
	// 尾部空白--主要是美观
	QStandardItem *item = new QStandardItem;
	int width = ui->messageList->width();
	item->setSizeHint(QSize(width, 25));
	m_messageListModel->appendRow(item);
	m_haveInChatList.insert(m_indexListModel->itemFromIndex(index)->data(Qt::UserRole).toString(), m_indexListModel->itemFromIndex(index));
	m_activeChatIndex = index;

	m_messageListModel->clear();
	QJsonObject sendData;
	sendData.insert("version", *(QQGlobals::g_version));
	sendData.insert("sender", "chat");
	sendData.insert("receiver", "storage");
	sendData.insert("action", "loadsmessage");
	QJsonObject data;
	data.insert("ID", m_indexListModel->itemFromIndex(index)->data(Qt::UserRole).toString());
	sendData.insert("data", data);
	QQ_SEND_EVENT_GLOBAL(QQEnums::loadstorage, QJsonDocument(sendData).toJson());
}

void QQChat::do_userClickSendEmoji()
{
	QPoint pos = ui->emojiBtn->mapToGlobal(QPoint(0, 0));
	pos.setY(pos.y() - QQEmoji::getInstance()->height() - 10);
	QQEmoji::getInstance()->move(pos);
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

	QStandardItem *item = new QStandardItem;
	QVariantMap map;
	QVariantList list;
	QString receiver = ui->indexList->currentIndex().data(Qt::UserRole).toString();
	QDateTime time = QDateTime::currentDateTime();
	list << QString::fromStdString(QQGlobals::g_user->m_ID)
		 << receiver
		 << time.toSecsSinceEpoch();
	map.insert("ID", QQFunctions::getUniqueHashID(list));
	map.insert("sender", static_cast<int>(QQConfigs::MessageConfig::SenderType::me));
	map.insert("type", static_cast<int>(QQConfigs::MessageConfig::MessageType::image));
	map.insert("content", QQFunctions::getImageToBase64(fileName).first);
	map.insert("sending", true);
	map.insert("sended", false);
	loadMessageInModelItem(item, map);
	m_messageListModel->appendRow(item);
	map.insert("sender", QString::fromStdString(QQGlobals::g_user->m_ID));
	map.insert("receiver", receiver);
	map.insert("time", time.toSecsSinceEpoch());
	sendMessageToNetWork(map);
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
	sendData.insert("version", *(QQGlobals::g_version));
	sendData.insert("sender", "chat");
	sendData.insert("reciver", "audio");
	sendData.insert("action", "sendaudio");
	QJsonObject data;
	data.insert("sender", QString::fromStdString(QQGlobals::g_user->m_ID));
	QModelIndex index = ui->indexList->currentIndex();
	data.insert("receiver", index.data(Qt::UserRole).toString());
	sendData.insert("data", data);
	QQ_SEND_EVENT_GLOBAL(QQEnums::requestmoudel, QJsonDocument(sendData).toJson());
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
	QStandardItem *item = new QStandardItem;
	QVariantMap map;
	QVariantList list;
	QString receiver = index.data(Qt::UserRole).toString();
	QDateTime time = QDateTime::currentDateTime();
	list << QString::fromStdString(QQGlobals::g_user->m_ID)
		 << receiver
		 << time.toSecsSinceEpoch();
	map.insert("ID", index.data(Qt::UserRole).toString());
	map.insert("sender", static_cast<int>(QQConfigs::MessageConfig::SenderType::me));
	map.insert("type", static_cast<int>(QQConfigs::MessageConfig::MessageType::text));
	map.insert("content", ui->textInput->toPlainText());
	map.insert("sending", true);
	map.insert("sended", false);
	loadMessageInModelItem(item, map);
	m_messageListModel->appendRow(item);
	map.insert("sender", QString::fromStdString(QQGlobals::g_user->m_ID));
	map.insert("receiver", receiver);
	map.insert("time", time.toSecsSinceEpoch());
	sendMessageToNetWork(map);
	ui->textInput->clear();
}