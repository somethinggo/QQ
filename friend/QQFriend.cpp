#include "QQFriend.h"

void QQFriendProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
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

void QQFriendProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	QString name = widget->objectName();
	if (element == CE_PushButton)
	{
		QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
		// 边框
		if (name == "main_chatTab_chatStack_NotNULL_inputGroup_clearBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputGroup_sendBtn" ||
			name == "friendShareOtherBtn" ||
			name == "friendAudioBtn" ||
			name == "friendSendMsgBtn")
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
		if (name == "main_chatTab_chatStack_NotNULL_inputGroup_clearBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputGroup_sendBtn" ||
			name == "friendShareOtherBtn" ||
			name == "friendAudioBtn" ||
			name == "friendSendMsgBtn" ||
			name == "friendNoticeBtn" ||
			name == "groupNoticeBtn")
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
		if (name == "main_chatTab_chatStack_NotNULL_inputToolGroup_emojiBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputToolGroup_fileBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputToolGroup_pictureBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputToolGroup_upperBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputToolGroup_fontBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputToolGroup_historyBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputGroup_audioBtn" ||
			name == "filterNoticeBtn" ||
			name == "clearNoticeBtn")
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
		if (name == "main_chatTab_chatStack_NotNULL_inputToolGroup_upperBtn")
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
		if (name == "friendNoticeBtn" ||
			name == "groupNoticeBtn")
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

QQFriend::QQFriend(QWidget *parent)
	: QWidget(parent), ui(new Ui::QQFriendClass())
{
	ui->setupUi(this);
	m_proxyStyle = new QQFriendProxyStyle;
	m_searchMoreMenu = new ElaMenu(this);
	m_searchMoreMenu->addAction(QIcon(":/widget/common/images/widget/common/searchMoreOne.png"), QString::fromLocal8Bit("发起群聊"));
	m_searchMoreMenu->addAction(QIcon(":/widget/common/images/widget/common/searchMoreTwo.png"), QString::fromLocal8Bit("加好友/群"));

	m_friendsListModel = new QStandardItemModel(this);
	m_groupsListModel = new QStandardItemModel(this);
	m_indexListDelegate = new QQFriendIndexDelegate(this);
	m_indexListDelegate->setItemSize(QSize(350, 90));
	m_indexListDelegate->setMAXStringWidth(300);
	m_friendContextMenu = new ElaMenu(this);
	m_friendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::Plus), QString::fromLocal8Bit("新建分组"));
	m_friendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::TrashCan), QString::fromLocal8Bit("删除该分组"));
	m_friendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::Pen), QString::fromLocal8Bit("重命名该分组"));
	m_tabBar = new QQWidgets::AnimationTabBar(this);
	m_tabBar->setFont(QFont("Microsoft YaHei", 10));
	m_tabBar->setFixedSize(320, 40);
	m_tabBar->addTab(QString::fromLocal8Bit("好友"));
	m_tabBar->addTab(QString::fromLocal8Bit("群聊"));
	m_tabBar->setMargin(5);
	m_tabBar->setBackgroundColor(QColor("#F0F0F0"));
	m_tabBar->setHoverColor(QColor("#F0F0F0"));
	m_tabBar->setSelectColor(Qt::white);
	m_tabBar->setSelectTextColor(QColor("#0099FF"));
	m_tabBar->setAutoFillBackground(true);
	int tabBarLayoutMargin = ui->indexStackWidget->width() / 2 - m_tabBar->width() / 2;
	QHBoxLayout *tabBarLayout = new QHBoxLayout;
	tabBarLayout->setContentsMargins(tabBarLayoutMargin, 0, tabBarLayoutMargin, 0);
	tabBarLayout->addWidget(m_tabBar);

	ui->searchInput->setStyle(QQThemes::Style::SearchProxyStyle::getInstance());
	ui->searchMoreBtn->setStyle(QQThemes::Style::SearchProxyStyle::getInstance());
	ui->searchMoreBtn->setMask(QQFunctions::getRoundedMask(ui->searchMoreBtn->size(), 5));
	ui->layout->insertLayout(3, tabBarLayout);
	ui->friendNoticeBtn->setStyle(m_proxyStyle);
	ui->friendNoticeBtn->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::AngleRight));
	ui->groupNoticeBtn->setStyle(m_proxyStyle);
	ui->groupNoticeBtn->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::AngleRight));
	ui->friendTree->setModel(m_friendsListModel);
	ui->friendTree->setItemDelegate(m_indexListDelegate);
	ui->friendTree->setAnimated(true);
	ui->friendTree->setStyle(m_proxyStyle);
	ui->groupTree->setModel(m_groupsListModel);
	ui->groupTree->setItemDelegate(m_indexListDelegate);
	ui->groupTree->setAnimated(true);
	ui->groupTree->setStyle(m_proxyStyle);
	ui->infoStackWidget->setStyle(m_proxyStyle);
	ui->friendShareOtherBtn->setStyle(m_proxyStyle);
	ui->friendShareOtherBtn->setMask(QQFunctions::getRoundedMask(ui->friendShareOtherBtn->size(), 5));
	ui->friendAudioBtn->setStyle(m_proxyStyle);
	ui->friendAudioBtn->setMask(QQFunctions::getRoundedMask(ui->friendAudioBtn->size(), 5));
	ui->friendSendMsgBtn->setStyle(m_proxyStyle);
	ui->friendSendMsgBtn->setMask(QQFunctions::getRoundedMask(ui->friendSendMsgBtn->size(), 5));

	ui->groupShareOtherBtn->setStyle(m_proxyStyle);
	ui->groupShareOtherBtn->setMask(QQFunctions::getRoundedMask(ui->groupShareOtherBtn->size(), 5));
	ui->groupSendMsgBtn->setStyle(m_proxyStyle);
	ui->groupSendMsgBtn->setMask(QQFunctions::getRoundedMask(ui->groupSendMsgBtn->size(), 5));

	QIcon friendOrGroupNoticeSelectIcon;
	friendOrGroupNoticeSelectIcon.addPixmap(ElaIcon::getInstance()->getElaIcon(ElaIconType::FilterList, 25, 25, 25, Qt::black).pixmap(25, 25), QIcon::Normal, QIcon::Off);
	friendOrGroupNoticeSelectIcon.addPixmap(ElaIcon::getInstance()->getElaIcon(ElaIconType::FilterList, 25, 25, 25, QColor("#0099FF")).pixmap(25, 25), QIcon::Normal, QIcon::On);
	ui->filterNoticeBtn->setIcon(friendOrGroupNoticeSelectIcon);
	ui->filterNoticeBtn->setStyle(m_proxyStyle);
	QIcon friendOrGroupNoticeClearIcon;
	friendOrGroupNoticeClearIcon.addPixmap(ElaIcon::getInstance()->getElaIcon(ElaIconType::TrashCan, 25, 25, 25, Qt::black).pixmap(25, 25), QIcon::Normal, QIcon::Off);
	friendOrGroupNoticeClearIcon.addPixmap(ElaIcon::getInstance()->getElaIcon(ElaIconType::TrashCan, 25, 25, 25, QColor("#0099FF")).pixmap(25, 25), QIcon::Normal, QIcon::On);
	ui->clearNoticeBtn->setIcon(friendOrGroupNoticeClearIcon);
	ui->clearNoticeBtn->setStyle(m_proxyStyle);
	ui->noticeNULLIconLab->setPixmap(ElaIcon::getInstance()->getElaIcon(ElaIconType::Bell, 100, 100, 100).pixmap(100, 100));

	qApp->installEventFilter(this);
	connect(ui->searchMoreBtn, &ElaIconButton::clicked, this, &QQFriend::do_userClickSearchMore);
	connect(ui->friendNoticeBtn, &QPushButton::clicked, this, &QQFriend::do_userClickNotice);
	connect(ui->groupNoticeBtn, &QPushButton::clicked, this, &QQFriend::do_userClickNotice);
	connect(m_tabBar, &QQWidgets::AnimationTabBar::currentChanged, ui->indexStackWidget, &QStackedWidget::setCurrentIndex);
	connect(ui->friendTree, &QTreeView::customContextMenuRequested, this, &QQFriend::do_userClickRightContextMenu);
	connect(m_friendContextMenu, &QMenu::triggered, this, &QQFriend::do_userClickRightContextMenuAction);
	connect(ui->friendTree, &QTreeView::clicked, this, &QQFriend::do_userClickFriendListIndex);
	connect(ui->groupTree, &QTreeView::clicked, this, &QQFriend::do_userClickGroupListIndex);
	connect(ui->friendShareOtherBtn, &QPushButton::clicked, this, &QQFriend::do_userClickShareOther);
	connect(ui->friendAudioBtn, &QPushButton::clicked, this, &QQFriend::do_userClickMusicConnect);
	connect(ui->friendSendMsgBtn, &QPushButton::clicked, this, &QQFriend::do_userClickSendMessage);
	connect(ui->groupShareOtherBtn, &QPushButton::clicked, this, &QQFriend::do_userClickShareOther);
	connect(ui->groupSendMsgBtn, &QPushButton::clicked, this, &QQFriend::do_userClickSendMessage);
}

