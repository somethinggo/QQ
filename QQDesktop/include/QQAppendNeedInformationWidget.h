#pragma once
// 添加好友或群组验证信息窗口
#include <qwidget.h>
#include <qlabel.h>
#include <qscrollarea.h>
#include <qlayout.h>
#include <qdialogbuttonbox.h>
#include <qpushbutton.h>
#include <qproxystyle.h>
#include <qstyleoption.h>

#include "qfunction.hpp"
#include "QQSinglePeopleWidget.h"

class QQAppendNeedInformationWidgetProxyStyle : public QProxyStyle
{
	Q_OBJECT
public:
	void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
};

class QQAppendNeedInformationWidget : public QWidget
{
	Q_OBJECT
	Q_MSINGLETON_CREATE(QQAppendNeedInformationWidget)
private:
	explicit QQAppendNeedInformationWidget(QWidget *parent = nullptr);
	virtual ~QQAppendNeedInformationWidget();

public:
	void setMBasePeopleInformation(ClientConfigs::UserFriend_C *user);								// 设置基础信息--好友
	void setMBasePeopleInformation(ClientConfigs::UserGroup_C *group);								// 设置基础信息--群组
	QPair<ClientConfigs::UserFriend_C *, ClientConfigs::UserGroup_C *> getMBasePeopleInformation(); // 获取基础信息
	void addMMoreInformation(QLabel *label, QWidget *widget);										// 添加更多信息
	void addMMoreInformation(QString name, QWidget *widget);										// 添加更多信息
	QPair<QLabel *, QWidget *> getMInformation(int index);											// 获取更多信息
	void removeMInformation(int index);																// 移除更多信息
	void removeMAllInformation();																	// 移除所有更多信息
private slots:
	void do_clickAppendButton(QAbstractButton *button);

private:
	QQAppendNeedInformationWidgetProxyStyle *m_proxyStyle;
	ClientConfigs::UserFriend_C *m_user;
	ClientConfigs::UserGroup_C *m_group;
	QQSinglePeopleWidget *m_baseInformationWidget;
	QScrollArea *m_moreInformationWidget;
	QDialogButtonBox *m_buttonBox;
	QMap<int, QPair<QLabel *, QWidget *>> m_map; // 更多信息维护
};
