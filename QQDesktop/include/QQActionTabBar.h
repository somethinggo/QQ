#pragma once
// 动画的自定义的仿QQ的QTabBar组件--未实现动画
#include <qtabbar.h>
#include <qproxystyle.h>
#include <qstyleoption.h>
#include <qpainter.h>
#include <qpainterpath.h>

#include "qfunction.hpp"

class QQActionTabBarProxyStyle : public QProxyStyle
{
public:
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

class QQActionTabBar : public QTabBar
{
	Q_OBJECT
	// 注册自定义属性
	Q_MPROPERTY_CREATE(qreal, MMaxWidth, m_maxWidth)				// 选项卡最大宽度
	Q_MPROPERTY_CREATE(QColor, MBackgroundColor, m_backgroundColor) // 背景板颜色
	Q_MPROPERTY_CREATE(QColor, MHoverColor, m_hoverColor)			// 鼠标选项卡悬停颜色
	Q_MPROPERTY_CREATE(QColor, MSelectColor, m_selectColor)			// 选中选项卡颜色
	Q_MPROPERTY_CREATE(QColor, MSelectTextColor, m_selectTextColor) // 选中选项卡文字颜色
public:
	explicit QQActionTabBar(QWidget *parent = nullptr);
	virtual ~QQActionTabBar();
	void setMMargin(int margin);										// 设置内部选项卡的外边距
	void setMContentsMargins(int left, int top, int right, int bottom); // 设置内部选项卡的外边距
	void setMContentsMargins(const QMargins &margins);					// 设置内部选项卡的外边距
	QMargins getMContentsMargins() const;								// 获取内部选项卡的外边距

protected:
	QSize tabSizeHint(int index) const override;

private:
	QQActionTabBarProxyStyle *m_proxyStyle;
	QMargins m_margins;
};
