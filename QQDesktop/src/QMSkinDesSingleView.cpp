#include "QMSkinDesSingleView.h"

QMSkinDesSingleView::QMSkinDesSingleView(QWidget *parent)
	: QWidget(parent)
{
	this->setStyleSheet(QString("background-color:blue;border:1px solid gray;border-radius:5px"));
	m_config = nullptr;
	m_pixmapLabel = new QLabel(this);
	m_pixmapLabel->setScaledContents(true);
	m_textLabel = new QLabel(this);
	m_animation = new QPropertyAnimation(m_textLabel, "geometry", this);
	m_layout = new QVBoxLayout(this);
	m_layout->addWidget(m_pixmapLabel);
}

QMSkinDesSingleView::~QMSkinDesSingleView()
{
}

void QMSkinDesSingleView::setMConfig(BaseConfigs::SkinBaseConfig *config)
{
	m_config = config;
	m_pixmapLabel->setPixmap(QPixmap(QString::fromStdString(m_config->m_icon)));
	m_textLabel->setText(QString::fromStdString(m_config->m_describe));
	m_textLabel->setAlignment(Qt::AlignCenter);
	m_textLabel->setStyleSheet("color:white");
	m_textLabel->setWordWrap(true);
	m_textLabel->setGeometry(QRect(10, height(), width() - 20, height() / 2 - 10));
	m_textLabel->show();
}

const BaseConfigs::SkinBaseConfig *QMSkinDesSingleView::getMConfig()
{
	return m_config;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void QMSkinDesSingleView::enterEvent(QEnterEvent *event)
#else
void QMSkinDesSingleView::enterEvent(QEvent *event)
#endif
{
	m_animation->setDirection(QAbstractAnimation::Forward);
	m_animation->start();
}

void QMSkinDesSingleView::leaveEvent(QEvent *event)
{
	m_animation->setDirection(QAbstractAnimation::Backward);
	m_animation->start();
}

void QMSkinDesSingleView::resizeEvent(QResizeEvent *event)
{
	m_textLabel->setGeometry(QRect(10, height(), width() - 20, height() / 2 - 10));
	// 初始化动画属性
	m_animation->setDuration(300);
	m_animation->setEasingCurve(QEasingCurve::InOutQuad);
	m_animation->setStartValue(QRect(10, height(), width() - 20, height() / 2 - 10));
	m_animation->setEndValue(QRect(10, height() / 2, width() - 20, height() / 2 - 10));
	m_animation->setDirection(QAbstractAnimation::Forward);
	return QWidget::resizeEvent(event);
}

void QMSkinDesSingleView::mousePressEvent(QMouseEvent *event)
{
	emit sign_clicked(m_config);
	return QWidget::mousePressEvent(event);
}
