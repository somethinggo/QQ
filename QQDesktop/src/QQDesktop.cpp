#include "QQDesktop.h"

void QMCommonProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
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

void QMCommonProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	QString name = widget->objectName();
	if (element == CE_PushButton)
	{
		QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
		// 边框
		if (name == "main_chatTab_chatStack_NotNULL_inputGroup_clearBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputGroup_sendBtn" ||
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
		if (name == "main_chatTab_chatStack_NotNULL_inputGroup_clearBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputGroup_sendBtn" ||
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
		if (name == "main_chatTab_chatStack_NotNULL_inputToolGroup_emojiBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputToolGroup_fileBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputToolGroup_pictureBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputToolGroup_upperBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputToolGroup_fontBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputToolGroup_historyBtn" ||
			name == "main_chatTab_chatStack_NotNULL_inputGroup_audioBtn" ||
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

QRect QMCommonProxyStyle::subElementRect(SubElement element, const QStyleOption *option, const QWidget *widget) const
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
		if (name == "main_chatTab_chatStack_NotNULL_textInput")
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

QQDesktop::QQDesktop(QWidget *parent)
	: QWidget(parent), ui(new Ui::QQDesktopClass())
{
	ui->setupUi(this);
	m_commonStyle = new QMCommonProxyStyle;
	m_searchStyle = new QMSearchProxyStyle;
	m_appBar = new QQAppBar(this, ClientEnums::AppBarHintType::WindowsHint);
	qobject_cast<QVBoxLayout *>(this->layout())->insertWidget(0, m_appBar);
	m_searchAction = new QAction(this);
	m_searchAction->setIcon(QIcon(":/widget/common/images/widget/common/searchLab.png"));
	m_searchMoreMenu = new ElaMenu(this);
	m_searchMoreMenu->addAction(QIcon(":/widget/common/images/widget/common/searchMoreOne.png"), QString::fromLocal8Bit("发起群聊"));
	m_searchMoreMenu->addAction(QIcon(":/widget/common/images/widget/common/searchMoreTwo.png"), QString::fromLocal8Bit("加好友/群"));
	m_buttonGroup = new QButtonGroup(this);
	QList<QQOptionFillButton *> buttonList = ui->option_mainGroup->findChildren<QQOptionFillButton *>();
	for (int i = 0; i < buttonList.count(); ++i)
	{
		buttonList[i]->setMask(getRoundedRect(QSize(50, 50), 5));
		m_buttonGroup->addButton(buttonList[i], i);
	}
	buttonList[0]->setChecked(true);
	QQMessageDialog::getInstance()->setMShadowWidget(this);
	ui->option_mainGroup->setStyle(m_commonStyle);
	ui->option_otherGroup->setStyle(m_commonStyle);

	// main
	ui->mainWidget->tabBar()->setHidden(true);
	ui->mainWidget->setCurrentIndex(0);
	ui->main_chatTab_listStack->setCurrentIndex(0);
	ui->main_chatTab_chatStack->setCurrentIndex(0);
	ui->main_friendAndGroupTab_informationStack->setCurrentIndex(0);

	// chat
	m_chatListModel = new QStandardItemModel(this);
	m_chatListDelegate = new QQSinglePeopleWidgetDelegate(this);
	m_chatListDelegate->setMItemSize(QSize(350, 90));
	m_chatListDelegate->setMMAXStringWidth(300);
	m_messageListModel = new QStandardItemModel(this);
	m_chatMessageDelegate = new QQChatMessageSingleViewDelegate(this);
	m_chatFriendContextMenu = new ElaMenu(this);
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::ArrowUp), QString::fromLocal8Bit("置顶"));
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::Ban), QString::fromLocal8Bit("消息免打扰"));
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::TrashCan), QString::fromLocal8Bit("删除该聊天"));
	m_chatFriendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::BrakeWarning), QString::fromLocal8Bit("举报"));
	m_friendContextMenu = new ElaMenu(this);

	ui->main_chatTab_searchInput->setStyle(m_searchStyle);
	ui->main_chatTab_searchInput->addAction(m_searchAction, QLineEdit::LeadingPosition);
	ui->main_chatTab_searchMoreBtn->setStyle(m_searchStyle);
	ui->main_chatTab_searchMoreBtn->setMask(getRoundedRect(ui->main_chatTab_searchMoreBtn->size(), 5));
	ui->main_chatTab_listStack_NotNULL_listView->setModel(m_chatListModel);
	ui->main_chatTab_listStack_NotNULL_listView->setItemDelegate(m_chatListDelegate);
	ui->main_chatTab_listStack_NotNULL_listView->setMMenu(m_chatFriendContextMenu);
	ui->main_chatTab_chatStack_NotNULL_friendNikeNameLab->setStyle(m_commonStyle);
	ui->main_chatTab_chatStack_NotNULL_msgListView->setModel(m_messageListModel);
	ui->main_chatTab_chatStack_NotNULL_msgListView->setItemDelegate(m_chatMessageDelegate);
	ui->main_chatTab_chatStack_NotNULL_msgListView->setStyle(m_commonStyle);
	ui->main_chatTab_chatStack_NotNULL_inputToolGroup->setStyle(m_commonStyle);
	ui->main_chatTab_chatStack_NotNULL_inputToolGroup_emojiBtn->setStyle(m_commonStyle);
	ui->main_chatTab_chatStack_NotNULL_inputToolGroup_fileBtn->setStyle(m_commonStyle);
	ui->main_chatTab_chatStack_NotNULL_inputToolGroup_pictureBtn->setStyle(m_commonStyle);
	ui->main_chatTab_chatStack_NotNULL_inputToolGroup_upperBtn->setStyle(m_commonStyle);
	ui->main_chatTab_chatStack_NotNULL_inputToolGroup_fontBtn->setStyle(m_commonStyle);
	ui->main_chatTab_chatStack_NotNULL_inputToolGroup_historyBtn->setStyle(m_commonStyle);
	ui->main_chatTab_chatStack_NotNULL_textInput->setStyle(m_commonStyle);
	ui->main_chatTab_chatStack_NotNULL_inputGroup->setStyle(m_commonStyle);
	ui->main_chatTab_chatStack_NotNULL_inputGroup_audioBtn->setStyle(m_commonStyle);
	ui->main_chatTab_chatStack_NotNULL_inputGroup_clearBtn->setMask(getRoundedRect(ui->main_chatTab_chatStack_NotNULL_inputGroup_clearBtn->size(), 10));
	ui->main_chatTab_chatStack_NotNULL_inputGroup_clearBtn->setStyle(m_commonStyle);
	ui->main_chatTab_chatStack_NotNULL_inputGroup_sendBtn->setMask(getRoundedRect(ui->main_chatTab_chatStack_NotNULL_inputGroup_sendBtn->size(), 10));
	ui->main_chatTab_chatStack_NotNULL_inputGroup_sendBtn->setStyle(m_commonStyle);

	// friendAndGroup
	m_friendsListModel = new QStandardItemModel(this);
	m_groupsListModel = new QStandardItemModel(this);
	m_friendAndGroupDelegate = new QQSingleFriendWidgetDelegate(this);
	m_friendAndGroupDelegate->setMItemSize(QSize(350, 90));
	m_friendAndGroupDelegate->setMMAXStringWidth(300);
	m_friendContextMenu = new ElaMenu(this);
	m_friendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::Plus), QString::fromLocal8Bit("新建分组"));
	m_friendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::TrashCan), QString::fromLocal8Bit("删除该分组"));
	m_friendContextMenu->addAction(ElaIcon::getInstance()->getElaIcon(ElaIconType::Pen), QString::fromLocal8Bit("重命名该分组"));
	m_friendAndGroupTabBar = new QQActionTabBar(this);
	m_friendAndGroupTabBar->setFont(QFont("Microsoft YaHei", 10));
	m_friendAndGroupTabBar->setFixedSize(320, 40);
	m_friendAndGroupTabBar->addTab(QString::fromLocal8Bit("好友"));
	m_friendAndGroupTabBar->addTab(QString::fromLocal8Bit("群聊"));
	int friendAndGroupTabBarLayoutMargin = ui->main_friendAndGroupTab_listStack->width() / 2 - m_friendAndGroupTabBar->width() / 2;
	m_friendAndGroupTabBar->setContentsMargins(friendAndGroupTabBarLayoutMargin, friendAndGroupTabBarLayoutMargin, friendAndGroupTabBarLayoutMargin, friendAndGroupTabBarLayoutMargin);
	m_friendAndGroupTabBar->setMMargin(5);
	m_friendAndGroupTabBar->setMBackgroundColor(QColor("#F5F5F5"));
	m_friendAndGroupTabBar->setMHoverColor(QColor("#F5F5F5"));
	m_friendAndGroupTabBar->setMSelectColor(QColor("#FFFFFF"));
	m_friendAndGroupTabBar->setMSelectTextColor(QColor("#0099FF"));

	QLineEdit *friendNikeNameInput = new QLineEdit(QQInformationListWidget::getInstance());
	friendNikeNameInput->setObjectName("friendNikeNameInput");
	QPalette friendNikeNameInputPalette = friendNikeNameInput->palette();
	friendNikeNameInputPalette.setColor(QPalette::Base, QColor("#F5F5F5"));
	friendNikeNameInput->setPalette(friendNikeNameInputPalette);
	friendNikeNameInput->setAutoFillBackground(true);
	friendNikeNameInput->setStyle(m_searchStyle);
	friendNikeNameInput->setReadOnly(true);
	friendNikeNameInput->setPlaceholderText(QString::fromLocal8Bit("设置好友备注"));
	friendNikeNameInput->setAlignment(Qt::AlignRight);
	QQInformationListWidget::getInstance()->addMInformation(QString::fromLocal8Bit("备注"), friendNikeNameInput, ElaIcon::getInstance()->getElaIcon(ElaIconType::PenLine));
	m_friendsGroupComboBox = new ElaComboBox(QQInformationListWidget::getInstance());
	m_friendsGroupComboBox->setFixedSize(120, 30);
	m_friendsGroupComboBox->addItems(QStringList() << QString::fromLocal8Bit("我的好友") << QString::fromLocal8Bit("特别关心"));
	QQInformationListWidget::getInstance()->addMInformation(QString::fromLocal8Bit("好友分组"), m_friendsGroupComboBox, ElaIcon::getInstance()->getElaIcon(ElaIconType::UserGroup));

	ui->main_friendAndGroupTab_searchInput->setStyle(m_searchStyle);
	ui->main_friendAndGroupTab_searchInput->addAction(m_searchAction, QLineEdit::LeadingPosition);
	ui->main_friendAndGroupTab_searchMoreBtn->setStyle(m_searchStyle);
	ui->main_friendAndGroupTab_searchMoreBtn->setMask(getRoundedRect(ui->main_friendAndGroupTab_searchMoreBtn->size(), 5));
	ui->main_friendAndGroupTab_listLayout->insertWidget(3, m_friendAndGroupTabBar);
	ui->main_friendAndGroupTab_friendNoticeButton->setStyle(m_commonStyle);
	ui->main_friendAndGroupTab_friendNoticeButton->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::AngleRight));
	ui->main_friendAndGroupTab_groupNoticeButton->setStyle(m_commonStyle);
	ui->main_friendAndGroupTab_groupNoticeButton->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::AngleRight));
	ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView->setModel(m_friendsListModel);
	ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView->setItemDelegate(m_friendAndGroupDelegate);
	ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView->setAnimated(true);
	ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView->setStyle(m_commonStyle);
	ui->main_friendAndGroupTab_listStack_NotNULL_groupsTab_treeView->setModel(m_groupsListModel);
	ui->main_friendAndGroupTab_listStack_NotNULL_groupsTab_treeView->setItemDelegate(m_friendAndGroupDelegate);
	ui->main_friendAndGroupTab_listStack_NotNULL_groupsTab_treeView->setAnimated(true);
	ui->main_friendAndGroupTab_listStack_NotNULL_groupsTab_treeView->setStyle(m_commonStyle);
	ui->main_friendAndGroupTab_informationStack->setStyle(m_commonStyle);
	qobject_cast<QVBoxLayout *>(ui->main_friendAndGroupTab_informationStack_NotNULL_friendWidget->layout())->insertWidget(2, QQInformationListWidget::getInstance());
	ui->main_friendAndGroupTab_informationStack_NotNULL_friendShareOtherBtn->setStyle(m_commonStyle);
	ui->main_friendAndGroupTab_informationStack_NotNULL_friendShareOtherBtn->setMask(getRoundedRect(ui->main_friendAndGroupTab_informationStack_NotNULL_friendShareOtherBtn->size(), 5));
	ui->main_friendAndGroupTab_informationStack_NotNULL_friendMusicConnectBtn->setStyle(m_commonStyle);
	ui->main_friendAndGroupTab_informationStack_NotNULL_friendMusicConnectBtn->setMask(getRoundedRect(ui->main_friendAndGroupTab_informationStack_NotNULL_friendMusicConnectBtn->size(), 5));
	ui->main_friendAndGroupTab_informationStack_NotNULL_friendSendMsgBtn->setStyle(m_commonStyle);
	ui->main_friendAndGroupTab_informationStack_NotNULL_friendSendMsgBtn->setMask(getRoundedRect(ui->main_friendAndGroupTab_informationStack_NotNULL_friendSendMsgBtn->size(), 5));

	ui->main_friendAndGroupTab_informationStack_NotNULL_groupShareOtherBtn->setStyle(m_commonStyle);
	ui->main_friendAndGroupTab_informationStack_NotNULL_groupShareOtherBtn->setMask(getRoundedRect(ui->main_friendAndGroupTab_informationStack_NotNULL_groupShareOtherBtn->size(), 5));
	ui->main_friendAndGroupTab_informationStack_NotNULL_groupSendMsgBtn->setStyle(m_commonStyle);
	ui->main_friendAndGroupTab_informationStack_NotNULL_groupSendMsgBtn->setMask(getRoundedRect(ui->main_friendAndGroupTab_informationStack_NotNULL_groupSendMsgBtn->size(), 5));

	QIcon friendOrGroupNoticeSelectIcon;
	friendOrGroupNoticeSelectIcon.addPixmap(ElaIcon::getInstance()->getElaIcon(ElaIconType::FilterList, 25, 25, 25, Qt::black).pixmap(25, 25), QIcon::Normal, QIcon::Off);
	friendOrGroupNoticeSelectIcon.addPixmap(ElaIcon::getInstance()->getElaIcon(ElaIconType::FilterList, 25, 25, 25, QColor("#0099FF")).pixmap(25, 25), QIcon::Normal, QIcon::On);
	ui->main_friendAndGroupTab_informationStack_NotNULL_noNoticeWidget_selectBtn->setIcon(friendOrGroupNoticeSelectIcon);
	ui->main_friendAndGroupTab_informationStack_NotNULL_noNoticeWidget_selectBtn->setStyle(m_commonStyle);
	QIcon friendOrGroupNoticeClearIcon;
	friendOrGroupNoticeClearIcon.addPixmap(ElaIcon::getInstance()->getElaIcon(ElaIconType::TrashCan, 25, 25, 25, Qt::black).pixmap(25, 25), QIcon::Normal, QIcon::Off);
	friendOrGroupNoticeClearIcon.addPixmap(ElaIcon::getInstance()->getElaIcon(ElaIconType::TrashCan, 25, 25, 25, QColor("#0099FF")).pixmap(25, 25), QIcon::Normal, QIcon::On);
	ui->main_friendAndGroupTab_informationStack_NotNULL_noNoticeWidget_clearBtn->setIcon(friendOrGroupNoticeClearIcon);
	ui->main_friendAndGroupTab_informationStack_NotNULL_noNoticeWidget_clearBtn->setStyle(m_commonStyle);
	ui->main_friendAndGroupTab_informationStack_NotNULL_noNoticeWidget_iconLab->setPixmap(ElaIcon::getInstance()->getElaIcon(ElaIconType::Bell, 100, 100, 100).pixmap(100, 100));

	qApp->installEventFilter(this);
	connectInit();
	do_loadingInformation();
}

