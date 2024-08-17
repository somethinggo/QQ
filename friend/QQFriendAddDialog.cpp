#include "QQFriendAddDialog.h"

void QQFriendAddDialogProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == PE_Frame)
	{
		return;
	}
	return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void QQFriendAddDialogProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == CE_PushButton)
	{
		QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
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
	return QProxyStyle::drawControl(element, option, painter, widget);
}

QQFriendAddDialog::QQFriendAddDialog(QWidget *parent)
	: QDialog(parent)
{
	this->setFixedSize(1000, 700);
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, Qt::white);
	this->setPalette(palette);
	this->setAutoFillBackground(true);
	m_proxyStyle = new QQFriendAddDialogProxyStyle;
	m_usersModel = new QStandardItemModel(this);
	m_groupsModel = new QStandardItemModel(this);

	m_appBar = new QQWidgets::AppBar(this, QQEnums::AppBarHintType::WindowsHint);
	m_appBar->setWindowsTitle(QString::fromLocal8Bit("全网搜索"));

	m_searchEdit = new ElaLineEdit(this);
	m_searchEdit->setFont(QQGlobals::g_theme->m_friend_add_input_font);
	m_searchEdit->setFixedHeight(40);
	m_searchEdit->setPlaceholderText(QString::fromLocal8Bit("输入搜索关键词"));
	m_searchEdit->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);

	m_tabBar = new QQWidgets::AnimationTabBar(this);
	m_tabBar->setFont(QQGlobals::g_theme->m_friend_add_tabbar_font);
	m_tabBar->setFixedHeight(40);
	m_tabBar->addTab(QString::fromLocal8Bit("用户"));
	m_tabBar->addTab(QString::fromLocal8Bit("群聊"));
	m_tabBar->setMargin(5);
	m_tabBar->setBackgroundColor(QQGlobals::g_theme->m_friend_add_tabbar_frame_color);
	m_tabBar->setHoverColor(QQGlobals::g_theme->m_friend_add_tabbar_hover_color);
	m_tabBar->setSelectColor(QQGlobals::g_theme->m_friend_add_tabbar_selected_color);
	m_tabBar->setSelectTextColor(QQGlobals::g_theme->m_friend_add_tabbar_selected_text_color);

	// 添加搜索结果列表
	m_stackedWidget = new QStackedWidget(this);
	m_stackedWidget->addWidget(new QQWidgets::PixelListView(m_stackedWidget));
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setModel(m_usersModel);
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setHorizontalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setFocusPolicy(Qt::NoFocus);
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setStyle(m_proxyStyle);
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setAutoFillBackground(true);
	m_stackedWidget->addWidget(new QQWidgets::PixelListView(m_stackedWidget));
	qobject_cast<QListView *>(m_stackedWidget->widget(1))->setModel(m_groupsModel);
	qobject_cast<QListView *>(m_stackedWidget->widget(1))->setHorizontalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
	qobject_cast<QListView *>(m_stackedWidget->widget(1))->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	qobject_cast<QListView *>(m_stackedWidget->widget(1))->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	qobject_cast<QListView *>(m_stackedWidget->widget(1))->setFocusPolicy(Qt::NoFocus);
	qobject_cast<QListView *>(m_stackedWidget->widget(1))->setStyle(m_proxyStyle);
	qobject_cast<QListView *>(m_stackedWidget->widget(1))->setAutoFillBackground(true);

	QLabel *searchLabelIcon = new QLabel(m_stackedWidget);
	searchLabelIcon->setPixmap(ElaIcon::getInstance()->getElaIcon(ElaIconType::Inbox, 100, 100, 100).pixmap(100, 100));
	searchLabelIcon->setAlignment(Qt::AlignCenter);
	QLabel *searchLabelText = new QLabel(m_stackedWidget);
	searchLabelText->setAlignment(Qt::AlignCenter);
	searchLabelText->setText(QString::fromLocal8Bit("输入关键词搜索"));
	searchLabelText->setFont(QFont("Microsoft YaHei", 10));
	QVBoxLayout *searchLabelLayout = new QVBoxLayout;
	searchLabelLayout->setContentsMargins(0, 0, 0, 0);
	searchLabelLayout->setSpacing(40);
	searchLabelLayout->addStretch();
	searchLabelLayout->addWidget(searchLabelIcon);
	searchLabelLayout->addWidget(searchLabelText);
	searchLabelLayout->addStretch();
	QWidget *searchIsNullWidget = new QWidget(m_stackedWidget);
	searchIsNullWidget->setLayout(searchLabelLayout);
	m_stackedWidget->addWidget(searchIsNullWidget);
	m_stackedWidget->setCurrentIndex(2);
	QLayout *searchLayout = new QVBoxLayout;
	searchLayout->setContentsMargins(0, 0, 0, 0);
	searchLayout->setSpacing(10);
	searchLayout->addWidget(m_searchEdit);
	searchLayout->addWidget(m_tabBar);
	searchLayout->addWidget(m_stackedWidget);

	m_informationWidget = new QStackedWidget(this);
	m_informationWidget->setFixedWidth(300);
	QLabel *informationLabel = new QLabel(m_informationWidget);
	QPalette informationLabelPalette = informationLabel->palette();
	informationLabelPalette.setColor(QPalette::WindowText, QColor("#999999"));
	informationLabel->setPalette(informationLabelPalette);
	informationLabel->setAutoFillBackground(true);
	informationLabel->setFont(QFont("Microsoft YaHei", 9));
	informationLabel->setAlignment(Qt::AlignCenter);
	informationLabel->setText(QString::fromLocal8Bit("请点击您要添加的好友或是群聊"));
	m_informationWidget->addWidget(informationLabel);

	m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	m_buttonBox->button(QDialogButtonBox::Ok)->setText(QString::fromLocal8Bit("添加"));
	m_buttonBox->button(QDialogButtonBox::Cancel)->setText(QString::fromLocal8Bit("取消"));
	m_buttonBox->button(QDialogButtonBox::Ok)->setFlat(true);
	m_buttonBox->button(QDialogButtonBox::Ok)->setStyle(m_proxyStyle);
	m_buttonBox->button(QDialogButtonBox::Ok)->setFixedSize(QQGlobals::g_theme->m_friend_add_validate_button_size);
	m_buttonBox->button(QDialogButtonBox::Ok)->setFont(QQGlobals::g_theme->m_friend_add_validate_button_font);
	m_buttonBox->button(QDialogButtonBox::Ok)->setMask(QQFunctions::getRoundedMask(m_buttonBox->button(QDialogButtonBox::Ok)->size(), 5));
	m_buttonBox->button(QDialogButtonBox::Cancel)->setFlat(true);
	m_buttonBox->button(QDialogButtonBox::Cancel)->setStyle(m_proxyStyle);
	m_buttonBox->button(QDialogButtonBox::Cancel)->setFixedSize(QQGlobals::g_theme->m_friend_add_validate_button_size);
	m_buttonBox->button(QDialogButtonBox::Cancel)->setFont(QQGlobals::g_theme->m_friend_add_validate_button_font);
	m_buttonBox->button(QDialogButtonBox::Cancel)->setMask(QQFunctions::getRoundedMask(m_buttonBox->button(QDialogButtonBox::Ok)->size(), 5));
	QVBoxLayout *validateLayout = new QVBoxLayout;
	validateLayout->setContentsMargins(0, 0, 0, 0);
	validateLayout->setSpacing(0);
	validateLayout->addWidget(QQFriendValidateInfo::getInstance());
	validateLayout->addWidget(m_buttonBox);
	QWidget *validateWidget = new QWidget(m_informationWidget);
	validateWidget->setLayout(validateLayout);
	m_informationWidget->addWidget(validateWidget);

	QHBoxLayout *tempLayout = new QHBoxLayout;
	tempLayout->setContentsMargins(20, 10, 20, 10);
	tempLayout->setSpacing(0);
	tempLayout->addLayout(searchLayout);
	tempLayout->addWidget(m_informationWidget);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(m_appBar);
	layout->addLayout(tempLayout);
	this->setLayout(layout);
	connect(m_searchEdit, &QLineEdit::textChanged, this, &QQFriendAddDialog::do_searchTextChanged);
	connect(m_tabBar, &QTabBar::currentChanged, this, &QQFriendAddDialog::do_tabBarCurrentChanged);
	connect(qobject_cast<QListView *>(m_stackedWidget->widget(0)), &QListView::clicked, this, &QQFriendAddDialog::do_userClickFriendItem);
	connect(qobject_cast<QListView *>(m_stackedWidget->widget(1)), &QListView::clicked, this, &QQFriendAddDialog::do_userClickGroupItem);
}