QQFriend::~QQFriend()
{
	delete m_proxyStyle;
	delete ui;
}
// 过滤事件--用于处理鼠标点击,重置输入框焦点
bool QQFriend::eventFilter(QObject *watch, QEvent *event)
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
		}
	}
	return QWidget::eventFilter(watch, event);
}

void QQFriend::loadPeopleInModelItem(QStandardItem *item, const QQConfigs::FriendConfig &user)
{
	item->setData(QVariant::fromValue(user), Qt::UserRole);
	QPixmap icon = QPixmap::fromImage(QQFunctions::getBase64ToImage(QString::fromStdString(user.m_icon)));
	icon = QQFunctions::getRoundedPixmap(icon.scaled(QQGlobals::g_theme->m_friend_index_icon_size), QQGlobals::g_theme->m_friend_index_icon_size.width() / 2);
	item->setData(icon, Qt::UserRole + 2);
	QString name = QString::fromLocal8Bit(user.m_name.c_str()) + QString("(%1)").arg(user.m_nickname);
	name = QQFunctions::getCalculateText(name, QQGlobals::g_theme->m_friend_index_name_font);
	item->setData(name, Qt::UserRole + 3);
	QString sign = QString::fromLocal8Bit(user.m_sign.c_str());
	sign = QQFunctions::getCalculateText(sign, QQGlobals::g_theme->m_friend_index_sign_font);
	item->setData(sign, Qt::UserRole + 4);
}

