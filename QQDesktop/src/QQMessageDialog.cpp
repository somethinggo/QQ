#include "QQMessageDialog.h"

void QQMessageDialogButtonStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == CE_PushButton)
	{
		QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
		if (buttonOption && (buttonOption->state & State_Sunken))
		{
			buttonOption->state &= ~State_Sunken;
			buttonOption->state &= ~State_MouseOver;
			int h, s, v;
			QColor color = widget->palette().brush(QPalette::Button).color();
			color.getHsv(&h, &s, &v);
			v = abs((v - 100) % 256);
			color.setHsv(h, s, v);
			painter->fillRect(option->rect, color);
		}
		if (buttonOption && (buttonOption->state & State_MouseOver))
		{
			buttonOption->state &= ~State_MouseOver;
			int h, s, v;
			QColor color = widget->palette().brush(QPalette::Button).color();
			color.getHsv(&h, &s, &v);
			v = abs((v - 30) % 256);
			color.setHsv(h, s, v);
			painter->fillRect(option->rect, color);
		}
	}
	return QProxyStyle::drawControl(element, option, painter, widget);
}

QQMessageDialog::QQMessageDialog(QWidget *parent)
	: QDialog(parent)
{
	this->setFixedWidth(420);
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, Qt::white);
	this->setPalette(palette);
	m_proxyStyle = new QQMessageDialogButtonStyle;
	m_tittle = QString();

	m_shadowWidget = new QWidget(this);
	QPalette shadowWidgetPalette = m_shadowWidget->palette();
	shadowWidgetPalette.setColor(QPalette::Window, QColor(0, 0, 0, 90));
	m_shadowWidget->setPalette(shadowWidgetPalette);
	m_shadowWidget->setAutoFillBackground(true);

	m_appBar = new QQAppBar(this, ClientEnums::AppBarHintType::NoButtonHint);
	m_appBar->setMIsAllowDrag(false);
	m_appBar->setMIsAllowResize(false);
	m_tittleLabel = new QLabel(this);
	m_tittleLabel->setAlignment(Qt::AlignCenter);
	m_tittleLabel->setFont(QFont("Microsoft YaHei", 9));
	QPalette tittleLabelPalette = m_tittleLabel->palette();
	tittleLabelPalette.setColor(QPalette::WindowText, QColor("#7F7F7F"));
	m_tittleLabel->setPalette(tittleLabelPalette);
	m_tittleLabel->setFixedHeight(20);

	m_buttonBox = new QDialogButtonBox(this);
	m_buttonBox->setOrientation(Qt::Horizontal);
	QPushButton *okButton = m_buttonBox->addButton(QString::fromLocal8Bit("确定"), QDialogButtonBox::AcceptRole);
	okButton->setFixedSize(QSize(100, 40));
	okButton->setStyle(m_proxyStyle);
	okButton->setFont(QFont("Microsoft YaHei", 12));
	okButton->setMask(getRoundedRect(okButton->size(), 5));
	okButton->setFlat(true);
	okButton->setAutoFillBackground(true);
	QPushButton *cancelButton = m_buttonBox->addButton(QString::fromLocal8Bit("取消"), QDialogButtonBox::RejectRole);
	cancelButton->setFixedSize(QSize(100, 40));
	cancelButton->setStyle(m_proxyStyle);
	cancelButton->setFont(QFont("Microsoft YaHei", 12));
	cancelButton->setMask(getRoundedRect(cancelButton->size(), 5));
	cancelButton->setFlat(true);
	QPalette cancelPalette = cancelButton->palette();
	cancelPalette.setColor(QPalette::Button, Qt::red);
	cancelPalette.setColor(QPalette::ButtonText, Qt::white);
	cancelButton->setPalette(cancelPalette);
	cancelButton->setAutoFillBackground(true);

	QVBoxLayout *m_layout = new QVBoxLayout(this);
	m_layout->setContentsMargins(20, 20, 20, 20);
	m_layout->setSpacing(15);
	m_layout->addWidget(m_appBar);
	m_layout->addWidget(m_tittleLabel);
	m_layout->addWidget(m_buttonBox);
	closeOkButton();
	connect(m_buttonBox, &QDialogButtonBox::accepted, this, &QQMessageDialog::do_accepted);
	connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QQMessageDialog::reject);
}

QQMessageDialog::~QQMessageDialog()
{
	delete m_proxyStyle;
}

