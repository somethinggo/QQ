#include "QQScrollerPixelListView.h"

QQScrollerPixelListView::QQScrollerPixelListView(QWidget *parent)
	: QListView(parent)
{
	this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}

QQScrollerPixelListView::~QQScrollerPixelListView()
{
}

void QQScrollerPixelListView::wheelEvent(QWheelEvent *event)
{
	int value = event->angleDelta().y();
	this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - value);
	event->accept();
}

void QQChatScrollerPixelListViewProxyStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
	if (element == PE_Frame)
	{
		return;
	}
	return QProxyStyle::drawPrimitive(element, option, painter, widget);
}

QQChatScrollerPixelListView::QQChatScrollerPixelListView(QWidget *parent)
	: QQScrollerPixelListView(parent)
{
	m_proxyStyle = new QQChatScrollerPixelListViewProxyStyle;
	m_currentPoint = QPoint(0, 0);
	m_scrollBar = new ElaScrollBar(this);
	m_scrollBar->setFixedWidth(5);
	m_scrollBar->hide();
	this->setStyle(m_proxyStyle);
	connect(m_scrollBar, &QScrollBar::valueChanged, this->verticalScrollBar(), &QScrollBar::setValue);
	connect(this->verticalScrollBar(), &QScrollBar::valueChanged, m_scrollBar, &QScrollBar::setValue);
}

QQChatScrollerPixelListView::~QQChatScrollerPixelListView()
{
	delete m_proxyStyle;
}

void QQChatScrollerPixelListView::setMMenu(QMenu *menu)
{
	m_menus.append(menu);
}

QPoint QQChatScrollerPixelListView::getMCurrentPoint()
{
	return m_currentPoint;
}

void QQChatScrollerPixelListView::resizeEvent(QResizeEvent *event)
{
	m_scrollBar->setMinimum(this->verticalScrollBar()->minimum());
	m_scrollBar->setMaximum(this->verticalScrollBar()->maximum());
	m_scrollBar->setSingleStep(this->verticalScrollBar()->singleStep());
	m_scrollBar->setPageStep(this->verticalScrollBar()->pageStep());
	m_scrollBar->setGeometry(this->width() - 10, 0, 10, this->height());
	return QListView::resizeEvent(event);
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
void QQChatScrollerPixelListView::enterEvent(QEnterEvent *event)
#else
void QQChatScrollerPixelListView::enterEvent(QEvent *event)
#endif
{
	QSize viewportSize = this->viewport()->size();
	QSize contentSize = QSize(viewportSize.width(), this->sizeHintForRow(0) * this->model()->rowCount());
	if (viewportSize.height() < contentSize.height())
	{
		m_scrollBar->show();
	}
	else
	{
		m_scrollBar->hide();
	}
	return QListView::enterEvent(event);
}

void QQChatScrollerPixelListView::leaveEvent(QEvent *event)
{
	m_scrollBar->hide();
	return QListView::leaveEvent(event);
}

void QQChatScrollerPixelListView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		QModelIndex index = this->indexAt(event->pos());
		if (index.isValid())
		{
			m_currentIndex = index;
			this->setCurrentIndex(index);
		}
	}
	else if (event->button() == Qt::RightButton)
	{
		m_currentPoint = event->pos();
		return;
	}
	return QListView::mousePressEvent(event);
}

void QQChatScrollerPixelListView::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		QModelIndex index = this->indexAt(event->pos());
		if (index.isValid())
		{
			if (m_currentIndex != index)
			{
				return;
			}
		}
	}
	else if (event->buttons() & Qt::RightButton)
	{
		m_currentPoint = event->pos();
	}
	return QListView::mouseMoveEvent(event);
}

void QQChatScrollerPixelListView::contextMenuEvent(QContextMenuEvent *event)
{
	QModelIndex index = this->indexAt(event->pos());
	if (index.isValid())
	{
		if (m_menus[0] != nullptr)
		{
			m_menus[0]->exec(QCursor::pos());
		}
	}
	return QListView::contextMenuEvent(event);
}