void QQFriend::loadPeopleInModelItem(QStandardItem *item, const QQConfigs::GroupConfig &group)
{
	item->setData(QVariant::fromValue(group), Qt::UserRole);
	QPixmap icon = QPixmap::fromImage(QQFunctions::getBase64ToImage(QString::fromStdString(group.m_icon)));
	icon = QQFunctions::getRoundedPixmap(icon.scaled(QQGlobals::g_theme->m_friend_index_icon_size), QQGlobals::g_theme->m_friend_index_icon_size.width() / 2);
	item->setData(icon, Qt::UserRole + 2);
	QString name = QString::fromLocal8Bit(group.m_name.c_str()) + QString("(%1)").arg(group.m_nickname);
	name = QQFunctions::getCalculateText(name, QQGlobals::g_theme->m_friend_index_name_font);
	item->setData(name, Qt::UserRole + 3);
	QString account = QString::fromLocal8Bit(group.m_account.c_str());
	account = QQFunctions::getCalculateText(account, QQGlobals::g_theme->m_friend_index_sign_font);
	item->setData(account, Qt::UserRole + 4);
}

void QQFriend::do_userClickSearchMore()
{
	m_searchMoreMenu->exec(QCursor::pos());
}

void QQFriend::do_userClickSearchMoreAction(QAction *action)
{
	if (action == m_searchMoreMenu->actions()[0])
	{
	}
	else if (action == m_searchMoreMenu->actions()[1])
	{
	}
}

