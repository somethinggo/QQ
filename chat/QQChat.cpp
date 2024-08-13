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
	else if (element == PE_FrameGroupBox)
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
		// 边框
		if (name == "clearInputBtn" ||
			name == "sendMessageBtn" ||
			name == "main_friendAndGroupTab_informationStack_NotNULL_friendShareOtherBtn" ||
			name == "main_friendAndGroupTab_informationStack_NotNULL_friendMusicConnectBtn" ||
			name == "main_friendAndGroupTab_informationStack_NotNULL_friendSendMsgBtn")
		{
			painter->save();
			painter->setRenderHint(QPainter::Antialiasing);
			painter->setPen(QColor("#CCCCCC"));
			QRect borderRect = option->rect;
			borderRect.adjust(1, 1, -1, -1);
			painter->drawRoundedRect(borderRect, 5, 5);
			painter->restore();
		}
		// 鼠标移入加深颜色，鼠标按下的颜色加深
		if (name == "clearInputBtn" ||
			name == "sendMessageBtn" ||
			name == "main_friendAndGroupTab_informationStack_NotNULL_friendShareOtherBtn" ||
			name == "main_friendAndGroupTab_informationStack_NotNULL_friendMusicConnectBtn" ||
			name == "main_friendAndGroupTab_informationStack_NotNULL_friendSendMsgBtn" ||
			name == "main_friendAndGroupTab_friendNoticeButton" ||
			name == "main_friendAndGroupTab_groupNoticeButton")
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
			name == "goToAudioTabBtn" ||
			name == "main_friendAndGroupTab_informationStack_NotNULL_noNoticeWidget_selectBtn" ||
			name == "main_friendAndGroupTab_informationStack_NotNULL_noNoticeWidget_clearBtn")
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
	else if (element == CE_PushButtonLabel)
	{
		if (name == "main_friendAndGroupTab_friendNoticeButton" ||
			name == "main_friendAndGroupTab_groupNoticeButton")
		{
			QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
			drawItemText(painter, buttonOption->rect.adjusted(20, 0, 0, 0), Qt::AlignVCenter, buttonOption->palette, buttonOption->state & State_Enabled, buttonOption->text, QPalette::ButtonText);
			if (!buttonOption->icon.isNull())
			{
				QSize iconSize = buttonOption->iconSize.isValid() ? buttonOption->iconSize : QSize(16, 16);
				QRect iconRect(buttonOption->rect.right() - 20 - iconSize.width(), buttonOption->rect.center().y() - iconSize.height() / 2, iconSize.width(), iconSize.height());
				buttonOption->icon.paint(painter, iconRect);
			}
			return;
		}
	}
	return QProxyStyle::drawControl(element, option, painter, widget);
}

QRect QQChatProxyStyle::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
{
	QString name = widget->objectName();
	if (element == SE_ShapedFrameContents)
	{
		if (name == "main_chatTab_chatStack_NotNULL_friendNikeNameLab")
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

	m_chatListModel = new QStandardItemModel(this);
	m_chatListDelegate = new QQChatIndexDelegate(this);
	m_chatListDelegate->setItemSize(QSize(350, 90));
	m_chatListDelegate->setMAXStringWidth(300);
	m_messageModel = new QStandardItemModel(this);
	m_chatMessageDelegate = new QQChatMessageDelegate(this);
	m_chatFriendContextMenu = new ElaMenu(this);
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::ArrowUp), QString::fromLocal8Bit("置顶"));
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::Ban), QString::fromLocal8Bit("消息免打扰"));
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::TrashCan), QString::fromLocal8Bit("删除该聊天"));
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::BrakeWarning), QString::fromLocal8Bit("举报"));

	ui->searchInput->setStyle(m_searchStyle);
	ui->searchInput->addAction(m_searchAction, QLineEdit::LeadingPosition);
	ui->searchMoreBtn->setStyle(m_searchStyle);
	ui->searchMoreBtn->setMask(QQFunctions::getRoundedMask(ui->searchMoreBtn->size(), 5));
	ui->indexList->setModel(m_chatListModel);
	ui->indexList->setItemDelegate(m_chatListDelegate);
	ui->nicknameLab->setStyle(m_commonStyle);
	ui->messageList->setModel(m_messageModel);
	ui->messageList->setItemDelegate(m_chatMessageDelegate);
	ui->messageList->setStyle(m_commonStyle);
	ui->normalInputOption->setStyle(m_commonStyle);
	ui->emojiBtn->setStyle(m_commonStyle);
	ui->fileBtn->setStyle(m_commonStyle);
	ui->pictureBtn->setStyle(m_commonStyle);
	ui->upperBtn->setStyle(m_commonStyle);
	ui->histroyBtn->setStyle(m_commonStyle);
	ui->textInput->setStyle(m_commonStyle);
	ui->otherInputOption->setStyle(m_commonStyle);
	ui->goToAudioTabBtn->setStyle(m_commonStyle);
	ui->clearInputBtn->setMask(QQFunctions::getRoundedMask(ui->clearInputBtn->size(), 10));
	ui->clearInputBtn->setStyle(m_commonStyle);
	ui->sendMessageBtn->setMask(QQFunctions::getRoundedMask(ui->sendMessageBtn->size(), 10));
	ui->sendMessageBtn->setStyle(m_commonStyle);

	qApp->installEventFilter(this);
	connectInit();
}

