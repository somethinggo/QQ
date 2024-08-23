#include "QQEmoji.h"

void QQEmojiProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == PE_Frame)
	{
		painter->setPen(QQGlobals::g_theme->g_line_color);
		painter->drawLine(option->rect.bottomLeft(), option->rect.bottomRight());
		return;
	}
	return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void QQEmojiProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == CE_PushButton)
	{
		QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
		if (buttonOption && (buttonOption->state & State_MouseOver))
		{
			buttonOption->state &= ~State_MouseOver;
			painter->save();
			painter->setRenderHint(QPainter::Antialiasing, true);
			QPainterPath path;
			path.addRoundedRect(option->rect, 10, 10);
			painter->fillPath(path, QQGlobals::g_theme->m_emojiLab_hovered);
			painter->restore();
		}
		if (buttonOption && (buttonOption->state & State_Sunken))
		{
			buttonOption->state &= ~State_Sunken;
			buttonOption->state &= ~State_MouseOver;
			painter->save();
			painter->setRenderHint(QPainter::Antialiasing, true);
			QPainterPath path;
			path.addRoundedRect(option->rect, 10, 10);
			painter->fillPath(path, QQGlobals::g_theme->m_emojiLab_pressed);
			painter->restore();
		}
	}
	else if (element == CE_ItemViewItem)
	{
		QStyleOptionViewItem *itemOption = const_cast<QStyleOptionViewItem *>(qstyleoption_cast<const QStyleOptionViewItem *>(option));
		if (itemOption && (itemOption->state & State_HasFocus))
		{
			itemOption->state &= ~State_HasFocus;
		}
	}
	return QProxyStyle::drawControl(element, option, painter, widget);
}

QQEmoji::QQEmoji(QWidget *parent)
	: QDialog(parent)
{
	this->setFixedSize(500, 360);
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, Qt::white);
	this->setPalette(palette);
	this->setAutoFillBackground(true);

	m_proxyStyle = new QQEmojiProxyStyle;
	m_appBar = new QQWidgets::AppBar(this, QQEnums::AppBarHintType::NoButtonHint);
	m_appBar->setIsAllowDrag(false);
	m_appBar->setIsAllowResize(false);
	m_stackedWidget = new QStackedWidget(this);
	m_stackedWidget->setStyle(m_proxyStyle);
	m_tabBar = new QQWidgets::AnimationTabBar(this);
	m_tabBar->setFixedHeight(50);
	m_tabBar->setIconSize(QSize(30, 30));
	m_tabBar->setMaxWidth(50);
	m_tabBar->setContentsMargins(5, 0, 5, 0);
	m_tabBar->setHoverColor(QQGlobals::g_theme->m_emoji_tabbar_hovered);
	m_tabBar->setSelectColor(QQGlobals::g_theme->m_emoji_tabbar_pressed);
	m_tabBar->addTab(QQGlobals::g_theme->m_emoji_tabbar_emoji_icon, QString());
	m_tabBar->addTab(QQGlobals::g_theme->m_emoji_tabbar_like_icon, QString());

	QHBoxLayout *optionLayout = new QHBoxLayout;
	optionLayout->setContentsMargins(0, 5, 0, 5);
	optionLayout->setSpacing(0);
	optionLayout->addWidget(m_tabBar);
	optionLayout->addStretch();
	QVBoxLayout *m_layout = new QVBoxLayout;
	m_layout->setContentsMargins(5, 5, 5, 0);
	m_layout->setSpacing(0);
	m_layout->addWidget(m_appBar);
	m_layout->addWidget(m_stackedWidget);
	m_layout->addLayout(optionLayout);
	this->setLayout(m_layout);
	initEmojiView();
	connect(m_tabBar, &QQWidgets::AnimationTabBar::currentChanged, m_stackedWidget, &QStackedWidget::setCurrentIndex);
}

QQEmoji::~QQEmoji()
{
	delete m_proxyStyle;
}