void QQFriend::do_userClickRightContextMenu(const QPoint &pos)
{
	QModelIndex index = ui->friendTree->indexAt(pos);
	if (!index.parent().isValid() && index != m_friendsListModel->index(0, 0))
	{
		m_friendContextMenu->exec(QCursor::pos());
	}
}

void QQFriend::do_userClickRightContextMenuAction(QAction *action)
{
	if (action == m_friendContextMenu->actions()[0])
	{
		QQWidgets::MessageDialog *messageDialog = QQWidgets::MessageDialog::getInstance();
		messageDialog->removeAllWidget();
		QPoint pos = this->mapToGlobal(QPoint(0, 0)) + QPoint(this->width() / 2 - 200, this->height() / 2 - 75);
		messageDialog->setGeometry(QRect(pos, QSize(400, 150)));
		messageDialog->setTittle(QString::fromLocal8Bit("新建分组"));
		messageDialog->setOkButtonEnable(false);
		QLineEdit *lineEdit = new QLineEdit(messageDialog);
		lineEdit->setFixedHeight(45);
		lineEdit->setFont(QFont("Microsoft YaHei", 10));
		lineEdit->setStyleSheet("QLineEdit{border:1px solid #C0C0C0;border-radius:5px;}");
		lineEdit->setPlaceholderText(QString::fromLocal8Bit("请输入分组名称"));
		connect(lineEdit, &QLineEdit::textChanged, [lineEdit, messageDialog]()
				{ messageDialog->setOkButtonEnable(!lineEdit->text().isEmpty()); });
		messageDialog->addWidget(lineEdit);
		std::function<void()> function = [this, lineEdit, messageDialog]
		{
			QString text = lineEdit->text();
			if (!m_friendsGroups.contains(text))
			{
				QStandardItem *item = new QStandardItem;
				item->setText(text);
				item->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::AngleRight, 32, Qt::black));
				m_friendsListModel->appendRow(item);
				m_friendsGroups.insert(text, m_friendsListModel->indexFromItem(item));
				// m_friendsGroupComboBox->addItem(text);
			}
			else
			{
				messageDialog->setAlertInformation(QString::fromLocal8Bit("分组已存在"));
			}
		};
		messageDialog->setFunction(function);
		messageDialog->exec();
	}
	else if (action == m_friendContextMenu->actions()[1])
	{
		QQWidgets::MessageDialog *messageDialog = QQWidgets::MessageDialog::getInstance();
		messageDialog->removeAllWidget();
		QPoint pos = this->mapToGlobal(QPoint(0, 0)) + QPoint(this->width() / 2 - 200, this->height() / 2 - 50);
		messageDialog->setGeometry(QRect(pos, QSize(400, 100)));
		messageDialog->setTittle(QString::fromLocal8Bit("删除该分组"));
		messageDialog->setOkButtonEnable(false);
		QLabel *label = new QLabel(messageDialog);
		label->setText(QString::fromLocal8Bit("您确定要删除该分组?"));
		label->setFixedHeight(40);
		label->setAlignment(Qt::AlignCenter);
		label->setFont(QFont("Microsoft YaHei", 11));
		messageDialog->addWidget(label);
		messageDialog->layout()->setSpacing(10);
		messageDialog->layout()->setContentsMargins(20, 5, 20, 10);
		std::function<void()> function = [this]
		{
			QModelIndex topIndex = m_friendsListModel->index(0, 0);
			QStandardItem *topItem = m_friendsListModel->itemFromIndex(topIndex);
			QModelIndex index = ui->friendTree->currentIndex();
			QStandardItem *item = m_friendsListModel->itemFromIndex(index);
			while (item->rowCount() != 0)
			{
				QStandardItem *childItem = item->child(0, 0);
				item->takeRow(0);
				topItem->appendRow(childItem);
			}
			m_friendsGroups.remove(index.data(Qt::DisplayRole).toString());
			// m_friendsGroupComboBox->removeItem(index.row());
			m_friendsListModel->removeRow(index.row());
		};
		messageDialog->setFunction(function);
		messageDialog->exec();
	}
	else if (action == m_friendContextMenu->actions()[2])
	{
		QQWidgets::MessageDialog *messageDialog = QQWidgets::MessageDialog::getInstance();
		messageDialog->removeAllWidget();
		QPoint pos = this->mapToGlobal(QPoint(0, 0)) + QPoint(this->width() / 2 - 200, this->height() / 2 - 75);
		messageDialog->setGeometry(QRect(pos, QSize(400, 150)));
		messageDialog->setTittle(QString::fromLocal8Bit("重命名该分组"));
		messageDialog->setOkButtonEnable(false);
		QLineEdit *lineEdit = new QLineEdit(messageDialog);
		QModelIndex index = ui->friendTree->currentIndex();
		lineEdit->setText(index.data(Qt::DisplayRole).toString());
		lineEdit->setFixedHeight(45);
		lineEdit->setFont(QFont("Microsoft YaHei", 10));
		lineEdit->setStyleSheet("QLineEdit{border:1px solid #C0C0C0;border-radius:5px;}");
		lineEdit->setPlaceholderText(QString::fromLocal8Bit("请输入分组名称"));
		connect(lineEdit, &QLineEdit::textChanged, [lineEdit, messageDialog]()
				{ messageDialog->setOkButtonEnable(!lineEdit->text().isEmpty()); });
		messageDialog->addWidget(lineEdit);
		std::function<void()> function = [this, &index, lineEdit, messageDialog]
		{
			QString text = lineEdit->text();
			if (!m_friendsGroups.contains(text))
			{
				m_friendsListModel->itemFromIndex(index)->setText(text);
				m_friendsGroups[text] = index;
			}
			else
			{
				messageDialog->setAlertInformation(QString::fromLocal8Bit("分组已存在"));
			}
		};
		messageDialog->setFunction(function);
		messageDialog->exec();
	}
}