QQChat::~QQChat()
{
	delete m_commonStyle;
	delete m_searchStyle;
	delete ui;
}

void QQChat::connectInit()
{
	connect(m_searchMoreMenu, &QMenu::triggered, this, &QQChat::do_userClickSearchMoreButtonAction);		// 搜索更多按钮选项
	connect(ui->searchMoreBtn, &QPushButton::clicked, this, &QQChat::do_userClickSearchMoreButton);			// chat界面 搜索更多按钮
	connect(ui->indexList, &QListView::clicked, this, &QQChat::do_userClickChatIndex);						// chat界面 点击聊天索引列表索引
	connect(m_chatFriendContextMenu, &QMenu::triggered, this, &QQChat::do_userClickRightContextMenuAction); // chat界面 聊天索引列表右键菜单
	connect(ui->emojiBtn, &QPushButton::clicked, this, &QQChat::do_userClickSendEmojiButton);				// chat界面 点击发送表情按钮
	connect(ui->fileBtn, &QPushButton::clicked, this, &QQChat::do_userClickSendFileButton);					// chat界面 点击发送文件按钮
	connect(ui->pictureBtn, &QPushButton::clicked, this, &QQChat::do_userClickSendPictureButton);			// chat界面 点击发送图片按钮
	connect(ui->upperBtn, &QPushButton::toggled, this, &QQChat::do_userOpenBigWriteButton);					// chat界面 点击打开大写按钮
	connect(ui->histroyBtn, &QPushButton::clicked, this, &QQChat::do_userClickLookHistroyButton);			// chat界面 点击查看历史消息按钮
	connect(ui->textInput, &QTextEdit::textChanged, this, &QQChat::do_limitUserInputTextCount);				// chat界面 限制用户输入文本长度
	connect(ui->goToAudioTabBtn, &QPushButton::clicked, this, &QQChat::do_userClickSendAudioButton);		// chat界面 点击发送语音按钮
	connect(ui->clearInputBtn, &QPushButton::clicked, this, &QQChat::do_userClickClearInputButton);			// chat界面 点击清空输入按钮
	connect(ui->sendMessageBtn, &QPushButton::clicked, this, &QQChat::do_userClickSendMessageButton);		// chat界面 点击发送消息按钮
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
			if (ui->inputStackWidget->currentIndex() == 0)
			{
				if (QQFunctions::getMouseIsInWidget(ui->textInput, mouseEvent->globalPos()))
				{
					ui->textInput->setFocus();
				}
				else
				{
					ui->textInput->clearFocus();
				}
				if (!QQFunctions::getMouseIsInWidget(QQEmoji::getInstance(), mouseEvent->globalPos()))
				{
					QQEmoji::getInstance()->hide();
				}
			}
		}
	}
	return QWidget::eventFilter(watch, event);
}

void QQChat::do_sendmsg(QStandardItem *item, QSharedPointer<QQConfigs::MessageBaseConfig> &message)
{
	// 更新消息模型的状态信息
	item->setData(true, Qt::UserRole + 5);
	// 写入消息数据
	QJsonObject senderData;
	senderData.insert("version", "1.0");
	senderData.insert("sender", "QQChat");
	senderData.insert("action", "send-message");
	QJsonObject data;
	switch (message->m_messageType)
	{
	case QQConfigs::MessageBaseConfig::MessageType::text:
	{
		data.insert("type", "text");
		break;
	}
	case QQConfigs::MessageBaseConfig::MessageType::image:
	{
		data.insert("type", "image");
		break;
	}
	case QQConfigs::MessageBaseConfig::MessageType::file:
	{
		data.insert("type", "file");
		break;
	}
	case QQConfigs::MessageBaseConfig::MessageType::link:
	{
		data.insert("type", "link");
		break;
	}
	}
	data.insert("sender", QString::fromStdString(message->m_senderID));
	data.insert("receiver", QString::fromStdString(message->m_receiverID));
	data.insert("content", QString::fromLocal8Bit(message->m_content.c_str()));
	data.insert("time", message->m_time);
	senderData.insert("data", data);
}

