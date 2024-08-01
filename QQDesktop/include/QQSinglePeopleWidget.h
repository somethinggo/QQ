#pragma once
// 个人信息索引封装

#include <qwidget.h>
#include <qicon.h>
#include <qdatetime.h>
#include <qrect.h>
#include <qfont.h>
#include <qcolor.h>
#include <qmenu.h>
#include <qaction.h>
#include <qevent.h>
#include <qstyle.h>
#include <qapplication.h>
#include <optional>

#include "qfunction.hpp"

class QQSinglePeopleWidget : public QWidget
{
	Q_OBJECT
public:
	explicit QQSinglePeopleWidget(QWidget *parent = nullptr);
	virtual ~QQSinglePeopleWidget();
	void setMBasePeopleInformation(ClientConfigs::UserFriend_C *user, ClientEnums::PeopleWidgetType type = ClientEnums::PeopleWidgetType::friendchats);
	void setMBasePeopleInformation(ClientConfigs::UserGroup_C *group, ClientEnums::PeopleWidgetType type = ClientEnums::PeopleWidgetType::groupchats);
	QPair<ClientConfigs::UserFriend_C *, ClientConfigs::UserGroup_C *> getMBasePeopleInformation();
	ClientEnums::PeopleWidgetType getMWidgetType();
	void setMMaxStringLength(qreal length);
	void setMSelectContentsColor(QColor color, bool selected);

protected:
	void paintEvent(QPaintEvent *event) override;

private:
	ClientConfigs::UserFriend_C *m_user;
	ClientConfigs::UserGroup_C *m_group;
	ClientEnums::PeopleWidgetType m_type;
	qreal m_maxStringWidth;
	bool m_isSelected;
	QColor m_selectedColor;
	QRect m_icon_rect;
	QRect m_name_rect;

	QPixmap m_icon;
	QString m_name;
	std::optional<QString> m_message;

	std::optional<QIcon> m_distrupIcon;	 // 是否屏蔽图标--仅用于聊天窗口
	std::optional<QRect> m_message_rect; // 最后信息内容--仅用于聊天窗口，个人签名--仅用于好友窗口，用户账号--仅用于添加界面
	std::optional<QRect> m_state_rect;	 // 单人信息状态--仅用于聊天窗口
	std::optional<QRect> m_time_rect;	 // 最后信息时间--仅用于聊天窗口
signals:
	void sign_contextMenuClicked(QAction *action);
};