void QQFriend::do_userClickNotice()
{
	ui->infoStackWidget->setCurrentIndex(3);
	if (sender() == ui->friendNoticeBtn)
	{
		ui->noticeNameLab->setText(QString::fromLocal8Bit("好友通知"));
	}
	else if (sender() == ui->groupNoticeBtn)
	{
		ui->noticeNameLab->setText(QString::fromLocal8Bit("群通知"));
	}
}

void QQFriend::do_userClickFriendListIndex(const QModelIndex &index)
{
	if (!index.parent().isValid())
	{
		// 如果点击的是顶部的分组,则展开或者收缩,并且改变图标
		QStandardItem *item = m_friendsListModel->itemFromIndex(index);
		bool isDecorated = ui->friendTree->isExpanded(index);
		QIcon originalIcon = item->icon();
		QPixmap originalPixmap = originalIcon.pixmap(originalIcon.availableSizes().first());
		QPixmap rotatedPixmap;
		if (isDecorated)
		{
			rotatedPixmap = originalPixmap.transformed(QTransform().rotate(-90));
		}
		else
		{
			rotatedPixmap = originalPixmap.transformed(QTransform().rotate(90));
		}
		QIcon rotatedIcon(rotatedPixmap);
		item->setIcon(rotatedIcon);
		ui->friendTree->setExpanded(index, !isDecorated);
	}
	else
	{
		ui->infoStackWidget->setCurrentIndex(1);
		QQConfigs::FriendConfig *user = qvariant_cast<QQConfigs::FriendConfig *>(index.data(Qt::UserRole));
		QPixmap icon = QPixmap::fromImage(QQFunctions::getBase64ToImage(QString::fromLocal8Bit(user->m_icon.c_str())));
		ui->friendIconLab->setPixmap(icon);
		ui->friendNameLab->setText(QString::fromLocal8Bit(user->m_name.c_str()));
		QString account = QString("QQ ") + QString::fromLocal8Bit(user->m_account.c_str());
		ui->friendCountLab->setText(account);
	}
}