void QQChat::do_revmsg()
{
}

void QQChat::loadPeopleInModelItem(QStandardItem *item, QQConfigs::UserFriend_C *user, ClientEnums::PeopleWidgetType type)
{
	item->setData(QVariant::fromValue(user), Qt::UserRole);
	item->setData(QVariant::fromValue(type), Qt::UserRole + 1);
	QString iconBase64 = QString::fromStdString(user->m_icon);
	QPixmap icon = QPixmap::fromImage(getBase64ToImage(iconBase64));
	item->setData(icon, Qt::UserRole + 2);
	QString name = QString::fromLocal8Bit(user->m_name.c_str());
	switch (type)
	{
	case ClientEnums::PeopleWidgetType::friendchats:
	{
		name = user->m_nikeName;
		break;
	}
	case ClientEnums::PeopleWidgetType::friends:
	{
		name = name + QString("(%1)").arg(user->m_nikeName);
		break;
	}
	default:
		break;
	}
	item->setData(name, Qt::UserRole + 3);
	switch (type)
	{
	case ClientEnums::PeopleWidgetType::friendchats:
	{
		item->setData(user->m_lastMsg, Qt::UserRole + 4);
		QString time = user->m_lastMsgTime.toString();
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
		item->setData(user->m_newMsgCount, Qt::UserRole + 6);
		QIcon stateIcon;
		stateIcon.addFile(":/widget/common/images/widget/common/distrup_no.png", QSize(), QIcon::Normal, QIcon::Off);
		stateIcon.addFile(":/widget/common/images/widget/common/distrup_yes.png", QSize(), QIcon::Normal, QIcon::On);
		item->setData(stateIcon, Qt::UserRole + 7);
		break;
	}
	case ClientEnums::PeopleWidgetType::friends:
	{
		QString sign = QString::fromLocal8Bit(user->m_sign.c_str());
		item->setData(sign, Qt::UserRole + 4);
		break;
	}
	case ClientEnums::PeopleWidgetType::friendappend:
	{
		QString account = QString::fromStdString(user->m_account.c_str());
		item->setData(account, Qt::UserRole + 4);
		break;
	}
	default:
		break;
	}
}

void QQChat::loadPeopleInModelItem(QStandardItem *item, QQConfigs::UserGroup_C *group, ClientEnums::PeopleWidgetType type)
{
	item->setData(QVariant::fromValue(group), Qt::UserRole);
	item->setData(QVariant::fromValue(type), Qt::UserRole + 1);
	QString iconBase64 = QString::fromStdString(group->m_icon);
	QPixmap icon = QPixmap::fromImage(getBase64ToImage(iconBase64));
	item->setData(icon, Qt::UserRole + 2);
	QString name = QString::fromLocal8Bit(group->m_name.c_str());
	switch (type)
	{
	case ClientEnums::PeopleWidgetType::groupchats:
	{
		name = group->m_nikeName;
		break;
	}
	case ClientEnums::PeopleWidgetType::groups:
	{
		name = name + QString("(%1)").arg(group->m_account.c_str());
		break;
	}
	default:
		break;
	}
	item->setData(name, Qt::UserRole + 3);
	switch (type)
	{
	case ClientEnums::PeopleWidgetType::groupchats:
	{
		item->setData(group->m_lastMsg, Qt::UserRole + 4);
		QString time = group->m_lastMsgTime.toString();
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
		item->setData(group->m_newMsgCount, Qt::UserRole + 6);
		QIcon stateIcon;
		stateIcon.addFile(":/widget/common/images/widget/common/distrup_no.png", QSize(), QIcon::Normal, QIcon::On);
		stateIcon.addFile(":/widget/common/images/widget/common/distrup_yes.png", QSize(), QIcon::Normal, QIcon::Off);
		item->setData(stateIcon, Qt::UserRole + 7);
		break;
	}
	case ClientEnums::PeopleWidgetType::groupappend:
	{
		QString sign = QString::fromLocal8Bit(group->m_account.c_str());
		item->setData(sign, Qt::UserRole + 4);
		break;
	}
	default:
		break;
	}
}

