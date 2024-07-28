#include "QQOptionFillButton.h"

void QQOptionFillButtonProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == PE_IndicatorRadioButton)
	{
		return;
	}
	return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void QQOptionFillButtonProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == CE_RadioButton)
	{
		QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
		if (buttonOption && (buttonOption->state & State_Sunken))
		{
			buttonOption->state &= ~State_Sunken;
			buttonOption->state &= ~State_MouseOver;
		}
		if (buttonOption && (buttonOption->state & State_MouseOver))
		{
			buttonOption->state &= ~State_MouseOver;
			painter->fillRect(option->rect, QColor("#C5C4C5"));
		}
		if (buttonOption && (buttonOption->state & State_On))
		{
			buttonOption->state &= ~State_On;
			painter->fillRect(option->rect, QColor("#C5C4C5"));
		}
	}
	else if (element == CE_RadioButtonLabel && widget)
	{
		QStyleOptionButton *buttonOption = const_cast<QStyleOptionButton *>(qstyleoption_cast<const QStyleOptionButton *>(option));
		const QRadioButton *radioButton = qobject_cast<const QRadioButton *>(widget);
		if (buttonOption && radioButton)
		{
			QRect rect = option->rect;
			QPixmap pixmap = buttonOption->icon.pixmap(buttonOption->iconSize);
			int x = (radioButton->rect().width() - pixmap.width()) / 2;
			int y = (radioButton->rect().height() - pixmap.height()) / 2;
			painter->drawPixmap(x, y, pixmap);
			return;
		}
	}
	return QProxyStyle::drawControl(element, option, painter, widget);
}

QQOptionFillButton::QQOptionFillButton(QWidget *parent)
	: QRadioButton(parent)
{
	m_width = 0;
	m_height = 0;
	m_newMsgCount = 0;
	m_round = 5;
	m_timer = new QTimer(this);
	m_isIn = false;
	m_proxyStyle = new QQOptionFillButtonProxyStyle;
	this->setStyle(m_proxyStyle);
	connect(m_timer, &QTimer::timeout, this, &QQOptionFillButton::do_timeout);
	connect(this, &QRadioButton::toggled, this, &QQOptionFillButton::do_toggled);
}

QQOptionFillButton::~QQOptionFillButton()
{
	delete m_proxyStyle;
}

void QQOptionFillButton::setIcon(const QIcon &icon)
{
	m_icon = icon;
	QRadioButton::setIcon(icon);
}

