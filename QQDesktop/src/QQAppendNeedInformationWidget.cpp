#include "QQAppendNeedInformationWidget.h"

void QQAppendNeedInformationWidgetProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
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
	QProxyStyle::drawControl(element, option, painter, widget);
}

QQAppendNeedInformationWidget::QQAppendNeedInformationWidget(QWidget *parent)
	: QWidget(parent)
{
	m_proxyStyle = new QQAppendNeedInformationWidgetProxyStyle;
	m_user = nullptr;
	m_group = nullptr;
	m_baseInformationWidget = new QQSinglePeopleWidget(this);
	m_moreInformationWidget = new QScrollArea(this);
	m_moreInformationWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_moreInformationWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	QLayout *moreLayout = new QVBoxLayout;
	moreLayout->setContentsMargins(0, 0, 0, 0);
	moreLayout->setSpacing(10);
	m_moreInformationWidget->setLayout(moreLayout);
	m_buttonBox = new QDialogButtonBox(this);
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
	QLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	layout->addWidget(m_baseInformationWidget);
	layout->addWidget(m_moreInformationWidget);
	layout->addWidget(m_buttonBox);
	this->setLayout(layout);
	connect(m_buttonBox, &QDialogButtonBox::clicked, this, &QQAppendNeedInformationWidget::do_clickAppendButton);
}

QQAppendNeedInformationWidget::~QQAppendNeedInformationWidget()
{
	delete m_proxyStyle;
}

void QQAppendNeedInformationWidget::setMBasePeopleInformation(ClientConfigs::UserFriend_C *user)
{
	this->m_user = user;
	m_baseInformationWidget->setMBasePeopleInformation(m_user, ClientEnums::PeopleWidgetType::friendappend);
	this->update();
}

void QQAppendNeedInformationWidget::setMBasePeopleInformation(ClientConfigs::UserGroup_C *group)
{
	this->m_group = group;
	m_baseInformationWidget->setMBasePeopleInformation(m_group, ClientEnums::PeopleWidgetType::groupappend);
	this->update();
}

QPair<ClientConfigs::UserFriend_C *, ClientConfigs::UserGroup_C *> QQAppendNeedInformationWidget::getMBasePeopleInformation()
{
	return QPair<ClientConfigs::UserFriend_C *, ClientConfigs::UserGroup_C *>(m_user, m_group);
}

void QQAppendNeedInformationWidget::addMMoreInformation(QLabel *label, QWidget *widget)
{
	QLayout *layout = m_moreInformationWidget->layout();
	layout->addWidget(label);
	layout->addWidget(widget);
	m_map.insert(m_map.size(), QPair<QLabel *, QWidget *>(label, widget));
}

void QQAppendNeedInformationWidget::addMMoreInformation(QString name, QWidget *widget)
{
	QLabel *label = new QLabel(name, m_moreInformationWidget);
	QPalette palette = label->palette();
	palette.setColor(QPalette::Text, Qt::gray);
	label->setPalette(palette);
	label->setAutoFillBackground(true);
	label->setFont(QFont("Microsoft YaHei", 9));
	addMMoreInformation(label, widget);
}

QPair<QLabel *, QWidget *> QQAppendNeedInformationWidget::getMInformation(int index)
{
	auto iter = m_map.find(index);
	if (iter != m_map.end())
	{
		return *iter;
	}
	return QPair<QLabel *, QWidget *>(nullptr, nullptr);
}

void QQAppendNeedInformationWidget::removeMInformation(int index)
{
	auto iter = m_map.find(index);
	if (iter != m_map.end())
	{
		QLayout *layout = m_moreInformationWidget->layout();
		layout->removeWidget(iter.value().first);
		layout->removeWidget(iter.value().second);
		delete iter.value().first;
		delete iter.value().second;
		m_map.erase(iter);
	}
}

void QQAppendNeedInformationWidget::removeMAllInformation()
{
	QLayout *layout = m_moreInformationWidget->layout();
	for (auto iter = m_map.begin(); iter != m_map.end(); ++iter)
	{
		layout->removeWidget(iter.value().first);
		layout->removeWidget(iter.value().second);
		delete iter.value().first;
		delete iter.value().second;
	}
	m_map.clear();
}

void QQAppendNeedInformationWidget::do_clickAppendButton(QAbstractButton *button)
{
	if (button == m_buttonBox->buttons()[1])
	{
		return;
	}
	if (m_user != nullptr)
	{
		QJsonObject appendData;
		appendData.insert("version", "1.0");
		appendData.insert("sender", "QQDesktop");
		appendData.insert("action", "append-friend");
		QJsonObject data;
		data.insert("user", QString::fromStdString(m_user->m_ID));
		appendData.insert("data", data);
		GlobalValuesAgent::m_localSocket.write(QJsonDocument(appendData).toJson());
		GlobalValuesAgent::m_localSocket.flush();
	}
	else if (m_group != nullptr)
	{
		QJsonObject appendData;
		appendData.insert("version", "1.0");
		appendData.insert("sender", "QQDesktop");
		appendData.insert("action", "append-group");
		QJsonObject data;
		data.insert("group", QString::fromStdString(m_group->m_ID));
		appendData.insert("data", data);
		GlobalValuesAgent::m_localSocket.write(QJsonDocument(appendData).toJson());
		GlobalValuesAgent::m_localSocket.flush();
	}
}
