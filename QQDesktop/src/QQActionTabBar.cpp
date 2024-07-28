#include "QQActionTabBar.h"

void QQActionTabBarProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	// 绘制背景板
	if (element == PE_FrameTabBarBase)
	{
		QQActionTabBar *tabBar = const_cast<QQActionTabBar *>(qobject_cast<const QQActionTabBar *>(widget));
		painter->save();
		QPainterPath path;
		path.addRoundedRect(widget->rect(), 5, 5);
		painter->setRenderHint(QPainter::Antialiasing, true);
		painter->fillPath(path, QBrush(tabBar->getMBackgroundColor()));
		painter->restore();
		return;
	}
	return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

void QQActionTabBarProxyStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == CE_TabBarTab)
	{
		QStyleOptionTab *tabOption = const_cast<QStyleOptionTab *>(qstyleoption_cast<const QStyleOptionTab *>(option));
		QQActionTabBar *tabBar = const_cast<QQActionTabBar *>(qobject_cast<const QQActionTabBar *>(widget));
		if (tabOption && tabBar)
		{
			// 绘制基本的位置
			QMargins margins = tabBar->getMContentsMargins();
			int width = tabOption->rect.width();
			int height = tabOption->rect.height();
			tabOption->rect.setX(tabOption->rect.x() + margins.left());
			tabOption->rect.setY(tabOption->rect.y() + margins.top());
			tabOption->rect.setWidth(width - margins.left() - margins.right());
			tabOption->rect.setHeight(height - margins.top() - margins.bottom());
			if (!tabOption->icon.isNull())
			{
				QPixmap pixmap = tabOption->icon.pixmap(tabOption->iconSize, QIcon::Normal, QIcon::Off);
				QPoint pos = tabOption->rect.topLeft() + QPoint(tabOption->rect.size().width() / 2 - tabOption->iconSize.width() / 2, tabOption->rect.size().height() / 2 - tabOption->iconSize.height() / 2);
				painter->drawPixmap(pos, pixmap);
				painter->setPen(Qt::color1);
				painter->drawText(tabOption->rect, Qt::AlignBottom | Qt::AlignHCenter, tabOption->text);
			}
			else
			{
				painter->setPen(Qt::color1);
				painter->drawText(tabOption->rect, Qt::AlignCenter, tabOption->text);
			}
		}
		if (tabOption && (tabOption->state & State_MouseOver))
		{
			// 绘制鼠标悬停,改变背景颜色,改变图标
			tabOption->state &= ~State_MouseOver;
			QPainterPath path;
			path.addRoundedRect(option->rect, 5, 5);
			painter->setRenderHint(QPainter::Antialiasing);
			painter->fillPath(path, QBrush(tabBar->getMHoverColor()));
			if (!tabOption->icon.isNull())
			{
				QPixmap pixmap = tabOption->icon.pixmap(tabOption->iconSize, QIcon::Normal, QIcon::On);
				QPoint pos = tabOption->rect.topLeft() + QPoint(tabOption->rect.size().width() / 2 - tabOption->iconSize.width() / 2, tabOption->rect.size().height() / 2 - tabOption->iconSize.height() / 2);
				painter->drawPixmap(pos, pixmap);
				painter->setPen(tabBar->getMSelectTextColor());
				painter->drawText(tabOption->rect, Qt::AlignBottom | Qt::AlignHCenter, tabOption->text);
			}
			else
			{
				painter->setPen(tabBar->getMSelectTextColor());
				painter->drawText(tabOption->rect, Qt::AlignCenter, tabOption->text);
			}
		}
		if (tabOption && (tabOption->state & State_Selected))
		{
			// 绘制选中,改变背景颜色,改变图标
			tabOption->state &= ~State_Selected;
			tabOption->state &= ~State_MouseOver;
			QPainterPath path;
			path.addRoundedRect(option->rect, 5, 5);
			painter->setRenderHint(QPainter::Antialiasing);
			painter->fillPath(path, QBrush(tabBar->getMSelectColor()));
			if (!tabOption->icon.isNull())
			{
				QPixmap pixmap = tabOption->icon.pixmap(tabOption->iconSize, QIcon::Normal, QIcon::On);
				QPoint pos = tabOption->rect.topLeft() + QPoint(tabOption->rect.size().width() / 2 - tabOption->iconSize.width() / 2, tabOption->rect.size().height() / 2 - tabOption->iconSize.height() / 2);
				painter->drawPixmap(pos, pixmap);
				painter->setPen(tabBar->getMSelectTextColor());
				painter->drawText(tabOption->rect, Qt::AlignBottom | Qt::AlignHCenter, tabOption->text);
			}
			else
			{
				painter->setPen(tabBar->getMSelectTextColor());
				painter->drawText(tabOption->rect, Qt::AlignCenter, tabOption->text);
			}
		}
		return;
	}
	return QProxyStyle::drawControl(element, option, painter, widget);
}

QQActionTabBar::QQActionTabBar(QWidget *parent)
	: QTabBar(parent)
{
	m_maxWidth = 0;
	m_backgroundColor = Qt::white;
	m_hoverColor = Qt::black;
	m_selectColor = Qt::black;
	m_selectTextColor = Qt::white;
	m_proxyStyle = new QQActionTabBarProxyStyle;
	this->setStyle(m_proxyStyle);
}

QQActionTabBar::~QQActionTabBar()
{
	delete m_proxyStyle;
}

void QQActionTabBar::setMMargin(int margin)
{
	m_margins = QMargins(margin, margin, margin, margin);
}

void QQActionTabBar::setMContentsMargins(int left, int top, int right, int bottom)
{
	m_margins = QMargins(left, top, right, bottom);
}

void QQActionTabBar::setMContentsMargins(const QMargins &margins)
{
	m_margins = margins;
}

QMargins QQActionTabBar::getMContentsMargins() const
{
	return m_margins;
}

QSize QQActionTabBar::tabSizeHint(int index) const
{
	int width = m_maxWidth == 0 ? this->width() / this->count() : m_maxWidth;
	int height = this->height();
	return QSize(width, height);
}