QQFriendAddDialog::~QQFriendAddDialog()
{
	delete m_proxyStyle;
}

void QQFriendAddDialog::appendUser(QQConfigs::FriendConfig *user)
{
	QStandardItem *item = new QStandardItem;
	item->setData(QVariant::fromValue(user), Qt::UserRole);
	QPixmap icon = QPixmap::fromImage(QQFunctions::getBase64ToImage(QString::fromStdString(user->m_icon)));
	icon = QQFunctions::getRoundedPixmap(icon.scaled(QQGlobals::g_theme->m_friend_index_icon_size, Qt::KeepAspectRatio), QQGlobals::g_theme->m_friend_index_icon_size.width() / 2);
	item->setData(icon, Qt::UserRole + 1);
	QString account = QString::fromStdString(user->m_account.c_str());
	item->setData(account, Qt::UserRole + 2);
	m_usersModel->appendRow(item);
}

void QQFriendAddDialog::appendGroup(QQConfigs::GroupConfig *group)
{
	QStandardItem *item = new QStandardItem;
	item->setData(QVariant::fromValue(group), Qt::UserRole);
	QPixmap icon = QPixmap::fromImage(QQFunctions::getBase64ToImage(QString::fromStdString(group->m_icon)));
	icon = QQFunctions::getRoundedPixmap(icon.scaled(QQGlobals::g_theme->m_friend_index_icon_size, Qt::KeepAspectRatio), QQGlobals::g_theme->m_friend_index_icon_size.width() / 2);
	item->setData(icon, Qt::UserRole + 1);
	QString account = QString::fromStdString(group->m_account.c_str());
	item->setData(account, Qt::UserRole + 2);
	m_groupsModel->appendRow(item);
}

