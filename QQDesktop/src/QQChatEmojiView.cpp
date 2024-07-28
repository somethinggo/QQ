#include "QQChatEmojiView.h"

void QQChatEmojiViewProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == PE_Frame)
	{
		painter->setPen(QColor("#E5E5E5"));
		painter->drawLine(option->rect.bottomLeft(), option->rect.bottomRight());
		return;
	}
	return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void QQChatEmojiViewProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
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
			painter->fillPath(path, QColor("#C5C4C5"));
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
			painter->fillPath(path, QColor("#0099FF"));
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

QMEmojiLabel::QMEmojiLabel(QWidget *parent)
	: QLabel(parent)
{
	m_isHovered = false;
	m_isPressed = false;
}

QMEmojiLabel::~QMEmojiLabel()
{
}

void QMEmojiLabel::setMMovie(QMovie *movie)
{
	if (movie != nullptr)
	{
		movie->jumpToFrame(0);
	}
	return QLabel::setMovie(movie);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void QMEmojiLabel::enterEvent(QEnterEvent *event)
#else
void QMEmojiLabel::enterEvent(QEvent *event)
#endif
{
	m_isHovered = true;
	if (this->movie() != nullptr)
	{
		this->movie()->jumpToFrame(0);
		this->movie()->start();
	}
	this->update();
	return QLabel::enterEvent(event);
}

void QMEmojiLabel::leaveEvent(QEvent *event)
{
	m_isHovered = false;
	if (this->movie() != nullptr)
	{
		this->movie()->stop();
		this->movie()->jumpToFrame(0);
	}
	this->update();
	return QLabel::leaveEvent(event);
}

void QMEmojiLabel::mousePressEvent(QMouseEvent *event)
{
	m_isPressed = true;
	this->update();
	return QLabel::mousePressEvent(event);
}

void QMEmojiLabel::mouseReleaseEvent(QMouseEvent *event)
{
	m_isPressed = false;
	this->update();
	return QLabel::mouseReleaseEvent(event);
}

void QMEmojiLabel::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
#else
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
#endif
	if (m_isPressed)
	{
		QPainterPath path;
		path.addRoundedRect(this->rect(), 10, 10);
		painter.fillPath(path, QColor("#0099FF"));
	}
	else if (m_isHovered)
	{
		QPainterPath path;
		path.addRoundedRect(this->rect(), 10, 10);
		painter.fillPath(path, QColor("#C5C4C5"));
	}
	return QLabel::paintEvent(event);
}

QMStandardItemModel::QMStandardItemModel(QObject *parent)
	: QStandardItemModel(parent)
{
	m_currentRow = 0;
	m_currentColumn = 0;
}

QMStandardItemModel::~QMStandardItemModel()
{
}

void QMStandardItemModel::setStartPos(uint row, uint column)
{
	if (row >= this->rowCount() || column >= this->columnCount())
	{
		return;
	}
	m_currentRow = row;
	m_currentColumn = column;
}

void QMStandardItemModel::insertData(QStandardItem *item)
{
	if (m_currentColumn >= this->columnCount())
	{
		m_currentColumn = 0;
		m_currentRow++;
	}
	if (m_currentRow >= this->rowCount())
	{
		this->insertRow(m_currentRow);
	}
	this->setItem(m_currentRow, m_currentColumn, item);
	m_currentColumn++;
}

QQChatEmojiView::QQChatEmojiView(QWidget *parent)
	: QDialog(parent)
{
	this->setFixedSize(500, 360);
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, Qt::white);
	this->setPalette(palette);
	this->setAutoFillBackground(true);

	m_proxyStyle = new QQChatEmojiViewProxyStyle;
	m_appBar = new QQAppBar(this, ClientEnums::AppBarHintType::NoButtonHint);
	m_appBar->setMIsAllowDrag(false);
	m_appBar->setMIsAllowResize(false);
	m_stackedWidget = new QStackedWidget(this);
	m_stackedWidget->setStyle(m_proxyStyle);
	m_tabBar = new QQActionTabBar(this);
	m_tabBar->setFixedHeight(50);
	m_tabBar->setIconSize(QSize(30, 30));
	m_tabBar->setMMaxWidth(50);
	m_tabBar->setMContentsMargins(5, 0, 5, 0);
	m_tabBar->setMHoverColor(QColor("#C5C4C5"));
	m_tabBar->setMSelectColor(QColor("#0099FF"));
	QIcon emojiIcon;
	emojiIcon.addFile(QString(":/emoji/option/images/emoji/option/emojiBtnIcon_no.png"), QSize(), QIcon::Normal, QIcon::Off);
	emojiIcon.addFile(QString(":/emoji/option/images/emoji/option/emojiBtnIcon_yes.png"), QSize(), QIcon::Normal, QIcon::On);
	m_tabBar->addTab(emojiIcon, QString());
	QIcon likeIcon;
	likeIcon.addFile(QString(":/emoji/option/images/emoji/option/likeBtnIcon_no.png"), QSize(), QIcon::Normal, QIcon::Off);
	likeIcon.addFile(QString(":/emoji/option/images/emoji/option/likeBtnIcon_yes.png"), QSize(), QIcon::Normal, QIcon::On);
	m_tabBar->addTab(likeIcon, QString());

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
	connect(m_tabBar, &QQActionTabBar::currentChanged, m_stackedWidget, &QStackedWidget::setCurrentIndex);
}