QQDesktop::~QQDesktop()
{
	for (int i = 0; i < m_friends.count(); ++i)
	{
		delete m_friends[i];
		m_friends[i] = nullptr;
	}
	delete m_commonStyle;
	delete m_searchStyle;
	delete ui;
}

void QQDesktop::connectInit()
{
	connect(m_buttonGroup, &QButtonGroup::idClicked, ui->mainWidget, &QTabWidget::setCurrentIndex);		// 切换主界面
	connect(m_searchMoreMenu, &QMenu::triggered, this, &QQDesktop::do_userClickSearchMoreButtonAction); // 搜索更多按钮选项
	// chat connect
	connect(ui->main_chatTab_searchMoreBtn, &QPushButton::clicked, this, &QQDesktop::do_userClickSearchMoreButton);										// chat界面 搜索更多按钮
	connect(ui->main_chatTab_listStack_NotNULL_listView, &QListView::clicked, this, &QQDesktop::do_chat_userClickChatIndex);							// chat界面 点击聊天索引列表索引
	connect(m_chatFriendContextMenu, &QMenu::triggered, this, &QQDesktop::do_chat_userClickRightContextMenuAction);										// chat界面 聊天索引列表右键菜单
	connect(ui->main_chatTab_chatStack_NotNULL_inputToolGroup_emojiBtn, &QPushButton::clicked, this, &QQDesktop::do_chat_userClickSendEmojiButton);		// chat界面 点击发送表情按钮
	connect(QQChatEmojiView::getInstance(), &QQChatEmojiView::sign_emojiItemClicked, this, &QQDesktop::do_chat_userClickSendEmojiButtonFile);			// chat界面 点击具体发送表情
	connect(ui->main_chatTab_chatStack_NotNULL_inputToolGroup_fileBtn, &QPushButton::clicked, this, &QQDesktop::do_chat_userClickSendFileButton);		// chat界面 点击发送文件按钮
	connect(ui->main_chatTab_chatStack_NotNULL_inputToolGroup_pictureBtn, &QPushButton::clicked, this, &QQDesktop::do_chat_userClickSendPictureButton); // chat界面 点击发送图片按钮
	connect(ui->main_chatTab_chatStack_NotNULL_inputToolGroup_upperBtn, &QPushButton::toggled, this, &QQDesktop::do_chat_userOpenBigWriteButton);		// chat界面 点击打开大写按钮
	connect(ui->main_chatTab_chatStack_NotNULL_inputToolGroup_fontBtn, &QPushButton::clicked, this, &QQDesktop::do_chat_userClickSetFontButton);		// chat界面 点击设置字体按钮
	connect(ui->main_chatTab_chatStack_NotNULL_inputToolGroup_historyBtn, &QPushButton::clicked, this, &QQDesktop::do_chat_userClickLookHistroyButton); // chat界面 点击查看历史消息按钮
	connect(ui->main_chatTab_chatStack_NotNULL_textInput, &QTextEdit::textChanged, this, &QQDesktop::do_chat_limitUserInputTextCount);					// chat界面 限制用户输入文本长度
	connect(ui->main_chatTab_chatStack_NotNULL_inputGroup_audioBtn, &QPushButton::clicked, this, &QQDesktop::do_chat_userClickSendAudioButton);			// chat界面 点击发送语音按钮
	connect(ui->main_chatTab_chatStack_NotNULL_inputGroup_clearBtn, &QPushButton::clicked, this, &QQDesktop::do_chat_userClickClearInputButton);		// chat界面 点击清空输入按钮
	connect(ui->main_chatTab_chatStack_NotNULL_inputGroup_sendBtn, &QPushButton::clicked, this, &QQDesktop::do_chat_userClickSendMessageButton);		// chat界面 点击发送消息按钮
	// friend connect
	connect(ui->main_friendAndGroupTab_searchMoreBtn, &QPushButton::clicked, this, &QQDesktop::do_userClickSearchMoreButton);												  // friend界面 搜索更多按钮
	connect(ui->main_friendAndGroupTab_friendNoticeButton, &QPushButton::clicked, this, &QQDesktop::do_friend_userClickNoticeButton);										  // friend界面 好友通知按钮
	connect(ui->main_friendAndGroupTab_groupNoticeButton, &QPushButton::clicked, this, &QQDesktop::do_friend_userClickNoticeButton);										  // friend界面 群通知按钮
	connect(m_friendAndGroupTabBar, &QTabBar::currentChanged, this, &QQDesktop::do_friend_userClickOption);																	  // friend界面 好友与群的选项卡
	connect(ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView, &QTreeView::customContextMenuRequested, this, &QQDesktop::do_friend_userClickRightContextMenu); // friend界面 好友列表右键菜单
	connect(m_friendContextMenu, &QMenu::triggered, this, &QQDesktop::do_friend_userClickRightContextMenuAction);															  // friend界面 好友列表右键菜单选项
	connect(ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView, &QTreeView::clicked, this, &QQDesktop::do_friend_userClickFriendListIndex);					  // friend界面 好友列表点击
	connect(ui->main_friendAndGroupTab_listStack_NotNULL_groupsTab_treeView, &QTreeView::clicked, this, &QQDesktop::do_friend_userClickGroupListIndex);						  // friend界面 群列表点击
	connect(ui->main_friendAndGroupTab_informationStack_NotNULL_friendShareOtherBtn, &QPushButton::clicked, this, &QQDesktop::do_friend_userClickShareOtherButton);			  // friend界面 好友分享按钮
	connect(ui->main_friendAndGroupTab_informationStack_NotNULL_friendMusicConnectBtn, &QPushButton::clicked, this, &QQDesktop::do_friend_userClickMusicConnectButton);		  // friend界面 好友音视频通话按钮
	connect(ui->main_friendAndGroupTab_informationStack_NotNULL_friendSendMsgBtn, &QPushButton::clicked, this, &QQDesktop::do_friend_userClickSendMessageButton);			  // friend界面 好友发送消息按钮
	connect(m_friendsGroupComboBox, &ElaComboBox::currentTextChanged, this, &QQDesktop::do_friend_userSelectedGroupChanged);												  // friend界面 好友分组选择
	connect(ui->main_friendAndGroupTab_informationStack_NotNULL_groupShareOtherBtn, &QPushButton::clicked, this, &QQDesktop::do_friend_userClickShareOtherButton);			  // friend界面 群分享按钮
	connect(ui->main_friendAndGroupTab_informationStack_NotNULL_groupSendMsgBtn, &QPushButton::clicked, this, &QQDesktop::do_friend_userClickSendMessageButton);			  // friend界面 群发送消息按钮
}