void QQEmoji::appendTabView()
{
	m_models.append(QPair<QStandardItemModel *, QPoint>(new QStandardItemModel(m_stackedWidget), QPoint(0, 0)));
	m_models[m_models.count() - 1].first->setColumnCount(MAX_COLUMN_COUNT);
	QTableView *tableView = new QTableView(m_stackedWidget);
	tableView->horizontalHeader()->hide();
	tableView->verticalHeader()->hide();
	tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	tableView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	tableView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	tableView->horizontalHeader()->setDefaultSectionSize(50);
	tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
	tableView->verticalHeader()->setDefaultSectionSize(50);
	tableView->setShowGrid(false);
	tableView->setStyle(m_proxyStyle);
	tableView->setFocusPolicy(Qt::NoFocus);
	tableView->setSelectionMode(QAbstractItemView::NoSelection);
	tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableView->setModel(m_models[m_models.count() - 1].first);
	m_stackedWidget->addWidget(tableView);
}

void QQEmoji::appendEmojiByUnicode(char32_t *start, int len, char32_t end)
{
	QTableView *tableView = qobject_cast<QTableView *>(m_stackedWidget->widget(0));
	for (char32_t i = start[0]; i <= end; ++i)
	{
		start[0] = i;
		QString emoji = QString::fromUcs4(start);
		if (emoji.isEmpty())
		{
			continue;
		}
		QStandardItem *item = new QStandardItem;
		QQEmojiLab *emojiLab = new QQEmojiLab(tableView);
		emojiLab->setFont(QQGlobals::g_theme->m_emojiLab_unicode_font);
		emojiLab->setAlignment(Qt::AlignCenter);
		emojiLab->setFixedSize(50, 50);
		emojiLab->setText(emoji);
		if (emojiLab->text() != emoji)
		{
			delete item;
			delete emojiLab;
			continue;
		}
		insertData(0, item);
		m_models[0].first->setData(m_models[0].first->indexFromItem(item), QString("Unicode"), Qt::UserRole);
		m_models[0].first->setData(m_models[0].first->indexFromItem(item), emoji, Qt::UserRole + 1);
		tableView->setIndexWidget(m_models[0].first->indexFromItem(item), emojiLab);
	}
	delete[] start;
}

void QQEmoji::insertData(int index, QStandardItem *item)
{
	QPoint point = m_models[index].second;
	if (point.x() >= m_models[index].first->rowCount())
	{
		m_models[index].first->insertRow(point.x());
	}
	if (point.y() >= m_models[index].first->columnCount())
	{
		point.setY(0);
		point.setX(point.x() + 1);
	}
	m_models[index].first->setItem(point.x(), point.y(), item);
	point.setY(point.y() + 1);
	m_models[index].second = point;
}

void QQEmoji::initEmojiView()
{
	// 加载系统的表情
	appendTabView();
	// Unicode的emoji表情--ttf不支持部分emoji表情--直接加载卡顿
	appendEmojiByUnicode(new char32_t[2]{0x1F601, 0x0}, 2, 0x1F64F);
	appendEmojiByUnicode(new char32_t[2]{0x1F910, 0x0}, 2, 0x1F9FF);
	appendEmojiByUnicode(new char32_t[2]{0x02600, 0x0}, 2, 0x026FF);
	appendEmojiByUnicode(new char32_t[2]{0x1F300, 0x0}, 2, 0x1F5FF);
	appendEmojiByUnicode(new char32_t[2]{0x1F400, 0x0}, 2, 0x1F4D3);
	appendEmojiByUnicode(new char32_t[2]{0x1F950, 0x0}, 2, 0x1F96B);
	appendEmojiByUnicode(new char32_t[2]{0x1F3A0, 0x0}, 2, 0x1F3FA);
	appendEmojiByUnicode(new char32_t[2]{0x1F4A0, 0x0}, 2, 0x1F4FE);
	appendEmojiByUnicode(new char32_t[2]{0x02700, 0x0}, 2, 0x027BF);
	appendEmojiByUnicode(new char32_t[2]{0x1F680, 0x0}, 2, 0x1F6FD);

	appendTabView();
	// 用户添加表情按键
	QStandardItem *item = new QStandardItem;
	QPushButton *appendEmojiButton = new QPushButton(qobject_cast<QTableView *>(m_stackedWidget->widget(1)));
	appendEmojiButton->setFixedSize(QSize(50, 50));
	appendEmojiButton->setFlat(true);
	appendEmojiButton->setIcon(QQGlobals::g_theme->m_emojiLab_append_icon);
	appendEmojiButton->setIconSize(QSize(30, 30));
	appendEmojiButton->setStyle(m_proxyStyle);
	insertData(1, item);
	qobject_cast<QTableView *>(m_stackedWidget->widget(1))->setIndexWidget(m_models[1].first->indexFromItem(item), appendEmojiButton);
	connect(appendEmojiButton, &QPushButton::clicked, this, &QQEmoji::do_emojiAppendBtnClicked);

	QJsonObject sendData;
	sendData.insert("version", *(QQGlobals::g_version));
	sendData.insert("sender", "emoji");
	sendData.insert("receiver", "storage");
	sendData.insert("action", "load");
	sendData.insert("data", QJsonObject());
	QQ_SEND_EVENT_GLOBAL(QQEnums::loadstorage, QJsonDocument(sendData).toJson());

	for (int i = 0; i < m_stackedWidget->count(); ++i)
	{
		QTableView *tableView = qobject_cast<QTableView *>(m_stackedWidget->widget(i));
		connect(tableView, &QTableView::clicked, this, &QQEmoji::do_emojiEmojiItemClicked);
	}
}

