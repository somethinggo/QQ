#pragma once

#include <qlistview.h>
#include <qstandarditemmodel.h>
#include <qevent.h>
#include <qscrollbar.h>
#include <qproxystyle.h>
#include <qstyleditemdelegate.h>
#include <qgraphicseffect.h>
#include <qmenu.h>
#include <qmimedata.h>

#include <ElaScrollbar.h>

#include "qfunction.hpp"

// 鼠标滚动像素级别的ListView
class QQScrollerPixelListView : public QListView
{
	Q_OBJECT
public:
	explicit QQScrollerPixelListView(QWidget *parent = nullptr);
	virtual ~QQScrollerPixelListView();

protected:
	void wheelEvent(QWheelEvent *event) override;
};

class QQChatScrollerPixelListViewProxyStyle : public QProxyStyle
{
public:
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

// 聊天界面的滚动像素级别的ListView
class QQChatScrollerPixelListView : public QQScrollerPixelListView
{
	Q_OBJECT

public:
	explicit QQChatScrollerPixelListView(QWidget *parent = nullptr);
	virtual ~QQChatScrollerPixelListView();
	void setMMenu(QMenu *menu);
	void setMMenus(QList<QMenu *> menus);
	QPoint getMCurrentPoint();

protected:
	void resizeEvent(QResizeEvent *event) override;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	void enterEvent(QEnterEvent *event) override; // 鼠标进入事件--显示滚动条
#else
	void enterEvent(QEvent *event) override;
#endif
	void leaveEvent(QEvent *event) override; // 鼠标离开事件--隐藏滚动条
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void contextMenuEvent(QContextMenuEvent *event) override;

private:
	QQChatScrollerPixelListViewProxyStyle *m_proxyStyle;
	QPoint m_currentPoint;
	QList<QMenu *> m_menus;
	QModelIndex m_currentIndex;
	ElaScrollBar *m_scrollBar;
};