void QQDesktop::do_loadingInformation()
{
	// test
	{
		for (int i = 0; i < 10; ++i)
		{
			ClientConfigs::UserFriend_C *user;
			user = new ClientConfigs::UserFriend_C;
			user->m_ID = std::to_string(i);
			user->m_account = std::to_string((i + 1) * 10 + i);
			QImage image = QImage(":/widget/common/images/widget/common/QQ.png");
			user->m_icon = getImageToBase64(image).first.toStdString();
			user->m_name = "helloworld";
			user->m_newMsgCount = rand() % 100;
			user->m_nikeName = QString("hello%1,%2,%3").arg(i).arg(i).arg(i);
			user->m_sign = "fuckyou";
			user->m_lastMsg = "fuckyou";
			user->m_lastMsgTime = QDateTime::currentDateTime();
			user->m_group = QString::fromLocal8Bit("我的好友");
			for (int j = 0; j < 50; ++j)
			{
				QSharedPointer<ClientConfigs::MessageBaseConfig> message(new ClientConfigs::MessageBaseConfig);
				message->m_messageType = ClientConfigs::MessageBaseConfig::MessageType::text;
				message->m_senderType = ClientConfigs::MessageBaseConfig::SenderType::me;
				message->m_content = getGenerateRandomText(20, 30).toStdString();
				user->m_msgList.append(message);
			}
			m_friends.append(user);
		}
		QStandardItem *item1 = new QStandardItem;
		item1->setText(QString::fromLocal8Bit("我的好友"));
		item1->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::AngleRight, 32, Qt::black));
		m_friendsListModel->appendRow(item1);
		QStandardItem *item2 = new QStandardItem;
		item2->setText(QString::fromLocal8Bit("特别关心"));
		item2->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::AngleRight, 32, Qt::black));
		m_friendsListModel->appendRow(item2);
		m_friendsGroups.insert(QString::fromLocal8Bit("我的好友"), item1->index());
		m_friendsGroups.insert(QString::fromLocal8Bit("特别关心"), item2->index());

		ui->main_chatTab_listStack->setCurrentIndex(1);
		for (int i = 0; i < 10; ++i)
		{
			QStandardItem *chatItem = new QStandardItem;
			do_peopleInModelItem(chatItem, m_friends[i]);
			m_chatListModel->appendRow(chatItem);
			m_haveInChatList.insert(m_friends[i]->m_ID, chatItem);

			QStandardItem *friendItem = new QStandardItem;
			do_peopleInModelItem(friendItem, m_friends[i], ClientEnums::PeopleWidgetType::friends);
			item1->appendRow(friendItem);
		}
	}
	QStandardItem *groupItem1 = new QStandardItem;
	groupItem1->setText(QString::fromLocal8Bit("置顶群聊"));
	groupItem1->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::AngleRight, 32, Qt::black));
	m_groupsListModel->appendRow(groupItem1);
	QStandardItem *groupItem2 = new QStandardItem;
	groupItem2->setText(QString::fromLocal8Bit("未命名的群聊"));
	groupItem2->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::AngleRight, 32, Qt::black));
	m_groupsListModel->appendRow(groupItem2);
	QStandardItem *groupItem3 = new QStandardItem;
	groupItem3->setText(QString::fromLocal8Bit("我创建的群聊"));
	groupItem3->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::AngleRight, 32, Qt::black));
	m_groupsListModel->appendRow(groupItem3);
	QStandardItem *groupItem4 = new QStandardItem;
	groupItem4->setText(QString::fromLocal8Bit("我管理的群聊"));
	groupItem4->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::AngleRight, 32, Qt::black));
	m_groupsListModel->appendRow(groupItem4);
	QStandardItem *groupItem5 = new QStandardItem;
	groupItem5->setText(QString::fromLocal8Bit("我加入的群聊"));
	groupItem5->setIcon(ElaIcon::getInstance()->getElaIcon(ElaIconType::AngleRight, 32, Qt::black));
	m_groupsListModel->appendRow(groupItem5);
}

