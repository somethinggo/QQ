#include "QQSinglePeopleWidget.h"

QQSinglePeopleWidget::QQSinglePeopleWidget(QWidget *parent)
	: QWidget(parent)
{
	m_user = nullptr;
	m_group = nullptr;
	m_type = ClientEnums::PeopleWidgetType::friendchats;
	m_maxStringWidth = this->width();
	m_isSelected = false;
	m_selectedColor = QColor("#0099FF");
}

QQSinglePeopleWidget::~QQSinglePeopleWidget()
{
}

void QQSinglePeopleWidget::setMBasePeopleInformation(ClientConfigs::UserFriend_C *user, ClientEnums::PeopleWidgetType type)
{
	if (m_group != nullptr)
	{
		return;
	}
	m_user = user;
	m_type = type;
	QString base64 = QString::fromLocal8Bit(m_user->m_icon.c_str());
	m_icon = QPixmap::fromImage(getBase64ToImage(base64));
	m_name = QString::fromLocal8Bit(m_user->m_name.c_str());
	switch (m_type)
	{
	case ClientEnums::PeopleWidgetType::friendchats:
	{
		if (m_user->m_nikeName != "")
		{
			m_name = m_user->m_nikeName;
		}
		m_message.emplace(m_user->m_lastMsg);
		m_distrupIcon.emplace();
		m_distrupIcon.value().addFile(":/widget/common/images/widget/common/distrup_no.png", QSize(), QIcon::Normal, QIcon::On);
		m_distrupIcon.value().addFile(":/widget/common/images/widget/common/distrup_yes.png", QSize(), QIcon::Normal, QIcon::Off);
		m_message_rect.emplace();
		m_state_rect.emplace();
		m_time_rect.emplace();
		break;
	}
	case ClientEnums::PeopleWidgetType::friends:
	{
		if (m_user->m_nikeName != "")
		{
			m_name = m_name + QString("(%1)").arg(m_user->m_nikeName);
		}
		m_message.emplace(QString::fromLocal8Bit(m_user->m_sign.c_str()));
		m_message_rect.emplace();
		break;
	}
	case ClientEnums::PeopleWidgetType::friendappend:
	{
		m_message.emplace(QString::fromLocal8Bit(m_user->m_account.c_str()));
		m_message_rect.emplace();
		break;
	}
	default:
		break;
	}
	this->update();
}

void QQSinglePeopleWidget::setMBasePeopleInformation(ClientConfigs::UserGroup_C *group, ClientEnums::PeopleWidgetType type)
{
	if (m_user != nullptr)
	{
		return;
	}
	m_group = group;
	m_type = type;
	QString base64 = QString::fromLocal8Bit(m_group->m_icon.c_str());
	m_icon = QPixmap::fromImage(getBase64ToImage(base64));
	m_name = QString::fromLocal8Bit(m_group->m_name.c_str());
	switch (m_type)
	{
	case ClientEnums::PeopleWidgetType::groupchats:
	{
		if (m_group->m_nikeName != "")
		{
			m_name = m_group->m_nikeName;
		}
		m_message.emplace(m_group->m_lastMsg);
		m_distrupIcon.emplace();
		m_distrupIcon.value().addFile(":/widget/common/images/widget/common/distrup_no.png", QSize(), QIcon::Normal, QIcon::On);
		m_distrupIcon.value().addFile(":/widget/common/images/widget/common/distrup_yes.png", QSize(), QIcon::Normal, QIcon::Off);
		m_message_rect.emplace();
		m_state_rect.emplace();
		m_time_rect.emplace();
		break;
	}
	case ClientEnums::PeopleWidgetType::groupappend:
	{
		m_message.emplace(QString::fromLocal8Bit(m_group->m_account.c_str()));
		m_message_rect.emplace();
		break;
	}
	default:
		break;
	}
	this->update();
}

QPair<ClientConfigs::UserFriend_C *, ClientConfigs::UserGroup_C *> QQSinglePeopleWidget::getMBasePeopleInformation()
{
	return QPair<ClientConfigs::UserFriend_C *, ClientConfigs::UserGroup_C *>(m_user, m_group);
}