void QQEmoji::loadEmojiData(const QByteArray &data)
{
}

bool QQEmoji::event(QEvent *event)
{
	switch (event->type())
	{
	default:
		break;
	}
	return QDialog::event(event);
}

void QQEmoji::do_emojiAppendBtnClicked()
{
	QFileDialog fileDialog(this);
	fileDialog.setFileMode(QFileDialog::ExistingFiles);
	fileDialog.setNameFilter("Images (*.png *.jpg *.gif *.ico *.jpeg)");
	if (fileDialog.exec())
	{
		for (auto fileName : fileDialog.selectedFiles())
		{
			QFile file(fileName);
			QPair<qreal, QString> pair = QQFunctions::getFileFormatSize(file.size());
			if (pair.second == "TB" || pair.second == "GB" || (pair.first > 8 && pair.second == "MB"))
			{
				ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("警告,表情文件不得高于8MB!!!"), 3000, this);
				return;
			}
			if (!file.open(QIODevice::ReadOnly))
			{
				ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("警告,表情文件打开失败!!!"), 3000, this);
				return;
			}
			QImageReader reader(fileName);
			QString format = reader.format();

			QStandardItem *item = new QStandardItem;
			QTableView *tableView = qobject_cast<QTableView *>(m_stackedWidget->widget(1));
			QQEmojiLab *emojiLab = new QQEmojiLab(tableView);
			emojiLab->setAlignment(Qt::AlignCenter);
			emojiLab->setFixedSize(50, 50);
			if (format == "gif")
			{
				QMovie *movie = new QMovie(fileName);
				emojiLab->setMovie(movie);
			}
			else
			{
				QPixmap pixmap(fileName);
				emojiLab->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio));
			}
			insertData(1, item);
			tableView->setIndexWidget(m_models[1].first->indexFromItem(item), emojiLab);

			QJsonObject emojiData;
			emojiData.insert("version", "1.0");
			emojiData.insert("sender", "emoji");
			emojiData.insert("action", "append");
			QJsonObject data;
			data.insert("type", format);
			data.insert("content", QString(file.readAll()));
			emojiData.insert("data", data);
			QQ_SEND_EVENT(QQEnums::addemoji, QByteArray(QJsonDocument(emojiData).toJson()));
		}
	}
}

void QQEmoji::do_emojiEmojiItemClicked(const QModelIndex &index)
{
	this->accept();
	QJsonObject emojiData;
	emojiData.insert("version", "1.0");
	emojiData.insert("sender", "emoji");
	emojiData.insert("action", "send");
	QJsonObject data;
	data.insert("type", index.data(Qt::UserRole).toString());
	data.insert("content", index.data(Qt::UserRole + 1).toString());
	emojiData.insert("data", data);
	QQ_SEND_EVENT(QQEnums::sendemoji, QByteArray(QJsonDocument(emojiData).toJson()));
}