QQChatEmojiView::~QQChatEmojiView()
{
	delete m_proxyStyle;
}

void QQChatEmojiView::appendTabView()
{
	m_models.append(new QMStandardItemModel(m_stackedWidget));
	m_models[m_models.count() - 1]->setColumnCount(MAX_COLUMN_COUNT);
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
	tableView->setModel(m_models[m_models.count() - 1]);
	m_stackedWidget->addWidget(tableView);
}

void QQChatEmojiView::appendEmojiByUnicode(char32_t *start, int len, char32_t end)
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
		QMEmojiLabel *emojiLabel = new QMEmojiLabel(tableView);
		emojiLabel->setFont(QFont("Segoe UI Emoji", 20));
		emojiLabel->setAlignment(Qt::AlignCenter);
		emojiLabel->setFixedSize(50, 50);
		emojiLabel->setText(emoji);
		if (emojiLabel->text() != emoji)
		{
			delete item;
			delete emojiLabel;
			continue;
		}
		m_models[0]->insertData(item);
		m_models[0]->setData(m_models[0]->indexFromItem(item), "unicode", Qt::UserRole);
		tableView->setIndexWidget(m_models[0]->indexFromItem(item), emojiLabel);
	}
	delete[] start;
}

void QQChatEmojiView::initEmojiView()
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

	// 加载用户的表情
	appendTabView();
	// 用户添加表情按键
	QStandardItem *item = new QStandardItem;
	QPushButton *appendEmojiButton = new QPushButton(qobject_cast<QTableView *>(m_stackedWidget->widget(1)));
	appendEmojiButton->setFixedSize(QSize(50, 50));
	appendEmojiButton->setFlat(true);
	appendEmojiButton->setIcon(QIcon(":/emoji/option/images/emoji/option/appendIcon_no.png"));
	appendEmojiButton->setIconSize(QSize(30, 30));
	appendEmojiButton->setStyle(m_proxyStyle);
	m_models[1]->insertData(item);
	qobject_cast<QTableView *>(m_stackedWidget->widget(1))->setIndexWidget(m_models[1]->indexFromItem(item), appendEmojiButton);
	connect(appendEmojiButton, &QPushButton::clicked, this, &QQChatEmojiView::do_emojiAppendBtnClicked);

	// 读取配置文件
	QFile configFile(configFileName);
	if (configFile.open(QIODevice::ReadOnly))
	{
		QJsonDocument doc = QJsonDocument::fromJson(configFile.readAll());
		QJsonObject obj = doc.object();
		int count = obj.value("count").toInt();
		QJsonArray emojiArray = obj.value("contents").toArray();
		for (int i = 0; i < count; i++)
		{
			QJsonObject value = emojiArray.at(i).toObject();
			QStandardItem *item = new QStandardItem;
			QMEmojiLabel *emojiLabel = new QMEmojiLabel(qobject_cast<QTableView *>(m_stackedWidget->widget(1)));
			emojiLabel->setFixedSize(50, 50);
			emojiLabel->setAlignment(Qt::AlignCenter);
			emojiLabel->setScaledContents(true);
			if (value["type"].toString() == "gif")
			{
				QBuffer *buffer = new QBuffer(emojiLabel);
				buffer->setData(QByteArray::fromBase64(value["data"].toString().toUtf8()));
				QMovie *movie = new QMovie(buffer, QByteArray(), emojiLabel);
				emojiLabel->setMMovie(movie);
			}
			else if (value["type"].toString() == "png" || value["type"].toString() == "jpg" || value["type"].toString() == "jpeg")
			{
				QString base64 = value["data"].toString();
				emojiLabel->setPixmap(QPixmap::fromImage(getBase64ToImage(base64)));
			}
			m_models[1]->insertData(item);
			m_models[1]->setData(m_models[1]->indexFromItem(item), value["type"].toString() == "gif" ? "gif" : "picture", Qt::UserRole);
			qobject_cast<QTableView *>(m_stackedWidget->widget(1))->setIndexWidget(m_models[1]->indexFromItem(item), emojiLabel);
		}
		configFile.close();
	}
	for (int i = 0; i < m_stackedWidget->count(); ++i)
	{
		QTableView *tableView = qobject_cast<QTableView *>(m_stackedWidget->widget(i));
		connect(tableView, &QTableView::clicked, this, &QQChatEmojiView::do_emojiEmojiItemClicked);
	}
}