// 过滤事件--用于处理鼠标点击,重置输入框焦点
bool QQDesktop::eventFilter(QObject *watch, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		if (mouseEvent->button() == Qt::LeftButton)
		{
			QPoint pos = mouseEvent->globalPos();
			int index = ui->mainWidget->currentIndex();
			if (index == 0)
			{
				QPoint main_chatTab_searchInput_point = ui->main_chatTab_searchInput->mapToGlobal(QPoint(0, 0));
				QRect main_chatTab_searchInput_rect(main_chatTab_searchInput_point, ui->main_chatTab_searchInput->size());
				if (main_chatTab_searchInput_rect.contains(pos))
				{
					ui->main_chatTab_searchInput->setFocus();
				}
				else
				{
					ui->main_chatTab_searchInput->clearFocus();
				}
				int nextIndex = ui->main_chatTab_chatStack->currentIndex();
				if (nextIndex == 1)
				{
					QPoint main_chatTab_chatStack_NotNULL_textInput_point = ui->main_chatTab_chatStack_NotNULL_textInput->mapToGlobal(QPoint(0, 0));
					QRect main_chatTab_chatStack_NotNULL_textInput_rect(main_chatTab_chatStack_NotNULL_textInput_point, ui->main_chatTab_chatStack_NotNULL_textInput->size());
					if (main_chatTab_chatStack_NotNULL_textInput_rect.contains(pos))
					{
						ui->main_chatTab_chatStack_NotNULL_textInput->setFocus();
					}
					else
					{
						ui->main_chatTab_chatStack_NotNULL_textInput->clearFocus();
					}
					QPoint emotionView_point = QQChatEmojiView::getInstance()->mapToGlobal(QPoint(0, 0));
					QRect emotionView_rect(emotionView_point, QQChatEmojiView::getInstance()->size());
					if (!emotionView_rect.contains(pos))
					{
						QQChatEmojiView::getInstance()->hide();
					}
				}
			}
			else if (index == 1)
			{
				QPoint main_friendAndGroupTab_searchInput_point = ui->main_friendAndGroupTab_searchInput->mapToGlobal(QPoint(0, 0));
				QRect main_friendAndGroupTab_searchInput_rect(main_friendAndGroupTab_searchInput_point, ui->main_friendAndGroupTab_searchInput->size());
				if (main_friendAndGroupTab_searchInput_rect.contains(pos))
				{
					ui->main_friendAndGroupTab_searchInput->setFocus();
				}
				else
				{
					ui->main_friendAndGroupTab_searchInput->clearFocus();
				}
				int nextIndex = ui->main_friendAndGroupTab_informationStack->currentIndex();
				if (nextIndex == 1)
				{
					QLineEdit *friendNikeNameInput = QQInformationListWidget::getInstance()->findChild<QLineEdit *>("friendNikeNameInput");
					QPoint friendNikeNameInput_point = friendNikeNameInput->mapToGlobal(QPoint(0, 0));
					QRect friendNikeNameInput_rect(friendNikeNameInput_point, friendNikeNameInput->size());
					if (friendNikeNameInput_rect.contains(pos))
					{
						friendNikeNameInput->setFocus();
						friendNikeNameInput->setAlignment(Qt::AlignLeft);
						friendNikeNameInput->setReadOnly(false);
					}
					else
					{
						friendNikeNameInput->clearFocus();
						friendNikeNameInput->setAlignment(Qt::AlignRight);
						friendNikeNameInput->setReadOnly(true);
					}
				}
			}
		}
	}
	return QWidget::eventFilter(watch, event);
}

// 退出系统,配置文件
void QQDesktop::closeEvent(QCloseEvent *event)
{
	exitSystem();
	QWidget::closeEvent(event);
}

