#include "QMStackedWidget.h"

QMStackedWidget::QMStackedWidget(QWidget *parent)
	: QStackedWidget(parent)
{
	m_value = 0;
	m_nextIndex = 0;
	m_direction = Direction::LeftToRight;
	m_isAnimation = false;
	m_animation = new QPropertyAnimation(this, QByteArray(), this);
	m_animation->setDuration(500);
	connect(m_animation, &QPropertyAnimation::valueChanged, this, &QMStackedWidget::do_animationValueChanged);
	connect(m_animation, &QPropertyAnimation::finished, this, &QMStackedWidget::do_animationFinished);
}

QMStackedWidget::~QMStackedWidget()
{
}

void QMStackedWidget::setMCurrentIndex(int index, Direction direction)
{
	emit currentChanged(index);
	if (m_isAnimation)
	{
		return;
	}
	this->widget(currentIndex())->hide();
	m_nextIndex = index;
	m_direction = direction;
	switch (m_direction)
	{
	case Direction::LeftToRight:
		m_animation->setStartValue(geometry().width());
		m_animation->setEndValue(0);
		m_animation->start();
		break;
	case Direction::RightToLeft:
		m_animation->setStartValue(0);
		m_animation->setEndValue(geometry().width());
		m_animation->start();
		break;
	default:
		break;
	}
}

void QMStackedWidget::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	QPainter painter(this);
	QRect baseRect = this->geometry();

	if (m_direction == Direction::LeftToRight)
	{
		QPixmap tempPixmapOne(this->size());
		this->widget(this->currentIndex())->render(&tempPixmapOne);
		QRectF targetOne(0, 0, m_value, baseRect.height());
		QRectF sourceOne(baseRect.width() - m_value, 0, m_value, baseRect.height());
		painter.drawPixmap(targetOne, tempPixmapOne, sourceOne);

		QPixmap tempPixmapTwo(this->size());
		this->widget(m_nextIndex)->resize(this->size());
		this->widget(m_nextIndex)->render(&tempPixmapTwo);
		QRectF targetTwo(m_value, 0, baseRect.width() - m_value, baseRect.height());
		QRectF sourceTwo(0, 0, baseRect.width() - m_value, baseRect.height());
		painter.drawPixmap(targetTwo, tempPixmapTwo, sourceTwo);
	}
	else if (m_direction == Direction::RightToLeft)
	{
		QPixmap tempPixmapOne(this->size());
		this->widget(this->currentIndex())->render(&tempPixmapOne);
		QRectF targetOne(m_value, 0, baseRect.width() - m_value, baseRect.height());
		QRectF sourceOne(0, 0, baseRect.width() - m_value, baseRect.height());
		painter.drawPixmap(targetOne, tempPixmapOne, sourceOne);

		QPixmap tempPixmapTwo(this->size());
		this->widget(m_nextIndex)->resize(this->size());
		this->widget(m_nextIndex)->render(&tempPixmapTwo);
		QRectF targetTwo(0, 0, m_value, baseRect.height());
		QRectF sourceTwo(baseRect.width() - m_value, 0, m_value, baseRect.height());
		painter.drawPixmap(targetTwo, tempPixmapTwo, sourceTwo);
	}
}

void QMStackedWidget::do_animationValueChanged(const QVariant &value)
{
	m_value = value.toDouble();
	this->update();
}

void QMStackedWidget::do_animationFinished()
{
	m_isAnimation = false;
	this->setCurrentIndex(m_nextIndex);
	this->update();
}