void QQMessageDialog::removeNextAllWidget(QLayout *layout)
{
	QQueue<QLayout *> layoutQueue;
	layoutQueue.enqueue(layout);
	while (!layoutQueue.isEmpty())
	{
		QLayout *nextLayout = layoutQueue.dequeue();
		QLayoutItem *item = nextLayout->takeAt(0);
		while (item != nullptr)
		{
			if (item->widget() != nullptr)
			{
				nextLayout->removeItem(item);
				delete item;
			}
			else if (item->spacerItem() != nullptr)
			{
				nextLayout->removeItem(item);
				delete item;
			}
			else if (item->layout() != nullptr)
			{
				layoutQueue.enqueue(item->layout());
				nextLayout->removeItem(item);
			}
			item = nextLayout->takeAt(0);
		}
		if (nextLayout != layout)
		{
			delete nextLayout;
		}
	}
}

void QQMessageDialog::showEvent(QShowEvent *event)
{
	if (m_shadowWidget != nullptr)
	{
		m_shadowWidget->setVisible(true);
	}
	return QDialog::showEvent(event);
}

void QQMessageDialog::hideEvent(QHideEvent *event)
{
	if (m_shadowWidget != nullptr)
	{
		m_shadowWidget->setVisible(false);
	}
	return QDialog::hideEvent(event);
}

void QQMessageDialog::setMShadowWidget(QWidget *parent)
{
	m_shadowWidget->setParent(parent);
	m_shadowWidget->setFixedSize(parent->size());
	m_shadowWidget->move(0, 0);
	m_shadowWidget->setVisible(false);
}

void QQMessageDialog::setMTittle(const QString &tittle)
{
	m_tittle = tittle;
	m_tittleLabel->setText(m_tittle);
}

void QQMessageDialog::addMWidget(QWidget *widget)
{
	qobject_cast<QVBoxLayout *>(this->layout())->insertWidget(this->layout()->count() - 1, widget);
}

void QQMessageDialog::removeAllMWidget()
{
	while (this->layout()->count() > 3)
	{
		QLayoutItem *item = this->layout()->takeAt(2);
		if (item->widget() != nullptr)
		{
			this->layout()->removeItem(item);
			delete item->widget();
		}
		else if (item->spacerItem() != nullptr)
		{
			this->layout()->removeItem(item);
			delete item->spacerItem();
		}
		else if (item->layout() != nullptr)
		{
			removeNextAllWidget(item->layout());
			this->layout()->removeItem(item);
			delete item->layout();
		}
		delete item;
	}
}

void QQMessageDialog::setMFunction(std::function<void()> &function)
{
	m_function.emplace();
	m_function = function;
}

void QQMessageDialog::setMAlertLabel(const QString &text)
{
	// 如果正在警告中，不允许再次警告,默认2.5秒后恢复
	m_isAlerting.emplace(false);
	m_alert.emplace();
	m_alert = text;
	QPalette palette = m_tittleLabel->palette();
	palette.setColor(QPalette::WindowText, Qt::red);
	m_tittleLabel->setPalette(palette);
	m_tittleLabel->setText(m_alert.value());
	QTimer::singleShot(2500, this, [=]()
					   {
			if (m_isAlerting.has_value())
			{
				QPalette palette = m_tittleLabel->palette();
				palette.setColor(QPalette::WindowText, QColor("#7F7F7F"));
				m_tittleLabel->setPalette(palette);
				m_tittleLabel->setText(m_tittle);
				m_isAlerting = false;
			} });
}

void QQMessageDialog::openOkButton()
{
	QPushButton *button = qobject_cast<QPushButton *>(m_buttonBox->buttons().at(0));
	button->setCursor(Qt::ArrowCursor);
	button->setEnabled(true);
	QPalette palette = button->palette();
	palette.setColor(QPalette::Button, QColor("#0099FF"));
	palette.setColor(QPalette::ButtonText, QColor("#FFFFFF"));
	button->setPalette(palette);
}

void QQMessageDialog::closeOkButton()
{
	QPushButton *button = qobject_cast<QPushButton *>(m_buttonBox->buttons().at(0));
	button->setCursor(Qt::ForbiddenCursor);
	button->setEnabled(false);
	QPalette palette = button->palette();
	palette.setColor(QPalette::Button, QColor("#B2E0FF"));
	palette.setColor(QPalette::ButtonText, QColor("#FFFFFF"));
	button->setPalette(palette);
}

void QQMessageDialog::do_accepted()
{
	// 如果正在警告中，不允许执行
	if (m_function.has_value())
	{
		m_function.value()();
		if (m_isAlerting.has_value() && m_isAlerting)
		{
			return;
		}
	}
	this->accept();
}