void QQChatEmojiView::do_emojiAppendBtnClicked()
{
	QFile configFile(configFileName);
	if (configFile.open(QIODevice::ReadOnly))
	{
		QFileDialog fileDialog(this);
		fileDialog.setFileMode(QFileDialog::ExistingFiles);
		fileDialog.setNameFilter("Images (*.png *.jpg *.gif *.ico *.jpeg)");
		if (fileDialog.exec())
		{
			for (auto fileName : fileDialog.selectedFiles())
			{
				QFile file(fileName);
				QPair<qreal, QString> pair = getFileFormatSize(file.size());
				if (pair.second == "TB" || pair.second == "GB" || (pair.first > 8 && pair.second == "MB"))
				{
					ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("警告,表情文件不得高于8MB!!!"), 3000, this);
					configFile.close();
					return;
				}
				QJsonDocument doc = QJsonDocument::fromJson(configFile.readAll());
				QJsonObject obj = doc.object();
				QJsonArray emojiArray = obj.value("contents").toArray();
				QJsonObject value;
				if (file.open(QIODevice::ReadOnly))
				{
					value["data"] = QString(file.readAll().toBase64());
					file.close();
				}
				else
				{
					ElaMessageBar::warning(ElaMessageBarType::Top, "", QString::fromLocal8Bit("警告,表情文件打开失败!!!"), 3000, this);
					configFile.close();
					return;
				}
				if (fileName.endsWith(".gif"))
				{
					value["type"] = "gif";
				}
				else
				{
					value["type"] = "picture";
				}
				emojiArray.append(value);
				obj["contents"] = emojiArray;
				obj["count"] = emojiArray.size();
				configFile.resize(0);
				configFile.write(QJsonDocument(obj).toJson());
				configFile.close();
				QStandardItem *item = new QStandardItem;
				QTableView *tableView = qobject_cast<QTableView *>(m_stackedWidget->widget(1));
				QMEmojiLabel *emojiLabel = new QMEmojiLabel(tableView);
				emojiLabel->setAlignment(Qt::AlignCenter);
				emojiLabel->setFixedSize(50, 50);
				if (value["type"] == "picture")
				{
					QString base64 = value["data"].toString();
					emojiLabel->setPixmap(QPixmap::fromImage(getBase64ToImage(base64)));
				}
				else if (value["type"] == "gif")
				{
					QBuffer *buffer = new QBuffer(emojiLabel);
					buffer->setData(value["data"].toString().toUtf8());
					QMovie *movie = new QMovie(buffer, QByteArray(), emojiLabel);
					emojiLabel->setMMovie(movie);
				}
				m_models[1]->insertData(item);
				m_models[1]->setData(m_models[1]->indexFromItem(item), value["type"] == "gif" ? "gif" : "picture", Qt::UserRole);
				tableView->setIndexWidget(m_models[1]->indexFromItem(item), emojiLabel);
			}
		}
	}
}

void QQChatEmojiView::do_emojiEmojiItemClicked(const QModelIndex &index)
{
	this->accept();
	QString type = index.model()->data(index, Qt::UserRole).toString();
	if (type.isNull())
	{
		return;
	}
	QTableView *tableView = qobject_cast<QTableView *>(sender());
	QMEmojiLabel *label = qobject_cast<QMEmojiLabel *>(tableView->indexWidget(index));
	if (type == "unicode")
	{
		emit sign_emojiItemClicked(label->text(), ClientEnums::EmojiType::unicode);
	}
	else if (type == "gif")
	{
		QMovie *movie = label->movie();
		QBuffer *buffer = qobject_cast<QBuffer *>(movie->device());
		emit sign_emojiItemClicked(buffer->data().toBase64(), ClientEnums::EmojiType::gif);
	}
	else if (type == "picture")
	{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		QImage image = label->pixmap().toImage();
#else
		QImage image = const_cast<QPixmap *>(label->pixmap())->toImage();
#endif
		emit sign_emojiItemClicked(getImageToBase64(image).first, ClientEnums::EmojiType::picture);
	}
}