void QQChat::loadMessageInModelItem(QStandardItem *item, QSharedPointer<QQConfigs::MessageBaseConfig> &message, bool isReceive)
{
	item->setData(QVariant::fromValue(message.data()), Qt::UserRole);
	switch (message->m_senderType)
	{
	case QQConfigs::MessageBaseConfig::SenderType::me:
	case QQConfigs::MessageBaseConfig::SenderType::she:
	{
		QString iconBase64 = QString::fromStdString(GlobalValuesAgent::m_user.m_icon);
		QPixmap icon = QPixmap::fromImage(getBase64ToImage(iconBase64));
		item->setData(icon, Qt::UserRole + 1);
		QString name = QString::fromLocal8Bit(GlobalValuesAgent::m_user.m_name.c_str());
		if (message->m_senderType == QQConfigs::MessageBaseConfig::SenderType::she)
		{
			name = getUserInfo(QString::fromStdString(message->m_senderID))->m_nikeName;
		}
		item->setData(name, Qt::UserRole + 2);
		switch (message->m_messageType)
		{
		case QQConfigs::MessageBaseConfig::MessageType::text:
		{
			item->setData(QString::fromLocal8Bit(message->m_content.c_str()), Qt::UserRole + 3);
			break;
		}
		case QQConfigs::MessageBaseConfig::MessageType::image:
		{
			QString imageBase64 = QString::fromStdString(message->m_content);
			QPixmap image = QPixmap::fromImage(getBase64ToImage(imageBase64));
			item->setData(image, Qt::UserRole + 4);
			break;
		}
		case QQConfigs::MessageBaseConfig::MessageType::file:
		{
			QString fileName = QString::fromLocal8Bit(message->m_content.c_str());
			QPair<qreal, QString> pair = getFileFormatSize(QFile(fileName).size());
			QString fileSize = QString::number(pair.first, 'f', 2) + " " + pair.second;
			item->setData(fileSize, Qt::UserRole + 3);
			QPixmap fileIcon = QFileIconProvider().icon(QFileInfo(fileName)).pixmap(50, 50);
			item->setData(fileIcon, Qt::UserRole + 4);
			break;
		}
		case QQConfigs::MessageBaseConfig::MessageType::link:
		{
			QString link = QString::fromLocal8Bit(message->m_content.c_str());
			item->setData(link, Qt::UserRole + 3);
			break;
		}
		case QQConfigs::MessageBaseConfig::MessageType::emoji:
		{
			// 内部自带表情类型--数据除了unicode,都转为了base64
			QString emoji = QString::fromStdString(message->m_content.c_str());
			item->setData(emoji, Qt::UserRole + 3);
			break;
		}
		default:
			break;
		}
		break;
	}
	case QQConfigs::MessageBaseConfig::SenderType::system:
	{
		QString system = QString::fromLocal8Bit(message->m_content.c_str());
		item->setData(system, Qt::UserRole + 3);
		break;
	}
	case QQConfigs::MessageBaseConfig::SenderType::time:
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
	switch (message->m_senderType)
	{
	case QQConfigs::MessageBaseConfig::SenderType::me:
	{
		item->setData(false, Qt::UserRole + 5);
		if (message->m_messageType == QQConfigs::MessageBaseConfig::MessageType::file)
		{
			item->setData(true, Qt::UserRole + 6);
		}
		break;
	}
	case QQConfigs::MessageBaseConfig::SenderType::she:
	{
		item->setData(true, Qt::UserRole + 5);
		if (message->m_messageType == QQConfigs::MessageBaseConfig::MessageType::file)
		{
			if (isReceive)
			{
				item->setData(false, Qt::UserRole + 6);
			}
			else
			{
				item->setData(true, Qt::UserRole + 6);
			}
		}
		break;
	}
	case QQConfigs::MessageBaseConfig::SenderType::system:
	case QQConfigs::MessageBaseConfig::SenderType::time:
	{
		item->setData(true, Qt::UserRole + 5);
		break;
	}
	}
}

void QQChat::sendFileByName(QString &fileName)
{
	// 内部自带文件检验机制
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
	if (file.size() / 1024 / 1024 > GlobalValuesAgent::MAX_FILE_SIZE)
	{
		ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::BottomLeft, "", QString::fromLocal8Bit("文件过大,请发送小于512MB的文件!!!"), 3000, this);
		return;
	}
	QModelIndex index = ui->indexList->currentIndex();
	QQConfigs::UserFriend_C *user = const_cast<QQConfigs::UserFriend_C *>(qvariant_cast<QQConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
	QQConfigs::UserGroup_C *group = const_cast<QQConfigs::UserGroup_C *>(qvariant_cast<QQConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
	QSharedPointer<QQConfigs::MessageBaseConfig> message(new QQConfigs::MessageBaseConfig);
	message->m_messageType = QQConfigs::MessageBaseConfig::MessageType::file;
	message->m_senderType = QQConfigs::MessageBaseConfig::SenderType::me;
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
}

void QQChat::appendMessage(QSharedPointer<QQConfigs::MessageBaseConfig> &message, bool isUpdateData, int index, bool isReceive)
{
	QStandardItem *item = new QStandardItem;
	QListView *listView = ui->messageList;
	do_messageInModelItem(item, message, isReceive);
	if (isUpdateData)
	{
		m_messageListModel->insertRow(m_messageListModel->rowCount() - 1, item);
		listView->scrollToBottom();
		QModelIndex index = ui->indexList->currentIndex();
		QQConfigs::UserFriend_C *user = const_cast<QQConfigs::UserFriend_C *>(qvariant_cast<QQConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
		QQConfigs::UserGroup_C *group = const_cast<QQConfigs::UserGroup_C *>(qvariant_cast<QQConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
		if (user != nullptr)
		{
			user->m_msgList.append(message);
		}
		else if (group != nullptr)
		{
			group->m_msgList.append(message);
		}
		// 如果是自己发送的消息,则发送给服务器--其余消息均重文件中加载(群聊与好友消息来源于服务器,下载到本地,系统消息与时间消息是由本地生成,存储于本地,不需要发送)
		if (message->m_senderType == QQConfigs::MessageBaseConfig::SenderType::me)
		{
			do_sendmsg(item, message);
		}
	}
	else
	{
		m_messageListModel->setItem(index, item);
	}
}

void QQChat::loadMessages(QQConfigs::UserFriend_C *user)
{
	// 预分配空间
	m_messageListModel->insertRows(0, user->m_msgList.count());
	for (int i = 0; i < user->m_msgList.count(); ++i)
	{
		int index = user->m_msgList.count() - i - 1;
		QSharedPointer<QQConfigs::MessageBaseConfig> message = user->m_msgList.at(index);
		do_appendMessage(message, false, index);
	}
}

void QQChat::loadMessages(QQConfigs::UserGroup_C *group)
{
	// 预分配空间
	m_messageListModel->insertRows(0, group->m_msgList.count());
	for (int i = 0; i < group->m_msgList.count(); ++i)
	{
		int index = group->m_msgList.count() - i - 1;
		QSharedPointer<QQConfigs::MessageBaseConfig> message = group->m_msgList.at(index);
		do_appendMessage(message, false, index);
	}
}

void QQChat::do_userClickSearchMoreButton()
{
	QPoint pos = qobject_cast<QPushButton *>(sender())->mapToGlobal(QPoint(0, 0));
	QPoint searchMoreMenuPos = pos + QPoint(5, qobject_cast<QPushButton *>(sender())->height());
	m_searchMoreMenu->popup(searchMoreMenuPos);
}

void QQChat::do_userClickSearchMoreButtonAction(QAction *action)
{
	if (action == m_searchMoreMenu->actions()[0])
	{
	}
	else if (action == m_searchMoreMenu->actions()[1])
	{
		QQAppendFriendOrGroupDialog *appendFriendOrGroupDialog = QQAppendFriendOrGroupDialog::getInstance();
		QPoint pos = this->mapToGlobal(QPoint(0, 0)) + QPoint(this->width() / 2 - appendFriendOrGroupDialog->width() / 2, this->height() / 2 - appendFriendOrGroupDialog->height() / 2);
		appendFriendOrGroupDialog->setGeometry(QRect(pos, appendFriendOrGroupDialog->size()));
		appendFriendOrGroupDialog->exec();
	}
}

void QQChat::do_userClickRightContextMenuAction(QAction *action)
{
	QPoint pos = ui->indexList->getMCurrentPoint();
	QModelIndex index = ui->indexList->indexAt(pos);
	if (action == m_chatFriendContextMenu->actions()[0])
	{
		m_chatListModel->insertRow(0, m_chatListModel->takeRow(index.row())[0]);
	}
	else if (action == m_chatFriendContextMenu->actions()[1])
	{
		QString text = action->text();
		QQConfigs::UserFriend_C *user = const_cast<QQConfigs::UserFriend_C *>(qvariant_cast<QQConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
		QQConfigs::UserGroup_C *group = const_cast<QQConfigs::UserGroup_C *>(qvariant_cast<QQConfigs::UserGroup_C *>(index.data(Qt::UserRole)));

		if (text == QString::fromLocal8Bit("消息免打扰"))
		{
			action->setText(QString::fromLocal8Bit("取消免打扰"));
			if (user != nullptr)
			{
				user->m_state = QQConfigs::UserFriend_C::UserStateType::distrup;
			}
			else if (group != nullptr)
			{
				group->m_state = QQConfigs::UserGroup_C::GroupStateType::disnotice;
			}
		}
		else if (text == QString::fromLocal8Bit("取消免打扰"))
		{
			action->setText(QString::fromLocal8Bit("消息免打扰"));
			if (user != nullptr)
			{
				user->m_state = QQConfigs::UserFriend_C::UserStateType::online;
			}
			else if (group != nullptr)
			{
				group->m_state = QQConfigs::UserGroup_C::GroupStateType::normal;
			}
		}
	}
	else if (action == m_chatFriendContextMenu->actions()[2])
	{
		m_haveInChatList.remove(qvariant_cast<QQConfigs::UserFriend_C *>(index.data(Qt::UserRole))->m_ID);
		// 清除默认的移除选中项,选中项效果下移
		if (ui->indexList->selectionModel()->isSelected(index))
		{
			ui->indexList->selectionModel()->clear();
			ui->main_chatTab_chatStack->setCurrentIndex(0);
		};
		m_chatListModel->removeRow(index.row());
		// 如果没有聊天索引,则隐藏聊天界面
		if (m_chatListModel->rowCount() == 0)
		{
			ui->main_chatTab_chatStack->setCurrentIndex(0);
			ui->main_chatTab_listStack->setCurrentIndex(0);
		}
	}
	else if (action == m_chatFriendContextMenu->actions()[3])
	{
		QQMessageDialog *messageDialog = QQMessageDialog::getInstance();
		messageDialog->removeAllMWidget();
		QPoint pos = this->mapToGlobal(QPoint(0, 0)) + QPoint(this->width() / 2 - 200, this->height() / 2 - 100);
		messageDialog->setGeometry(QRect(pos, QSize(400, 100)));
		messageDialog->setMTittle(QString::fromLocal8Bit("举报聊天"));
		messageDialog->closeOkButton();
		QLabel *label = new QLabel(messageDialog);
		label->setText(QString::fromLocal8Bit("您确定要举报该聊天?"));
		label->setFixedHeight(40);
		label->setAlignment(Qt::AlignCenter);
		label->setFont(QFont("Microsoft YaHei", 11));
		messageDialog->addMWidget(label);
		QTextEdit *textEdit = new QTextEdit(messageDialog);
		textEdit->setFont(QFont("Microsoft YaHei", 11));
		textEdit->setPlaceholderText(QString::fromLocal8Bit("请说明举报理由"));
		textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		messageDialog->addMWidget(textEdit);
		messageDialog->layout()->setSpacing(10);
		messageDialog->layout()->setContentsMargins(20, 5, 20, 10);
		connect(textEdit, &QTextEdit::textChanged, [textEdit, messageDialog]()
				{
				if (textEdit->toPlainText().isEmpty())
				{
					messageDialog->closeOkButton();
				}
				else
				{
					messageDialog->openOkButton();
				} });
		std::function<void()> function = [this]
		{
			// 获取举报信息
			QModelIndex index = ui->indexList->currentIndex();
			QQConfigs::UserFriend_C *user = const_cast<QQConfigs::UserFriend_C *>(qvariant_cast<QQConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
			QQConfigs::UserGroup_C *group = const_cast<QQConfigs::UserGroup_C *>(qvariant_cast<QQConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
			QString report = QQMessageDialog::getInstance()->findChild<QTextEdit *>()->toPlainText();
			// 发送举报消息
			QJsonObject reportData;
			reportData.insert("version", "1.0");
			reportData.insert("sender", "QQChat");
			reportData.insert("action", "report");
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
			GlobalValuesAgent::m_localSocket.write(QJsonDocument(reportData).toJson());
			GlobalValuesAgent::m_localSocket.flush();
			// 提示发送成功
			ElaMessageBar::success(ElaMessageBarType::PositionPolicy::TopRight, "", QString::fromLocal8Bit("发送举报消息成功，感谢您的指正！"), 2000, this);
		};
		messageDialog->setMFunction(function);
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
	ui->main_chatTab_chatStack->setCurrentIndex(1);
	QQConfigs::UserFriend_C *user = const_cast<QQConfigs::UserFriend_C *>(qvariant_cast<QQConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
	QQConfigs::UserGroup_C *group = const_cast<QQConfigs::UserGroup_C *>(qvariant_cast<QQConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
	// 更新聊天界面的信息与模型的信息
	if (user != nullptr)
	{
		user->m_newMsgCount = 0;
		m_chatListModel->itemFromIndex(index)->setData(0, Qt::UserRole + 6);
		ui->main_chatTab_chatStack_NotNULL_friendNikeNameLab->setText(user->m_nikeName);
	}
	else if (group != nullptr)
	{
		group->m_newMsgCount = 0;
		m_chatListModel->itemFromIndex(index)->setData(0, Qt::UserRole + 6);
		ui->main_chatTab_chatStack_NotNULL_friendNikeNameLab->setText(group->m_nikeName);
	}
	m_messageListModel->clear();
	if (user != nullptr)
	{
		do_loadMessages(user);
	}
	else if (group != nullptr)
	{
		do_loadMessages(group);
	}
	// 尾部空白--主要是美观
	QStandardItem *item = new QStandardItem;
	int width = ui->messageList->width();
	item->setSizeHint(QSize(width, 25));
	m_messageListModel->appendRow(item);
	m_haveInChatList.insert(user->m_ID, m_chatListModel->itemFromIndex(index));
	QTimer::singleShot(50, [this]
					   { ui->messageList->scrollToBottom(); });
	m_activeChatIndex = index;
}

void QQChat::do_userClickSendEmojiButton()
{
	QPoint pos = ui->emojiBtn->mapToGlobal(QPoint(0, 0));
	pos.setY(pos.y() - QQEmoji::getInstance()->height() - 10);
	QQEmoji::getInstance()->setGeometry(QRect(pos, QQEmoji::getInstance()->size()));
	QQEmoji::getInstance()->show();
}

void QQChat::do_userClickSendEmojiButtonFile(const QString &emoji)
{
	switch (type)
	{
	case ClientEnums::EmojiType::unicode:
	{
		QModelIndex index = ui->indexList->currentIndex();
		QQConfigs::UserFriend_C *user = const_cast<QQConfigs::UserFriend_C *>(qvariant_cast<QQConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
		QQConfigs::UserGroup_C *group = const_cast<QQConfigs::UserGroup_C *>(qvariant_cast<QQConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
		QSharedPointer<QQConfigs::MessageBaseConfig> message(new QQConfigs::MessageBaseConfig);
		message->m_messageType = QQConfigs::MessageBaseConfig::MessageType::emoji;
		message->m_senderType = QQConfigs::MessageBaseConfig::SenderType::me;
		message->m_senderID = GlobalValuesAgent::m_user.m_ID;
		if (user != nullptr)
		{
			message->m_receiverID = user->m_ID;
		}
		else if (group != nullptr)
		{
			message->m_receiverID = group->m_ID;
		}
		QString content = QString("type:unicode,data:");
		message->m_content = content.toStdString() + emoji.toStdString();
		message->m_time = QDateTime::currentDateTime().toSecsSinceEpoch();
		do_appendMessage(message);
		break;
	}
	default:
		break;
	}
}

void QQChat::do_userClickSendFileButton()
{
	QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择文件"), "C:/", QString::fromLocal8Bit("所有文件(*.*)"));
	sendFileByName(fileName);
}

void QQChat::do_userClickSendPictureButton()
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
	if (file.size() / 1024 / 1024 > GlobalValuesAgent::MAX_IMAGE_SIZE)
	{
		QQMessageDialog *messageDialog = QQMessageDialog::getInstance();
		messageDialog->removeAllMWidget();
		QPoint pos = this->mapToGlobal(QPoint(0, 0)) + QPoint(this->width() / 2 - 200, this->height() / 2 - 50);
		messageDialog->setGeometry(QRect(pos, QSize(400, 100)));
		messageDialog->openOkButton();
		QLabel *label = new QLabel(messageDialog);
		label->setText(QString::fromLocal8Bit("图片过大，是否改为文件进行发送?"));
		label->setFixedHeight(40);
		label->setAlignment(Qt::AlignCenter);
		label->setFont(QFont("Microsoft YaHei", 11));
		messageDialog->addMWidget(label);
		messageDialog->layout()->setSpacing(10);
		messageDialog->layout()->setContentsMargins(20, 5, 20, 10);
		std::function<void()> function = [this, &fileName]
		{
			do_sendFileByName(fileName);
		};
		messageDialog->setMFunction(function);
		messageDialog->exec();
		return;
	}

	QModelIndex index = ui->indexList->currentIndex();
	QQConfigs::UserFriend_C *user = const_cast<QQConfigs::UserFriend_C *>(qvariant_cast<QQConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
	QQConfigs::UserGroup_C *group = const_cast<QQConfigs::UserGroup_C *>(qvariant_cast<QQConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
	QPair<QString, QString> byteArray = getImageToBase64(fileName);
	QSharedPointer<QQConfigs::MessageBaseConfig> message(new QQConfigs::MessageBaseConfig);
	message->m_messageType = QQConfigs::MessageBaseConfig::MessageType::image;
	message->m_senderType = QQConfigs::MessageBaseConfig::SenderType::me;
	message->m_senderID = GlobalValuesAgent::m_user.m_ID;
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
	do_appendMessage(message);
}

void QQChat::do_userOpenBigWriteButton(bool isChecked)
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

void QQChat::do_userClickSetFontButton()
{
}

void QQChat::do_userClickLookHistroyButton()
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
	if (count > GlobalValuesAgent::MAX_MSG_LENGTH)
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

void QQChat::do_userClickSendAudioButton()
{
	ui->main_chatTab_chatStack_NotNULL_inputStack->setCurrentIndex(1);
	QModelIndex index = ui->indexList->currentIndex();
	QQConfigs::UserFriend_C *user = const_cast<QQConfigs::UserFriend_C *>(qvariant_cast<QQConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
	QQConfigs::UserGroup_C *group = const_cast<QQConfigs::UserGroup_C *>(qvariant_cast<QQConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
	if (user != nullptr)
	{
		QString senderID = QString::fromStdString(GlobalValuesAgent::m_user.m_ID);
		QString receiverID = QString::fromStdString(user->m_ID);
		QString time = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
		QString fileName = senderID + "_" + receiverID + "_" + time + "." + GlobalValuesAgent::AUDIO_FORMAT;
		ui->main_chatTab_chatStack_NotNULL_inputMusic->setMFileName(fileName);
		ui->main_chatTab_chatStack_NotNULL_inputMusic->setMMax(GlobalValuesAgent::MAX_AUDIO_TIME);
		ui->main_chatTab_chatStack_NotNULL_inputMusic->setMMin(GlobalValuesAgent::MIN_AUDIO_TIME);
	}
	else if (group != nullptr)
	{
		QString senderID = QString::fromStdString(GlobalValuesAgent::m_user.m_ID);
		QString receiverID = QString::fromStdString(group->m_ID);
		QString time = QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
		QString fileName = senderID + "_" + receiverID + "_" + time + "." + GlobalValuesAgent::AUDIO_FORMAT;
		ui->main_chatTab_chatStack_NotNULL_inputMusic->setMFileName(fileName);
		ui->main_chatTab_chatStack_NotNULL_inputMusic->setMMax(GlobalValuesAgent::MAX_AUDIO_TIME);
		ui->main_chatTab_chatStack_NotNULL_inputMusic->setMMin(GlobalValuesAgent::MIN_AUDIO_TIME);
	}
	ui->main_chatTab_chatStack_NotNULL_inputMusic->setFocus();
}

void QQChat::do_userClickEscInAudioView()
{
	ui->main_chatTab_chatStack_NotNULL_inputStack->setCurrentIndex(0);
	ui->main_chatTab_chatStack_NotNULL_inputMusic->clearFocus();
}

void QQChat::do_userAudioDataOverTheLimit(int time)
{
	if (time < GlobalValuesAgent::MIN_AUDIO_TIME)
	{
		ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::Bottom, "", QString::fromLocal8Bit("录音时间过短"), 2000, this);
	}
	else if (time > GlobalValuesAgent::MAX_AUDIO_TIME)
	{
		ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::Bottom, "", QString::fromLocal8Bit("录音时间过长"), 2000, this);
	}
}

void QQChat::do_userClickClearInputButton()
{
	// 清除输入框的内容,并设置焦点
	ui->textInput->clear();
	ui->textInput->setFocus();
}

void QQChat::do_userClickSendMessageButton()
{
	if (ui->textInput->toPlainText() == "")
	{
		ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::Bottom, "", QString::fromLocal8Bit("发送内容不能为空"), 3000, this);
		return;
	}
	QModelIndex index = ui->indexList->currentIndex();
	QQConfigs::UserFriend_C *user = const_cast<QQConfigs::UserFriend_C *>(qvariant_cast<QQConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
	QQConfigs::UserGroup_C *group = const_cast<QQConfigs::UserGroup_C *>(qvariant_cast<QQConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
	QSharedPointer<QQConfigs::MessageBaseConfig> message(new QQConfigs::MessageBaseConfig);
	message->m_messageType = QQConfigs::MessageBaseConfig::MessageType::text;
	message->m_senderType = QQConfigs::MessageBaseConfig::SenderType::me;
	message->m_senderID = GlobalValuesAgent::m_user.m_ID;
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