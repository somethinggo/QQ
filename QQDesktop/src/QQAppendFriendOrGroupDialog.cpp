#include "QQAppendFriendOrGroupDialog.h"

void QQAppendFriendOrGroupDialogProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == PE_Frame)
	{
		return;
	}
	else if (element == PE_FrameLineEdit)
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

QQAppendFriendOrGroupDialog::QQAppendFriendOrGroupDialog(QWidget *parent)
	: QDialog(parent)
{
	this->setFixedSize(1000, 700);
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, Qt::white);
	this->setPalette(palette);
	this->setAutoFillBackground(true);
	m_proxyStyle = new QQAppendFriendOrGroupDialogProxyStyle;
	m_usersModel = new QStandardItemModel(this);
	m_groupsModel = new QStandardItemModel(this);
	m_friendAndGroupDelegate = new QQSinglePeopleWidgetDelegate(this);

	m_appBar = new QQAppBar(this, ClientEnums::AppBarHintType::WindowsHint);
	m_appBar->setMWindowsTitle(QString::fromLocal8Bit("全网搜索"));

	m_searchEdit = new QLineEdit(this);
	m_searchEdit->setFont(QFont("Microsoft YaHei", 9));
	m_searchEdit->addAction(new QAction(QIcon(":/widget/common/images/widget/common/searchLab.png"), "", m_searchEdit), QLineEdit::LeadingPosition);
	m_searchEdit->setFixedHeight(40);
	m_searchEdit->setStyle(m_proxyStyle);
	m_searchEdit->setPlaceholderText(QString::fromLocal8Bit("输入搜索关键词"));
	m_searchEdit->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);

	m_tabBar = new QQActionTabBar(this);
	m_tabBar->setFont(QFont("Microsoft YaHei", 10));
	m_tabBar->setFixedHeight(40);
	m_tabBar->addTab(QString::fromLocal8Bit("用户"));
	m_tabBar->addTab(QString::fromLocal8Bit("群聊"));
	m_tabBar->setMMargin(5);
	m_tabBar->setMBackgroundColor(QColor("#F5F5F5"));
	m_tabBar->setMHoverColor(QColor("#F5F5F5"));
	m_tabBar->setMSelectColor(QColor("#FFFFFF"));
	m_tabBar->setMSelectTextColor(QColor("#0099FF"));

	// 添加搜索结果列表
	m_stackedWidget = new QStackedWidget(this);
	m_stackedWidget->addWidget(new QQScrollerPixelListView(m_stackedWidget));
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setModel(m_usersModel);
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setItemDelegate(m_friendAndGroupDelegate);
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setHorizontalScrollMode(QAbstractItemView::ScrollMode::ScrollPerPixel);
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setFocusPolicy(Qt::NoFocus);
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setStyle(m_proxyStyle);
	qobject_cast<QListView *>(m_stackedWidget->widget(0))->setAutoFillBackground(true);
	m_stackedWidget->addWidget(new QQScrollerPixelListView(m_stackedWidget));
	qobject_cast<QListView *>(m_stackedWidget->widget(1))->setModel(m_groupsModel);
	qobject_cast<QListView *>(m_stackedWidget->widget(1))->setItemDelegate(m_friendAndGroupDelegate);
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
	m_informationWidget->addWidget(QQAppendNeedInformationWidget::getInstance());

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
	connect(m_searchEdit, &QLineEdit::textChanged, this, &QQAppendFriendOrGroupDialog::do_searchTextChanged);
	connect(m_tabBar, &QTabBar::currentChanged, this, &QQAppendFriendOrGroupDialog::do_tabBarCurrentChanged);
	connect(qobject_cast<QListView *>(m_stackedWidget->widget(0)), &QListView::clicked, this, &QQAppendFriendOrGroupDialog::do_userClickFriendItem);
	connect(qobject_cast<QListView *>(m_stackedWidget->widget(1)), &QListView::clicked, this, &QQAppendFriendOrGroupDialog::do_userClickGroupItem);
}

QQAppendFriendOrGroupDialog::~QQAppendFriendOrGroupDialog()
{
	delete m_proxyStyle;
}

void QQAppendFriendOrGroupDialog::appendUser(ClientConfigs::UserFriend_C *user)
{
	QListView *listView = qobject_cast<QListWidget *>(m_stackedWidget->widget(0));
	QStandardItem *item = new QStandardItem;
	item->setData(QVariant::fromValue(user), Qt::UserRole);
	item->setData(QVariant::fromValue(ClientEnums::PeopleWidgetType::friendappend), Qt::UserRole + 1);
	QString iconBase64 = QString::fromStdString(user->m_icon);
	QPixmap icon = QPixmap::fromImage(getBase64ToImage(iconBase64));
	item->setData(icon, Qt::UserRole + 2);
	QString account = QString::fromStdString(user->m_account.c_str());
	item->setData(account, Qt::UserRole + 4);
	m_usersModel->appendRow(item);
}