void QQFriend::do_userClickGroupListIndex(const QModelIndex &index)
{
	if (!index.parent().isValid())
	{
		// 如果点击的是顶部的分组,则展开或者收缩,并且改变图标
		QStandardItem *item = m_groupsListModel->itemFromIndex(index);
		bool isDecorated = ui->groupTree->isExpanded(index);
		QIcon originalIcon = item->icon();
		QPixmap originalPixmap = originalIcon.pixmap(originalIcon.availableSizes().first());
		QPixmap rotatedPixmap;
		if (isDecorated)
		{
			rotatedPixmap = originalPixmap.transformed(QTransform().rotate(-90));
		}
		else
		{
			rotatedPixmap = originalPixmap.transformed(QTransform().rotate(90));
		}
		QIcon rotatedIcon(rotatedPixmap);
		item->setIcon(rotatedIcon);
		ui->groupTree->setExpanded(index, !isDecorated);
	}
	else
	{
		// 如果点击的是群聊,则展示群聊的信息
		// 更新基础信息
		ui->infoStackWidget->setCurrentIndex(2);
		QModelIndex index = ui->groupTree->currentIndex();
		QQConfigs::GroupConfig *group = qvariant_cast<QQConfigs::GroupConfig *>(index.data(Qt::UserRole));
		QString iconBase64String = QString::fromLocal8Bit(group->m_icon.c_str());
		QPixmap icon = QPixmap::fromImage(QQFunctions::getBase64ToImage(QString::fromLocal8Bit(group->m_icon.c_str())));
		ui->groupIconLab->setPixmap(icon);
		ui->groupNameLab->setText(QString::fromLocal8Bit(group->m_name.c_str()));
		QString account = QString::fromLocal8Bit("群聊 ") + QString::fromLocal8Bit(group->m_account.c_str());
		ui->friendCountLab->setText(account);
	}
}

void QQFriend::do_userSelectedGroupChanged(const QString &text)
{
	// 如果分组改变,则将选中的好友移动到新的分组
	QModelIndex index = ui->friendTree->currentIndex();
	QQConfigs::FriendConfig *user = qvariant_cast<QQConfigs::FriendConfig *>(index.data(Qt::UserRole));
	QModelIndex previousIndex = m_friendsGroups[user->m_group];
	QModelIndex currentIndex = m_friendsGroups[text];
	QStandardItem *item = m_friendsListModel->itemFromIndex(index);
	m_friendsListModel->itemFromIndex(previousIndex)->takeRow(item->row());
	m_friendsListModel->itemFromIndex(currentIndex)->appendRow(item);
	user->m_group = text;
}

void QQFriend::do_userClickShareOther()
{
}

void QQFriend::do_userClickMusicConnect()
{
}

void QQFriend::do_userClickSendMessage()
{
	QJsonObject sendData;
	sendData.insert("version", "1.0");
	sendData.insert("sender", "friend");
	sendData.insert("action", "requestchat");
	QJsonObject data;
	if (sender() == ui->friendSendMsgBtn)
	{
		QModelIndex index = ui->friendTree->currentIndex();
		QQConfigs::FriendConfig *user = qvariant_cast<QQConfigs::FriendConfig *>(index.data(Qt::UserRole));
		data.insert("type", "friend");
		data.insert("chatId", QString::fromStdString(user->m_ID));
	}
	else if (sender() == ui->groupSendMsgBtn)
	{
		QModelIndex index = ui->friendTree->currentIndex();
		QQConfigs::GroupConfig *group = qvariant_cast<QQConfigs::GroupConfig *>(index.data(Qt::UserRole));
		data.insert("type", "group");
		data.insert("chatId", QString::fromStdString(group->m_ID));
	}
	sendData.insert("data", data);
	Q_MSEND_EVENT(QQEnums::requestchat, QJsonDocument(sendData).toJson());
}