void QQOptionFillButton::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QPixmap selectPixmap = m_icon.pixmap(this->iconSize(), QIcon::Normal, QIcon::On);
	QPixmap unSelectPixmap = m_icon.pixmap(this->iconSize(), QIcon::Normal, QIcon::Off);
	QPixmap pixmap = QPixmap(this->iconSize());
	pixmap.fill(Qt::transparent);
	QPainter pixmapPainter(&pixmap);
	pixmapPainter.setRenderHint(QPainter::Antialiasing);
	QPainterPath leftBottomTriangle;
	QPainterPath rightTopTriangle;
	if (m_width < this->iconSize().width())
	{
		leftBottomTriangle.moveTo(0, this->iconSize().height());
		leftBottomTriangle.lineTo(QPoint(0, this->iconSize().height()) + QPoint(0, -m_width));
		leftBottomTriangle.lineTo(QPoint(0, this->iconSize().height()) + QPoint(m_width, 0));
		leftBottomTriangle.closeSubpath();
		rightTopTriangle.moveTo(this->iconSize().width(), 0);
		rightTopTriangle.lineTo(this->iconSize().width(), this->iconSize().height());
		rightTopTriangle.lineTo(QPoint(0, this->iconSize().height()) + QPoint(m_width, 0));
		rightTopTriangle.lineTo(QPoint(0, this->iconSize().height()) + QPoint(0, -m_width));
		rightTopTriangle.lineTo(0, 0);
		rightTopTriangle.closeSubpath();
	}
	else
	{
		leftBottomTriangle.moveTo(0, this->iconSize().height());
		leftBottomTriangle.lineTo(0, 0);
		leftBottomTriangle.lineTo(QPoint(0, 0) + QPoint(m_height, 0));
		leftBottomTriangle.lineTo(QPoint(this->iconSize().width(), this->iconSize().height()) + QPoint(0, -m_height));
		leftBottomTriangle.lineTo(this->iconSize().width(), this->iconSize().height());
		leftBottomTriangle.closeSubpath();
		rightTopTriangle.moveTo(this->iconSize().width(), 0);
		rightTopTriangle.lineTo(QPoint(0, 0) + QPoint(m_height, 0));
		rightTopTriangle.lineTo(QPoint(this->iconSize().width(), this->iconSize().height()) + QPoint(0, -m_height));
		rightTopTriangle.closeSubpath();
	}
	pixmapPainter.setClipPath(leftBottomTriangle);
	pixmapPainter.drawPixmap(QPoint(0, 0), selectPixmap);
	pixmapPainter.setClipPath(rightTopTriangle);
	pixmapPainter.drawPixmap(QPoint(0, 0), unSelectPixmap);
	QRadioButton::setIcon(QIcon(pixmap));

	if (m_newMsgCount > 0)
	{
		m_newMsgRect.setRect(width() - 12, 0, 12, 12);
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(255, 0, 0));
		painter.drawEllipse(m_newMsgRect);
		QPen penNew;
		penNew.setColor(QColor(Qt::white));
		painter.setPen(penNew);
		QString number = "";
		if (m_newMsgCount < 100)
		{
			number = QString::number(m_newMsgCount);
		}
		else
		{
			number = "99+";
		}
		QFont font = QFont("Arial", 7);
		QFontMetrics fm(font);
		int numberWidth = fm.horizontalAdvance(number);
		int numberHeight = fm.height();
		painter.setFont(font);
		painter.drawText(m_newMsgRect.x() + 6 - numberWidth / 2, m_newMsgRect.y() + 7 + numberHeight / 4, number);
	}

	return QRadioButton::paintEvent(event);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void QQOptionFillButton::enterEvent(QEnterEvent *event)
#else
void QQOptionFillButton::enterEvent(QEvent *event)
#endif
{
	if (this->isChecked())
		return;
	m_isIn = true;
	m_timer->start(2);
	return QRadioButton::enterEvent(event);
}

void QQOptionFillButton::leaveEvent(QEvent *event)
{
	if (this->isChecked())
		return;
	m_isIn = false;
	m_timer->start(2);
	return QRadioButton::leaveEvent(event);
}

void QQOptionFillButton::mousePressEvent(QMouseEvent *event)
{
	if (isEnabled())
	{
		if (event->buttons() & Qt::LeftButton)
		{
			return QRadioButton::mousePressEvent(event);
		}
		else
		{
			event->ignore();
		}
	}
}

void QQOptionFillButton::mouseReleaseEvent(QMouseEvent *event)
{
	if (isEnabled())
	{
		if ((event->type() == QMouseEvent::MouseButtonRelease) && (event->button() == Qt::LeftButton))
		{
			if (m_width < this->iconSize().width())
				m_width = this->iconSize().width();
			if (m_height < this->iconSize().height())
				m_height = this->iconSize().height();
			this->update();
			m_timer->stop();
			return QRadioButton::mouseReleaseEvent(event);
		}
		else
		{
			event->ignore();
		}
	}
}

void QQOptionFillButton::do_toggled(bool isChecked)
{
	m_isIn = isChecked;
	m_timer->start(2);
	this->update();
}

void QQOptionFillButton::do_timeout()
{
	if (m_isIn)
	{
		if (m_width < this->iconSize().width())
		{
			m_width += 1;
		}
		else if (m_height < this->iconSize().height())
		{
			m_height += 1;
		}
		else
		{
			m_timer->stop();
		}
	}
	else
	{
		if (m_height > 0)
		{
			m_height -= 1;
		}
		else if (m_width > 0)
		{
			m_width -= 1;
		}
		else
		{
			m_timer->stop();
		}
	}
	this->update();
}