void QQAppendFriendOrGroupDialog::appendGroup(ClientConfigs::UserGroup_C *group)
{
	QListView *listView = qobject_cast<QListWidget *>(m_stackedWidget->widget(1));
	QStandardItem *item = new QStandardItem;
	item->setData(QVariant::fromValue(group), Qt::UserRole);
	item->setData(QVariant::fromValue(ClientEnums::PeopleWidgetType::groupappend), Qt::UserRole + 1);
	QString iconBase64 = QString::fromStdString(group->m_icon);
	QPixmap icon = QPixmap::fromImage(getBase64ToImage(iconBase64));
	item->setData(icon, Qt::UserRole + 2);
	QString account = QString::fromStdString(group->m_account.c_str());
	item->setData(account, Qt::UserRole + 4);
	m_groupsModel->appendRow(item);
}

void QQAppendFriendOrGroupDialog::closeEvent(QCloseEvent *event)
{
	// 清除数据,恢复初始状态
	m_searchEdit->clear();
	m_tabBar->setCurrentIndex(0);
	m_stackedWidget->setCurrentIndex(2);
	m_informationWidget->setCurrentIndex(0);
	m_usersModel->clear();
	m_groupsModel->clear();
	return QDialog::closeEvent(event);
}

void QQAppendFriendOrGroupDialog::do_tabBarCurrentChanged(int index)
{
	if (m_searchEdit->text() == "")
	{
		m_stackedWidget->setCurrentIndex(2);
		return;
	}
	m_stackedWidget->setCurrentIndex(index);
}

void QQAppendFriendOrGroupDialog::do_searchTextChanged(const QString &text)
{
	if (text == "")
	{
		m_stackedWidget->setCurrentIndex(2);
		return;
	}
	m_stackedWidget->setCurrentIndex(m_tabBar->currentIndex());
	// 发送搜索请求
	QJsonObject searchData;
	searchData.insert("version", "1.0");
	searchData.insert("sendet", "QQDesktop");
	if (m_tabBar->currentIndex() == 0)
	{
		searchData.insert("action", "search-user-append");
	}
	else
	{
		searchData.insert("action", "search-group-append");
	}
	QJsonObject data;
	data.insert("text", text);
	searchData.insert("data", data);
	GlobalValuesAgent::m_localSocket.write(QJsonDocument(searchData).toJson());
	GlobalValuesAgent::m_localSocket.flush();
	if (GlobalValuesAgent::m_localSocket.waitForReadyRead(3000))
	{
		// todo
		QJsonObject answerData = QJsonDocument::fromJson(GlobalValuesAgent::m_localSocket.readAll()).object();
		if (answerData["result"] == "success")
		{
			if (m_tabBar->currentIndex() == 0)
			{
				m_usersModel->clear();
				QJsonArray users = answerData["data"].toArray();
				for (int i = 0; i < users.size(); i++)
				{
					ClientConfigs::UserFriend_C *user = getUserInfo(users.at(i).toObject()["ID"].toString());
					appendUser(user);
				}
			}
			else
			{
				m_groupsModel->clear();
				QJsonArray groups = answerData["data"].toArray();
				for (int i = 0; i < groups.size(); i++)
				{
					ClientConfigs::UserGroup_C *group = getGroupInfo(groups.at(i).toObject()["ID"].toString());
					appendGroup(group);
				}
			}
		}
	}
}

void QQAppendFriendOrGroupDialog::do_userClickFriendItem(const QModelIndex &index)
{
	// 设置添加验证信息的面板
	ClientConfigs::UserFriend_C *user = const_cast<ClientConfigs::UserFriend_C *>(qvariant_cast<ClientConfigs::UserFriend_C *>(index.data(Qt::UserRole)));
	QQAppendNeedInformationWidget::getInstance()->setMBasePeopleInformation(user);
}

void QQAppendFriendOrGroupDialog::do_userClickGroupItem(const QModelIndex &index)
{
	// 设置添加验证信息的面板
	ClientConfigs::UserGroup_C *group = const_cast<ClientConfigs::UserGroup_C *>(qvariant_cast<ClientConfigs::UserGroup_C *>(index.data(Qt::UserRole)));
	QQAppendNeedInformationWidget::getInstance()->setMBasePeopleInformation(group);
}