// 向后台发送消息(聊天消息)--本机到后台
void QQDesktop::do_sendmsg(QStandardItem *item, QSharedPointer<ClientConfigs::MessageBaseConfig> &message)
{
	// 更新消息模型的状态信息
	item->setData(true, Qt::UserRole + 5);
	// 写入消息数据
	QJsonObject senderData;
	senderData.insert("version", "1.0");
	senderData.insert("sender", "QQDesktop");
	senderData.insert("action", "send-message");
	QJsonObject data;
	switch (message->m_messageType)
	{
	case ClientConfigs::MessageBaseConfig::MessageType::text:
	{
		data.insert("type", "text");
		break;
	}
	case ClientConfigs::MessageBaseConfig::MessageType::image:
	{
		data.insert("type", "image");
		break;
	}
	case ClientConfigs::MessageBaseConfig::MessageType::file:
	{
		data.insert("type", "file");
		break;
	}
	case ClientConfigs::MessageBaseConfig::MessageType::link:
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
	GlobalValuesAgent::m_localSocket.write(QJsonDocument(senderData).toJson());
	GlobalValuesAgent::m_localSocket.flush(); // 无阻塞写入数据
}

void QQDesktop::do_revmsg()
{
	// 转为message,调用do_appendMessage
}

void QQDesktop::do_peopleInModelItem(QStandardItem *item, ClientConfigs::UserFriend_C *user, ClientEnums::PeopleWidgetType type)
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

void QQDesktop::do_peopleInModelItem(QStandardItem *item, ClientConfigs::UserGroup_C *group, ClientEnums::PeopleWidgetType type)
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

void QQDesktop::do_messageInModelItem(QStandardItem *item, QSharedPointer<ClientConfigs::MessageBaseConfig> &message, bool isReceive)
{
	item->setData(QVariant::fromValue(message.data()), Qt::UserRole);
	switch (message->m_senderType)
	{
	case ClientConfigs::MessageBaseConfig::SenderType::me:
	case ClientConfigs::MessageBaseConfig::SenderType::she:
	{
		QString iconBase64 = QString::fromStdString(GlobalValuesAgent::m_user.m_icon);
		QPixmap icon = QPixmap::fromImage(getBase64ToImage(iconBase64));
		item->setData(icon, Qt::UserRole + 1);
		QString name = QString::fromLocal8Bit(GlobalValuesAgent::m_user.m_name.c_str());
		if (message->m_senderType == ClientConfigs::MessageBaseConfig::SenderType::she)
		{
			name = getUserInfo(QString::fromStdString(message->m_senderID))->m_nikeName;
		}
		item->setData(name, Qt::UserRole + 2);
		switch (message->m_messageType)
		{
		case ClientConfigs::MessageBaseConfig::MessageType::text:
		{
			item->setData(QString::fromLocal8Bit(message->m_content.c_str()), Qt::UserRole + 3);
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::image:
		{
			QString imageBase64 = QString::fromStdString(message->m_content);
			QPixmap image = QPixmap::fromImage(getBase64ToImage(imageBase64));
			item->setData(image, Qt::UserRole + 4);
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::file:
		{
			QString fileName = QString::fromLocal8Bit(message->m_content.c_str());
			QPair<qreal, QString> pair = getFileFormatSize(QFile(fileName).size());
			QString fileSize = QString::number(pair.first, 'f', 2) + " " + pair.second;
			item->setData(fileSize, Qt::UserRole + 3);
			QPixmap fileIcon = QFileIconProvider().icon(QFileInfo(fileName)).pixmap(50, 50);
			item->setData(fileIcon, Qt::UserRole + 4);
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::link:
		{
			QString link = QString::fromLocal8Bit(message->m_content.c_str());
			item->setData(link, Qt::UserRole + 3);
			break;
		}
		case ClientConfigs::MessageBaseConfig::MessageType::emoji:
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
	case ClientConfigs::MessageBaseConfig::SenderType::system:
	{
		QString system = QString::fromLocal8Bit(message->m_content.c_str());
		item->setData(system, Qt::UserRole + 3);
		break;
	}
	case ClientConfigs::MessageBaseConfig::SenderType::time:
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
	case ClientConfigs::MessageBaseConfig::SenderType::me:
	{
		item->setData(false, Qt::UserRole + 5);
		if (message->m_messageType == ClientConfigs::MessageBaseConfig::MessageType::file)
		{
			item->setData(true, Qt::UserRole + 6);
		}
		break;
	}
	case ClientConfigs::MessageBaseConfig::SenderType::she:
	{
		item->setData(true, Qt::UserRole + 5);
		if (message->m_messageType == ClientConfigs::MessageBaseConfig::MessageType::file)
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
	case ClientConfigs::MessageBaseConfig::SenderType::system:
	case ClientConfigs::MessageBaseConfig::SenderType::time:
	{
		item->setData(true, Qt::UserRole + 5);
		break;
	}
	}
}

void QQDesktop::do_sendFileByName(QString &fileName)
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
	QModelIndex index = ui->main_chatTab_listStack_NotNULL_listView->currentIndex();
	ClientConfigs::UserFriend_C *user = const_cast<ClientConfigs::UserFriend_C *>(qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
	ClientConfigs::UserGroup_C *group = const_cast<ClientConfigs::UserGroup_C *>(qvariant_cast<ClientConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
	QSharedPointer<ClientConfigs::MessageBaseConfig> message(new ClientConfigs::MessageBaseConfig);
	message->m_messageType = ClientConfigs::MessageBaseConfig::MessageType::file;
	message->m_senderType = ClientConfigs::MessageBaseConfig::SenderType::me;
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
	do_appendMessage(message);
}

void QQDesktop::do_appendMessage(QSharedPointer<ClientConfigs::MessageBaseConfig> &message, bool isUpdateData, int index, bool isReceive)
{
	QStandardItem *item = new QStandardItem;
	QListView *listView = ui->main_chatTab_chatStack_NotNULL_msgListView;
	do_messageInModelItem(item, message, isReceive);
	if (isUpdateData)
	{
		m_messageListModel->insertRow(m_messageListModel->rowCount() - 1, item);
		listView->scrollToBottom();
		QModelIndex index = ui->main_chatTab_listStack_NotNULL_listView->currentIndex();
		ClientConfigs::UserFriend_C *user = const_cast<ClientConfigs::UserFriend_C *>(qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
		ClientConfigs::UserGroup_C *group = const_cast<ClientConfigs::UserGroup_C *>(qvariant_cast<ClientConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
		if (user != nullptr)
		{
			user->m_msgList.append(message);
		}
		else if (group != nullptr)
		{
			group->m_msgList.append(message);
		}
		// 如果是自己发送的消息,则发送给服务器--其余消息均重文件中加载(群聊与好友消息来源于服务器,下载到本地,系统消息与时间消息是由本地生成,存储于本地,不需要发送)
		if (message->m_senderType == ClientConfigs::MessageBaseConfig::SenderType::me)
		{
			do_sendmsg(item, message);
		}
	}
	else
	{
		m_messageListModel->setItem(index, item);
	}
}

void QQDesktop::do_loadMessages(ClientConfigs::UserFriend_C *user)
{
	// 预分配空间
	m_messageListModel->insertRows(0, user->m_msgList.count());
	for (int i = 0; i < user->m_msgList.count(); ++i)
	{
		int index = user->m_msgList.count() - i - 1;
		QSharedPointer<ClientConfigs::MessageBaseConfig> message = user->m_msgList.at(index);
		do_appendMessage(message, false, index);
	}
}

void QQDesktop::do_loadMessages(ClientConfigs::UserGroup_C *group)
{
	// 预分配空间
	m_messageListModel->insertRows(0, group->m_msgList.count());
	for (int i = 0; i < group->m_msgList.count(); ++i)
	{
		int index = group->m_msgList.count() - i - 1;
		QSharedPointer<ClientConfigs::MessageBaseConfig> message = group->m_msgList.at(index);
		do_appendMessage(message, false, index);
	}
}

void QQDesktop::do_userClickSearchMoreButton()
{
	QPoint pos = qobject_cast<QPushButton *>(sender())->mapToGlobal(QPoint(0, 0));
	QPoint searchMoreMenuPos = pos + QPoint(5, qobject_cast<QPushButton *>(sender())->height());
	m_searchMoreMenu->popup(searchMoreMenuPos);
}

void QQDesktop::do_userClickSearchMoreButtonAction(QAction *action)
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

void QQDesktop::do_chat_userClickRightContextMenuAction(QAction *action)
{
	QPoint pos = ui->main_chatTab_listStack_NotNULL_listView->getMCurrentPoint();
	QModelIndex index = ui->main_chatTab_listStack_NotNULL_listView->indexAt(pos);
	if (action == m_chatFriendContextMenu->actions()[0])
	{
		m_chatListModel->insertRow(0, m_chatListModel->takeRow(index.row())[0]);
	}
	else if (action == m_chatFriendContextMenu->actions()[1])
	{
		QString text = action->text();
		ClientConfigs::UserFriend_C *user = const_cast<ClientConfigs::UserFriend_C *>(qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
		ClientConfigs::UserGroup_C *group = const_cast<ClientConfigs::UserGroup_C *>(qvariant_cast<ClientConfigs::UserGroup_C *>(index.data(Qt::UserRole)));

		if (text == QString::fromLocal8Bit("消息免打扰"))
		{
			action->setText(QString::fromLocal8Bit("取消免打扰"));
			if (user != nullptr)
			{
				user->m_state = ClientConfigs::UserFriend_C::UserStateType::distrup;
			}
			else if (group != nullptr)
			{
				group->m_state = ClientConfigs::UserGroup_C::GroupStateType::disnotice;
			}
		}
		else if (text == QString::fromLocal8Bit("取消免打扰"))
		{
			action->setText(QString::fromLocal8Bit("消息免打扰"));
			if (user != nullptr)
			{
				user->m_state = ClientConfigs::UserFriend_C::UserStateType::online;
			}
			else if (group != nullptr)
			{
				group->m_state = ClientConfigs::UserGroup_C::GroupStateType::normal;
			}
		}
	}
	else if (action == m_chatFriendContextMenu->actions()[2])
	{
		m_haveInChatList.remove(qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole))->m_ID);
		// 清除默认的移除选中项,选中项效果下移
		if (ui->main_chatTab_listStack_NotNULL_listView->selectionModel()->isSelected(index))
		{
			ui->main_chatTab_listStack_NotNULL_listView->selectionModel()->clear();
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
			QModelIndex index = ui->main_chatTab_listStack_NotNULL_listView->currentIndex();
			ClientConfigs::UserFriend_C *user = const_cast<ClientConfigs::UserFriend_C *>(qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
			ClientConfigs::UserGroup_C *group = const_cast<ClientConfigs::UserGroup_C *>(qvariant_cast<ClientConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
			QString report = QQMessageDialog::getInstance()->findChild<QTextEdit *>()->toPlainText();
			// 发送举报消息
			QJsonObject reportData;
			reportData.insert("version", "1.0");
			reportData.insert("sender", "QQDesktop");
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

void QQDesktop::do_chat_userClickChatIndex(const QModelIndex &index)
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
	ClientConfigs::UserFriend_C *user = const_cast<ClientConfigs::UserFriend_C *>(qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
	ClientConfigs::UserGroup_C *group = const_cast<ClientConfigs::UserGroup_C *>(qvariant_cast<ClientConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
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
	int width = ui->main_chatTab_chatStack_NotNULL_msgListView->width();
	item->setSizeHint(QSize(width, 25));
	m_messageListModel->appendRow(item);
	m_haveInChatList.insert(user->m_ID, m_chatListModel->itemFromIndex(index));
	QTimer::singleShot(50, [this]
					   { ui->main_chatTab_chatStack_NotNULL_msgListView->scrollToBottom(); });
	m_activeChatIndex = index;
}

void QQDesktop::do_chat_userClickSendEmojiButton()
{
	QPoint pos = ui->main_chatTab_chatStack_NotNULL_inputToolGroup_emojiBtn->mapToGlobal(QPoint(0, 0));
	pos.setY(pos.y() - QQChatEmojiView::getInstance()->height() - 10);
	QQChatEmojiView::getInstance()->setGeometry(QRect(pos, QQChatEmojiView::getInstance()->size()));
	QQChatEmojiView::getInstance()->show();
}

void QQDesktop::do_chat_userClickSendEmojiButtonFile(const QString &emoji, ClientEnums::EmojiType type)
{
	switch (type)
	{
	case ClientEnums::EmojiType::unicode:
	{
		QModelIndex index = ui->main_chatTab_listStack_NotNULL_listView->currentIndex();
		ClientConfigs::UserFriend_C *user = const_cast<ClientConfigs::UserFriend_C *>(qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
		ClientConfigs::UserGroup_C *group = const_cast<ClientConfigs::UserGroup_C *>(qvariant_cast<ClientConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
		QSharedPointer<ClientConfigs::MessageBaseConfig> message(new ClientConfigs::MessageBaseConfig);
		message->m_messageType = ClientConfigs::MessageBaseConfig::MessageType::emoji;
		message->m_senderType = ClientConfigs::MessageBaseConfig::SenderType::me;
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

void QQDesktop::do_chat_userClickSendFileButton()
{
	QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择文件"), "C:/", QString::fromLocal8Bit("所有文件(*.*)"));
	do_sendFileByName(fileName);
}

void QQDesktop::do_chat_userClickSendPictureButton()
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

	QModelIndex index = ui->main_chatTab_listStack_NotNULL_listView->currentIndex();
	ClientConfigs::UserFriend_C *user = const_cast<ClientConfigs::UserFriend_C *>(qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
	ClientConfigs::UserGroup_C *group = const_cast<ClientConfigs::UserGroup_C *>(qvariant_cast<ClientConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
	QPair<QString, QString> byteArray = getImageToBase64(fileName);
	QSharedPointer<ClientConfigs::MessageBaseConfig> message(new ClientConfigs::MessageBaseConfig);
	message->m_messageType = ClientConfigs::MessageBaseConfig::MessageType::image;
	message->m_senderType = ClientConfigs::MessageBaseConfig::SenderType::me;
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

void QQDesktop::do_chat_userOpenBigWriteButton(bool isChecked)
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

void QQDesktop::do_chat_userClickSetFontButton()
{
}

void QQDesktop::do_chat_userClickLookHistroyButton()
{
}

void QQDesktop::do_chat_limitUserInputTextCount()
{
	// 统计输入的文本数量
	int count = 0;
	QTextDocument *textDocument = ui->main_chatTab_chatStack_NotNULL_textInput->document();
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
		int position = ui->main_chatTab_chatStack_NotNULL_textInput->textCursor().position();
		QTextCursor textCursor = ui->main_chatTab_chatStack_NotNULL_textInput->textCursor();
		textCursor.movePosition(QTextCursor::End);
		if (textCursor.hasSelection())
		{
			textCursor.clearSelection();
		}
		textCursor.deletePreviousChar();
		ui->main_chatTab_chatStack_NotNULL_textInput->setTextCursor(textCursor);
	}
}

void QQDesktop::do_chat_userClickSendAudioButton()
{
}

void QQDesktop::do_chat_userClickClearInputButton()
{
	// 清除输入框的内容,并设置焦点
	ui->main_chatTab_chatStack_NotNULL_textInput->clear();
	ui->main_chatTab_chatStack_NotNULL_textInput->setFocus();
}

void QQDesktop::do_chat_userClickSendMessageButton()
{
	if (ui->main_chatTab_chatStack_NotNULL_textInput->toPlainText() == "")
	{
		ElaMessageBar::warning(ElaMessageBarType::PositionPolicy::Bottom, "", QString::fromLocal8Bit("发送内容不能为空"), 3000, this);
		return;
	}
	QModelIndex index = ui->main_chatTab_listStack_NotNULL_listView->currentIndex();
	ClientConfigs::UserFriend_C *user = const_cast<ClientConfigs::UserFriend_C *>(qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
	ClientConfigs::UserGroup_C *group = const_cast<ClientConfigs::UserGroup_C *>(qvariant_cast<ClientConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
	QSharedPointer<ClientConfigs::MessageBaseConfig> message(new ClientConfigs::MessageBaseConfig);
	message->m_messageType = ClientConfigs::MessageBaseConfig::MessageType::text;
	message->m_senderType = ClientConfigs::MessageBaseConfig::SenderType::me;
	message->m_senderID = GlobalValuesAgent::m_user.m_ID;
	if (user != nullptr)
	{
		message->m_receiverID = user->m_ID;
	}
	else if (group != nullptr)
	{
		message->m_receiverID = group->m_ID;
	}
	message->m_content = ui->main_chatTab_chatStack_NotNULL_textInput->toPlainText().toLocal8Bit();
	message->m_time = QDateTime::currentDateTime().toSecsSinceEpoch();
	do_appendMessage(message);
	ui->main_chatTab_chatStack_NotNULL_textInput->clear();
}

void QQDesktop::do_friend_userClickRightContextMenu(const QPoint &pos)
{
	QModelIndex index = ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView->indexAt(pos);
	// 如果点击的是顶部的分组,则不弹出菜单
	if (!index.parent().isValid() && index != m_friendsListModel->index(0, 0))
	{
		m_friendContextMenu->exec(QCursor::pos());
	}
}

void QQDesktop::do_friend_userClickRightContextMenuAction(QAction *action)
{
	// 主要是对分组的操作
	if (action == m_friendContextMenu->actions()[0])
	{
		QQMessageDialog *messageDialog = QQMessageDialog::getInstance();
		messageDialog->removeAllMWidget();
		QPoint pos = this->mapToGlobal(QPoint(0, 0)) + QPoint(this->width() / 2 - 200, this->height() / 2 - 75);
		messageDialog->setGeometry(QRect(pos, QSize(400, 150)));
		messageDialog->setMTittle(QString::fromLocal8Bit("新建分组"));
		messageDialog->closeOkButton();
		QLineEdit *lineEdit = new QLineEdit(messageDialog);
		lineEdit->setFixedHeight(45);
		lineEdit->setFont(QFont("Microsoft YaHei", 10));
		lineEdit->setStyleSheet("QLineEdit{border:1px solid #C0C0C0;border-radius:5px;}");
		lineEdit->setPlaceholderText(QString::fromLocal8Bit("请输入分组名称"));
		connect(lineEdit, &QLineEdit::textChanged, [lineEdit, messageDialog]()
				{
				if (lineEdit->text().isEmpty())
				{
					messageDialog->closeOkButton();
				}
				else
				{
					messageDialog->openOkButton();
				} });
		messageDialog->addMWidget(lineEdit);
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
				m_friendsGroupComboBox->addItem(text);
			}
			else
			{
				messageDialog->setMAlertLabel(QString::fromLocal8Bit("分组已存在"));
			}
		};
		messageDialog->setMFunction(function);
		messageDialog->exec();
	}
	else if (action == m_friendContextMenu->actions()[1])
	{
		QQMessageDialog *messageDialog = QQMessageDialog::getInstance();
		messageDialog->removeAllMWidget();
		QPoint pos = this->mapToGlobal(QPoint(0, 0)) + QPoint(this->width() / 2 - 200, this->height() / 2 - 50);
		messageDialog->setGeometry(QRect(pos, QSize(400, 100)));
		messageDialog->setMTittle(QString::fromLocal8Bit("删除该分组"));
		messageDialog->openOkButton();
		QLabel *label = new QLabel(messageDialog);
		label->setText(QString::fromLocal8Bit("您确定要删除该分组?"));
		label->setFixedHeight(40);
		label->setAlignment(Qt::AlignCenter);
		label->setFont(QFont("Microsoft YaHei", 11));
		messageDialog->addMWidget(label);
		messageDialog->layout()->setSpacing(10);
		messageDialog->layout()->setContentsMargins(20, 5, 20, 10);
		std::function<void()> function = [this]
		{
			QModelIndex topIndex = m_friendsListModel->index(0, 0);
			QStandardItem *topItem = m_friendsListModel->itemFromIndex(topIndex);
			QModelIndex index = ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView->currentIndex();
			QStandardItem *item = m_friendsListModel->itemFromIndex(index);
			while (item->rowCount() != 0)
			{
				QStandardItem *childItem = item->child(0, 0);
				item->takeRow(0);
				topItem->appendRow(childItem);
			}
			m_friendsGroups.remove(index.data(Qt::DisplayRole).toString());
			m_friendsGroupComboBox->removeItem(index.row());
			m_friendsListModel->removeRow(index.row());
		};
		messageDialog->setMFunction(function);
		messageDialog->exec();
	}
	else if (action == m_friendContextMenu->actions()[2])
	{
		QQMessageDialog *messageDialog = QQMessageDialog::getInstance();
		messageDialog->removeAllMWidget();
		QPoint pos = this->mapToGlobal(QPoint(0, 0)) + QPoint(this->width() / 2 - 200, this->height() / 2 - 75);
		messageDialog->setGeometry(QRect(pos, QSize(400, 150)));
		messageDialog->setMTittle(QString::fromLocal8Bit("重命名该分组"));
		messageDialog->openOkButton();
		QLineEdit *lineEdit = new QLineEdit(messageDialog);
		QModelIndex index = ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView->currentIndex();
		lineEdit->setText(index.data(Qt::DisplayRole).toString());
		lineEdit->setFixedHeight(45);
		lineEdit->setFont(QFont("Microsoft YaHei", 10));
		lineEdit->setStyleSheet("QLineEdit{border:1px solid #C0C0C0;border-radius:5px;}");
		lineEdit->setPlaceholderText(QString::fromLocal8Bit("请输入分组名称"));
		connect(lineEdit, &QLineEdit::textChanged, [lineEdit, messageDialog]()
				{
				if (lineEdit->text().isEmpty())
				{
					messageDialog->closeOkButton();
				}
				else
				{
					messageDialog->openOkButton();
				} });
		messageDialog->addMWidget(lineEdit);
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
				messageDialog->setMAlertLabel(QString::fromLocal8Bit("分组已存在"));
			}
		};
		messageDialog->setMFunction(function);
		messageDialog->exec();
	}
}

void QQDesktop::do_friend_userClickNoticeButton()
{
	ui->main_friendAndGroupTab_informationStack->setCurrentIndex(3);
	if (sender() == ui->main_friendAndGroupTab_friendNoticeButton)
	{
		ui->main_friendAndGroupTab_informationStack_NotNULL_noNoticeWidge_nameLab->setText(QString::fromLocal8Bit("好友通知"));
	}
	else if (sender() == ui->main_friendAndGroupTab_groupNoticeButton)
	{
		ui->main_friendAndGroupTab_informationStack_NotNULL_noNoticeWidge_nameLab->setText(QString::fromLocal8Bit("群通知"));
	}
}

void QQDesktop::do_friend_userClickOption(int index)
{
	ui->main_friendAndGroupTab_listStack->setCurrentIndex(index);
}

void QQDesktop::do_friend_userClickFriendListIndex(const QModelIndex &index)
{
	if (!index.parent().isValid())
	{
		// 如果点击的是顶部的分组,则展开或者收缩,并且改变图标
		QStandardItem *item = m_friendsListModel->itemFromIndex(index);
		bool isDecorated = ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView->isExpanded(index);
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
		ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView->setExpanded(index, !isDecorated);
	}
	else
	{
		// 如果点击的是好友,则展示好友的信息
		// 更新基础信息
		ui->main_friendAndGroupTab_informationStack->setCurrentIndex(1);
		ClientConfigs::UserFriend_C *user = qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole));
		QString iconBase64String = QString::fromLocal8Bit(user->m_icon.c_str());
		QPixmap icon = QPixmap::fromImage(getBase64ToImage(iconBase64String));
		ui->main_friendAndGroupTab_informationStack_NotNULL_friendIconLab->setPixmap(icon);
		ui->main_friendAndGroupTab_informationStack_NotNULL_friendNameLab->setText(QString::fromLocal8Bit(user->m_name.c_str()));
		QString account = QString("QQ ") + QString::fromLocal8Bit(user->m_account.c_str());
		ui->main_friendAndGroupTab_informationStack_NotNULL_friendCountLab->setText(account);
		// 更新可变框的信息
		if (!user->m_nikeName.isEmpty())
		{
			QLineEdit *friendNikeNameInput = qobject_cast<QLineEdit *>(QQInformationListWidget::getInstance()->getMInformation(0, ClientEnums::InformationWidgetType::widget));
			friendNikeNameInput->setText(user->m_nikeName);
		}
		if (!user->m_group.isEmpty())
		{
			m_friendsGroupComboBox->setCurrentText(user->m_group);
		}
		if (!user->m_sign.empty())
		{
			QLabel *signLabel = qobject_cast<QLabel *>(QQInformationListWidget::getInstance()->getMInformation(2, ClientEnums::InformationWidgetType::widget));
			if (signLabel != nullptr)
			{
				QFontMetrics fm(QQInformationListWidget::getInstance()->font());
				int width = QQInformationListWidget::getInstance()->width() - 200;
				QString elidedText = fm.elidedText(QString::fromLocal8Bit(user->m_sign.c_str()), Qt::ElideRight, width);
				signLabel->setText(elidedText);
				signLabel->setToolTip(QString::fromLocal8Bit(user->m_sign.c_str()));
			}
			else
			{
				signLabel = new QLabel(QQInformationListWidget::getInstance());
				QFontMetrics fm(QQInformationListWidget::getInstance()->font());
				int width = QQInformationListWidget::getInstance()->width() - 200;
				QString elidedText = fm.elidedText(QString::fromLocal8Bit(user->m_sign.c_str()), Qt::ElideRight, width);
				signLabel->setText(elidedText);
				signLabel->setToolTip(QString::fromLocal8Bit(user->m_sign.c_str()));
				QQInformationListWidget::getInstance()->addMInformation(QString::fromLocal8Bit("个性签名"), signLabel, QIcon(":/widget/main/friendAndGroup/images/widget/main/friendAndGroup/rename_no.png"));
			}
		}
	}
}

void QQDesktop::do_friend_userClickGroupListIndex(const QModelIndex &index)
{
	if (!index.parent().isValid())
	{
		// 如果点击的是顶部的分组,则展开或者收缩,并且改变图标
		QStandardItem *item = m_groupsListModel->itemFromIndex(index);
		bool isDecorated = ui->main_friendAndGroupTab_listStack_NotNULL_groupsTab_treeView->isExpanded(index);
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
		ui->main_friendAndGroupTab_listStack_NotNULL_groupsTab_treeView->setExpanded(index, !isDecorated);
	}
	else
	{
		// 如果点击的是群聊,则展示群聊的信息
		// 更新基础信息
		ui->main_friendAndGroupTab_informationStack->setCurrentIndex(2);
		QModelIndex index = ui->main_friendAndGroupTab_listStack_NotNULL_groupsTab_treeView->currentIndex();
		ClientConfigs::UserGroup_C *group = qvariant_cast<ClientConfigs::UserGroup_C *>(index.data(Qt::UserRole));
		QString iconBase64String = QString::fromLocal8Bit(group->m_icon.c_str());
		QPixmap icon = QPixmap::fromImage(getBase64ToImage(iconBase64String));
		ui->main_friendAndGroupTab_informationStack_NotNULL_groupIconLab->setPixmap(icon);
		ui->main_friendAndGroupTab_informationStack_NotNULL_groupNameLab->setText(QString::fromLocal8Bit(group->m_name.c_str()));
		QString account = QString::fromLocal8Bit("群聊 ") + QString::fromLocal8Bit(group->m_account.c_str());
		ui->main_friendAndGroupTab_informationStack_NotNULL_friendCountLab->setText(account);
	}
}

void QQDesktop::do_friend_userSelectedGroupChanged(const QString &text)
{
	// 如果分组改变,则将选中的好友移动到新的分组
	QModelIndex index = ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView->currentIndex();
	ClientConfigs::UserFriend_C *user = qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole));
	QModelIndex previousIndex = m_friendsGroups[user->m_group];
	QModelIndex currentIndex = m_friendsGroups[text];
	QStandardItem *item = m_friendsListModel->itemFromIndex(index);
	m_friendsListModel->itemFromIndex(previousIndex)->takeRow(item->row());
	m_friendsListModel->itemFromIndex(currentIndex)->appendRow(item);
	user->m_group = text;
}

void QQDesktop::do_friend_userClickShareOtherButton()
{
}

void QQDesktop::do_friend_userClickMusicConnectButton()
{
}

void QQDesktop::do_friend_userClickSendMessageButton()
{
	// 如果点击的是发送消息,则将选中的好友或者群聊添加到聊天列表
	if (sender() == ui->main_friendAndGroupTab_informationStack_NotNULL_friendSendMsgBtn)
	{
		QModelIndex index = ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView->currentIndex();
		ClientConfigs::UserFriend_C *user = qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole));
		// 如果已经在聊天列表中,则直接切换到该聊天
		if (m_haveInChatList.contains(user->m_ID))
		{
			ui->main_chatTab_listStack_NotNULL_listView->setCurrentIndex(m_chatListModel->indexFromItem(m_haveInChatList[user->m_ID]));
			do_chat_userClickChatIndex(m_chatListModel->indexFromItem(m_haveInChatList[user->m_ID]));
		}
		else
		{
			QStandardItem *item = new QStandardItem;
			do_peopleInModelItem(item, user);
			m_chatListModel->insertRow(0, item);
			do_chat_userClickChatIndex(m_chatListModel->indexFromItem(item));
			ui->main_chatTab_listStack_NotNULL_listView->setCurrentIndex(m_chatListModel->indexFromItem(item));
			m_haveInChatList.insert(user->m_ID, item);
		}
	}
	else if (sender() == ui->main_friendAndGroupTab_informationStack_NotNULL_groupSendMsgBtn)
	{
		QModelIndex index = ui->main_friendAndGroupTab_listStack_NotNULL_friendsTab_treeView->currentIndex();
		ClientConfigs::UserGroup_C *group = qvariant_cast<ClientConfigs::UserGroup_C *>(index.data(Qt::UserRole));
		// 如果已经在聊天列表中,则直接切换到该聊天
		if (m_haveInChatList.contains(group->m_ID))
		{
			ui->main_chatTab_listStack_NotNULL_listView->setCurrentIndex(m_chatListModel->indexFromItem(m_haveInChatList[group->m_ID]));
			do_chat_userClickChatIndex(m_chatListModel->indexFromItem(m_haveInChatList[group->m_ID]));
		}
		else
		{
			QStandardItem *item = new QStandardItem;
			do_peopleInModelItem(item, group);
			m_chatListModel->insertRow(0, item);
			do_chat_userClickChatIndex(m_chatListModel->indexFromItem(item));
			ui->main_chatTab_listStack_NotNULL_listView->setCurrentIndex(m_chatListModel->indexFromItem(item));
			m_haveInChatList.insert(group->m_ID, item);
		}
	}
	// 切换到聊天界面,并且切换到聊天列表
	ui->option_mainGroup_chatBtn->setChecked(true);
	ui->mainWidget->setCurrentIndex(0);
	ui->main_chatTab_listStack->setCurrentIndex(1);
}