void QQFriendAddDialog::closeEvent(QCloseEvent *event)
{
	m_searchEdit->clear();
	m_tabBar->setCurrentIndex(0);
	m_stackedWidget->setCurrentIndex(2);
	m_informationWidget->setCurrentIndex(0);
	m_usersModel->clear();
	m_groupsModel->clear();
	return QDialog::closeEvent(event);
}

void QQFriendAddDialog::do_tabBarCurrentChanged(int index)
{
	if (m_searchEdit->text() == "")
	{
		m_stackedWidget->setCurrentIndex(2);
		return;
	}
	m_stackedWidget->setCurrentIndex(index);
}

void QQFriendAddDialog::do_searchTextChanged(const QString &text)
{
	if (text == "")
	{
		m_stackedWidget->setCurrentIndex(2);
		return;
	}
	m_stackedWidget->setCurrentIndex(m_tabBar->currentIndex());
	// 发送搜索请求
	QJsonObject sendData;
	sendData.insert("version", "1.0");
	sendData.insert("sender", "friend");
	if (m_tabBar->currentIndex() == 0)
	{
		sendData.insert("action", "searchfriend");
	}
	else if (m_tabBar->currentIndex() == 1)
	{
		sendData.insert("action", "searchgroup");
	}
	QJsonObject data;
	data.insert("text", text);
	sendData.insert("data", data);
	if (m_tabBar->currentIndex() == 0)
	{
		Q_MSEND_EVENT(QQEnums::searchfriend, QJsonDocument(sendData).toJson());
	}
	else if (m_tabBar->currentIndex() == 1)
	{
		Q_MSEND_EVENT(QQEnums::searchgroup, QJsonDocument(sendData).toJson());
	}
}

void QQFriendAddDialog::do_userClickFriendItem(const QModelIndex &index)
{
	QQConfigs::FriendConfig *user = const_cast<QQConfigs::FriendConfig *>(qvariant_cast<QQConfigs::FriendConfig *>(index.data(Qt::UserRole)));
	QQFriendValidateInfo::getInstance()->setBasePeople(user);
}

void QQFriendAddDialog::do_userClickGroupItem(const QModelIndex &index)
{
	QQConfigs::GroupConfig *group = const_cast<QQConfigs::GroupConfig *>(qvariant_cast<QQConfigs::GroupConfig *>(index.data(Qt::UserRole)));
	QQFriendValidateInfo::getInstance()->setBaseGroup(group);
}