ClientEnums::PeopleWidgetType QQSinglePeopleWidget::getMWidgetType()
{
	return m_type;
}

void QQSinglePeopleWidget::setMMaxStringLength(qreal length)
{
	m_maxStringWidth = length;
}

void QQSinglePeopleWidget::setMSelectContentsColor(QColor color, bool selected)
{
	m_selectedColor = color;
	m_isSelected = selected;
	this->update();
}

void QQSinglePeopleWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	if (m_isSelected)
	{
		painter.setPen(m_selectedColor);
	}
	else
	{
		painter.setPen(Qt::black);
	}

	int8_t commonSpace = 10;
	if (m_user != nullptr)
	{
		painter.save();
		m_icon_rect = QRect(this->height() / 2 - 25, this->height() / 2 - 25, 50, 50);
		if (m_icon.isNull())
		{
			QString base64 = QString::fromLocal8Bit(m_user->m_icon.c_str());
			m_icon = QPixmap::fromImage(getBase64ToImage(base64));
		}
		m_icon = getRounedPixmap(m_icon.scaled(m_icon_rect.size(), Qt::KeepAspectRatio), m_icon_rect.size().width() / 2);
		painter.drawPixmap(m_icon_rect, m_icon);
		painter.restore();
	}
	else if (m_group != nullptr)
	{
		painter.save();
		m_icon_rect = QRect(this->height() / 2 - 25, this->height() / 2 - 25, 50, 50);
		if (m_icon.isNull())
		{
			QString base64 = QString::fromLocal8Bit(m_group->m_icon.c_str());
			m_icon = QPixmap::fromImage(getBase64ToImage(base64));
		}
		m_icon = getRounedPixmap(m_icon.scaled(m_icon_rect.size(), Qt::KeepAspectRatio), m_icon_rect.size().width() / 2);
		painter.drawPixmap(m_icon_rect, m_icon);
		painter.restore();
	}
	switch (m_type)
	{
	case ClientEnums::PeopleWidgetType::friendchats:
	{
		if (m_user == nullptr)
		{
			return;
		}
		painter.save();
		painter.setFont(QFont("Microsoft YaHei", 12));
		if (m_name.isEmpty())
		{
			m_name = QString::fromLocal8Bit(m_user->m_name.c_str());
			if (m_user->m_nikeName != "")
			{
				m_name = m_user->m_nikeName;
			}
		}
		m_name_rect = getCalculateTextRects(m_name, painter.font(), m_maxStringWidth, true);
		m_name_rect.moveTopLeft(QPoint(m_icon_rect.right() + commonSpace, m_icon_rect.top()));
		painter.drawText(m_name_rect, m_name, Qt::AlignLeft | Qt::AlignVCenter);
		painter.restore();
		if (m_message_rect.has_value())
		{
			painter.save();
			painter.setFont(QFont("Microsoft YaHei", 10));
			if (m_message.has_value() && m_message.value().isEmpty())
			{
				m_message = m_user->m_lastMsg;
			}
			if (m_isSelected)
			{
				painter.setPen(m_selectedColor);
			}
			else
			{
				painter.setPen(Qt::gray);
			}
			m_message_rect = getCalculateTextRects(m_message.value(), painter.font(), m_maxStringWidth, true);
			m_message_rect.value().moveTopLeft(QPoint(m_name_rect.left(), m_name_rect.bottom()));
			painter.drawText(m_message_rect.value(), m_message.value(), Qt::AlignLeft | Qt::AlignVCenter);
			painter.restore();
		}
		if (m_time_rect.has_value())
		{
			painter.save();
			painter.setFont(QFont("Microsoft YaHei", 8));
			if (m_isSelected)
			{
				painter.setPen(m_selectedColor);
			}
			else
			{
				painter.setPen(Qt::gray);
			}
			QString time = m_user->m_lastMsgTime.time().toString("hh:mm");
			m_time_rect = getCalculateTextRects(time, painter.font());
			m_time_rect.value().moveTopLeft(QPoint(this->width() - m_time_rect.value().width() - 2.5 * commonSpace, m_icon_rect.top() + 0.2 * commonSpace));
			painter.setFont(QFont("Microsoft YaHei", 8));
			painter.drawText(m_time_rect.value(), time, Qt::AlignLeft | Qt::AlignVCenter);
			painter.restore();
		}
		if (m_state_rect.has_value())
		{
			painter.save();
			m_state_rect = QRect(m_time_rect.value().left() + commonSpace, m_time_rect.value().bottom() + commonSpace, 2 * commonSpace, 2 * commonSpace);
			if (m_user->m_newMsgCount <= 0)
			{
				if (m_isSelected)
				{
					painter.drawPixmap(m_state_rect.value(), m_distrupIcon.value().pixmap(m_state_rect.value().size(), QIcon::Normal, QIcon::Off));
				}
				else
				{
					painter.drawPixmap(m_state_rect.value(), m_distrupIcon.value().pixmap(m_state_rect.value().size(), QIcon::Normal, QIcon::On));
				}
			}
			else if (m_user->m_state == ClientConfigs::UserFriend_C::UserStateType::distrup)
			{
				painter.setFont(QFont("Microsoft YaHei", 7));
				painter.setBrush(Qt::gray);
				painter.setPen(Qt::transparent);
				painter.drawEllipse(m_state_rect.value());
				painter.setPen(QColor(255, 255, 255));
				if (m_user->m_newMsgCount < 100)
				{
					painter.drawText(m_state_rect.value(), Qt::AlignCenter, QString::number(m_user->m_newMsgCount));
				}
				else
				{
					painter.drawText(m_state_rect.value(), Qt::AlignCenter, "99+");
				}
			}
			else if (m_user->m_newMsgCount > 0)
			{
				painter.setFont(QFont("Microsoft YaHei", 7));
				painter.setBrush(Qt::red);
				painter.setPen(Qt::transparent);
				painter.drawEllipse(m_state_rect.value());
				painter.setPen(QColor(255, 255, 255));
				if (m_user->m_newMsgCount < 100)
				{
					painter.drawText(m_state_rect.value(), Qt::AlignCenter, QString::number(m_user->m_newMsgCount));
				}
				else
				{
					painter.drawText(m_state_rect.value(), Qt::AlignCenter, "99+");
				}
			}
			painter.restore();
		}
		break;
	}
	case ClientEnums::PeopleWidgetType::groupchats:
	{
		if (m_group == nullptr)
		{
			return;
		}
		painter.save();
		painter.setFont(QFont("Microsoft YaHei", 12));
		if (m_name.isEmpty())
		{
			m_name = QString::fromLocal8Bit(m_group->m_name.c_str());
			if (m_group->m_nikeName != "")
			{
				m_name = m_group->m_nikeName;
			}
		}
		m_name_rect = getCalculateTextRects(m_name, painter.font(), m_maxStringWidth, true);
		m_name_rect.moveTopLeft(QPoint(m_icon_rect.right() + commonSpace, m_icon_rect.top()));
		painter.drawText(m_name_rect, m_name, Qt::AlignLeft | Qt::AlignVCenter);
		painter.restore();
		if (m_message_rect.has_value())
		{
			painter.save();
			painter.setFont(QFont("Microsoft YaHei", 10));
			if (m_message.has_value() && m_message.value().isEmpty())
			{
				m_message = m_group->m_lastMsg;
			}
			if (m_isSelected)
			{
				painter.setPen(m_selectedColor);
			}
			else
			{
				painter.setPen(Qt::gray);
			}
			m_message_rect = getCalculateTextRects(m_message.value(), painter.font(), m_maxStringWidth, true);
			m_message_rect.value().moveTopLeft(QPoint(m_name_rect.left(), m_name_rect.bottom()));
			painter.drawText(m_message_rect.value(), m_message.value(), Qt::AlignLeft | Qt::AlignVCenter);
			painter.restore();
		}
		if (m_time_rect.has_value())
		{
			painter.save();
			painter.setFont(QFont("Microsoft YaHei", 8));
			if (m_isSelected)
			{
				painter.setPen(m_selectedColor);
			}
			else
			{
				painter.setPen(Qt::gray);
			}
			QString time = m_group->m_lastMsgTime.time().toString("hh:mm");
			m_time_rect = getCalculateTextRects(time, painter.font());
			m_time_rect.value().moveTopLeft(QPoint(this->width() - m_time_rect.value().width() - 2.5 * commonSpace, m_icon_rect.top() + 0.2 * commonSpace));
			painter.setFont(QFont("Microsoft YaHei", 8));
			painter.drawText(m_time_rect.value(), time, Qt::AlignLeft | Qt::AlignTop);
			painter.restore();
		}
		if (m_state_rect.has_value())
		{
			painter.save();
			m_state_rect = QRect(m_time_rect.value().left() + commonSpace, m_time_rect.value().bottom() + commonSpace, 2 * commonSpace, 2 * commonSpace);
			if (m_group->m_newMsgCount <= 0)
			{
				if (m_isSelected)
				{
					painter.drawPixmap(m_state_rect.value(), m_distrupIcon.value().pixmap(m_state_rect.value().size(), QIcon::Normal, QIcon::Off));
				}
				else
				{
					painter.drawPixmap(m_state_rect.value(), m_distrupIcon.value().pixmap(m_state_rect.value().size(), QIcon::Normal, QIcon::On));
				}
			}
			else if (m_group->m_state == ClientConfigs::UserGroup_C::GroupStateType::disnotice)
			{
				painter.setFont(QFont("Microsoft YaHei", 7));
				painter.setBrush(Qt::gray);
				painter.setPen(Qt::transparent);
				painter.drawEllipse(m_state_rect.value());
				painter.setPen(QColor(255, 255, 255));
				if (m_group->m_newMsgCount < 100)
				{
					painter.drawText(m_state_rect.value(), Qt::AlignCenter, QString::number(m_group->m_newMsgCount));
				}
				else
				{
					painter.drawText(m_state_rect.value(), Qt::AlignCenter, "99+");
				}
			}
			else if (m_group->m_newMsgCount > 0)
			{
				painter.setFont(QFont("Microsoft YaHei", 7));
				painter.setBrush(Qt::gray);
				painter.setPen(Qt::transparent);
				painter.drawEllipse(m_state_rect.value());
				painter.setPen(QColor(255, 255, 255));
				if (m_group->m_newMsgCount < 100)
				{
					painter.drawText(m_state_rect.value(), Qt::AlignCenter, QString::number(m_group->m_newMsgCount));
				}
				else
				{
					painter.drawText(m_state_rect.value(), Qt::AlignCenter, "99+");
				}
			}
			painter.restore();
		}
		break;
	}
	case ClientEnums::PeopleWidgetType::friends:
	{
		if (m_user == nullptr)
		{
			return;
		}
		painter.save();
		painter.setFont(QFont("Microsoft YaHei", 12));
		if (m_name.isEmpty())
		{
			m_name = QString::fromLocal8Bit(m_user->m_name.c_str());
			if (m_user->m_nikeName != "")
			{
				m_name = m_name + QString("(%1)").arg(m_user->m_nikeName);
			}
		}
		m_name_rect = getCalculateTextRects(m_name, painter.font(), m_maxStringWidth, true);
		m_name_rect.moveTopLeft(QPoint(m_icon_rect.right() + commonSpace, m_icon_rect.top()));
		painter.drawText(m_name_rect, m_name, Qt::AlignLeft | Qt::AlignVCenter);
		painter.restore();
		if (m_message_rect.has_value())
		{
			painter.save();
			painter.setFont(QFont("Microsoft YaHei", 10));
			if (m_message.has_value() && m_message.value().isEmpty())
			{
				m_message = QString::fromLocal8Bit(m_user->m_sign.c_str());
			}
			if (m_isSelected)
			{
				painter.setPen(m_selectedColor);
			}
			else
			{
				painter.setPen(Qt::gray);
			}
			m_message_rect = getCalculateTextRects(m_message.value(), painter.font(), m_maxStringWidth, true);
			m_message_rect.value().moveTopLeft(QPoint(m_name_rect.left(), m_name_rect.bottom()));
			painter.drawText(m_message_rect.value(), m_message.value(), Qt::AlignLeft | Qt::AlignVCenter);
			painter.restore();
		}
		break;
	}
	case ClientEnums::PeopleWidgetType::groups:
	{
		if (m_group == nullptr)
		{
			return;
		}
		painter.save();
		painter.setFont(QFont("Microsoft YaHei", 12));
		if (m_name.isEmpty())
		{
			m_name = QString::fromLocal8Bit(m_group->m_name.c_str());
			if (m_group->m_nikeName != "")
			{
				m_name = m_name + QString("(%1)").arg(m_group->m_nikeName);
			}
		}
		m_name_rect = getCalculateTextRects(m_name, painter.font(), m_maxStringWidth, true);
		m_name_rect.moveTopLeft(QPoint(m_icon_rect.right() + commonSpace, this->height() / 2 - m_name_rect.height() / 2));
		painter.drawText(m_name_rect, m_name, Qt::AlignLeft | Qt::AlignVCenter);
		painter.restore();
		return;
	}
	case ClientEnums::PeopleWidgetType::friendappend:
	{
		if (m_user == nullptr)
		{
			return;
		}
		painter.save();
		painter.setFont(QFont("Microsoft YaHei", 12));
		if (m_name.isEmpty())
		{
			m_name = QString::fromLocal8Bit(m_user->m_name.c_str());
		}
		m_name_rect = getCalculateTextRects(m_name, painter.font(), m_maxStringWidth, true);
		m_name_rect.moveTopLeft(QPoint(m_icon_rect.right() + commonSpace, m_icon_rect.top()));
		painter.drawText(m_name_rect, m_name, Qt::AlignLeft | Qt::AlignTop);
		painter.restore();
		if (m_message_rect.has_value())
		{
			painter.save();
			painter.setFont(QFont("Microsoft YaHei", 10));
			if (m_message.has_value() && m_message.value().isEmpty())
			{
				m_message = QString::fromStdString(m_user->m_account.c_str());
			}
			painter.setPen(Qt::gray);
			m_message_rect = getCalculateTextRects(m_message.value(), painter.font(), m_maxStringWidth, true);
			m_message_rect.value().moveTopLeft(QPoint(m_name_rect.left(), m_name_rect.bottom()));
			painter.drawText(m_message_rect.value(), m_message.value(), Qt::AlignLeft | Qt::AlignVCenter);
			painter.restore();
		}
		break;
	}
	case ClientEnums::PeopleWidgetType::groupappend:
	{
		if (m_group == nullptr)
		{
			return;
		}
		painter.save();
		painter.setFont(QFont("Microsoft YaHei", 12));
		if (m_name.isEmpty())
		{
			m_name = QString::fromLocal8Bit(m_group->m_name.c_str());
		}
		m_name_rect = getCalculateTextRects(m_name, painter.font(), m_maxStringWidth, true);
		m_name_rect.moveTopLeft(QPoint(m_icon_rect.right() + commonSpace, m_icon_rect.top()));
		painter.drawText(m_name_rect, m_name, Qt::AlignLeft | Qt::AlignVCenter);
		painter.restore();
		if (m_message_rect.has_value())
		{
			painter.save();
			painter.setFont(QFont("Microsoft YaHei", 10));
			if (m_message.has_value() && m_message.value().isEmpty())
			{
				m_message = QString::fromStdString(m_group->m_account.c_str());
			}
			painter.setPen(Qt::gray);
			m_message_rect = getCalculateTextRects(m_message.value(), painter.font(), m_maxStringWidth, true);
			m_message_rect.value().moveTopLeft(QPoint(m_name_rect.left(), m_name_rect.bottom()));
			painter.drawText(m_message_rect.value(), m_message.value(), Qt::AlignLeft | Qt::AlignVCenter);
			painter.restore();
		}
		break;
	}
	default:
		break;
	}
}
